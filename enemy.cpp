#include "enemy.h"
#include <iostream>
#include "circle.h"


Enemy::Enemy() : Actor({0,0,0},{.5,.5,.5},0,0,30) {
    delayTimer = 30;
    delay = delayTimer;
    speed = 2;
    targetDir = (5*PI)/4;
    alive = true;
    hp = 30;
    moveTimer = 0;
    pauseTimer = 13;
}

Enemy::Enemy(double x,double y) : Actor({0,0,0},{.5,.5,.5},x,y,30) {
    delayTimer = 30;
    delay = delayTimer;
    speed = 2;
    targetDir = (5*PI)/4;
    alive = true;
    hp = 30;
    moveTimer = 0;
    pauseTimer = 13;
}

void Enemy::target(Actor &other) {
    float dx = getXPos() - other.getXPos();
    float dy = getYPos() - other.getYPos();
    targetDir = atan(dy / dx);
    if (dx >= 0){
        targetDir += PI;
    }
}

void Enemy::shoot(std::vector<Bullet> &bullets) {
    int bulletSize = 15;
    int power = 10;
    if (delay <= 0) {
        std::random_device rd;
        delay = delayTimer + rd()%5;
        float launchX = getXPos() + bulletSize*1.5 * cos(targetDir);
        float launchY = getYPos() + bulletSize*1.5 * sin(targetDir);
        bullets.push_back(Bullet(launchX, launchY, targetDir, bulletSize, power, false));
    }
}

bool Enemy::update(std::vector<Bullet> &bullets, Actor &player) {
    target(player);
    int bLoc = 0;
    for (Bullet &b : bullets){
        if (isTouching(b) && b.isFriendly()){
            if (b.getDamage() < hp) {
                bullets.erase(bullets.begin() + bLoc);
            }
            hp -= b.getDamage();
        }
        bLoc ++;
    }
    if (hp <= 0){
        die(bullets);
        return false;
    }
    move();
    shoot(bullets);
    Actor::update();
    if (getXPos() + getRadius() > 1500){
        setXPos(1500-getRadius());
    }
    if (getYPos() + getRadius() > 1500){
        setYPos(1500-getRadius());
    }
    if (getXPos() - getRadius() < 0){
        setXPos(getRadius());
    }
    if (getYPos() - getRadius() < 0){
        setYPos(getRadius());
    }
    delay--;
    return true;
}


int Enemy::getHP() {
    return hp;
}

void Enemy::setHP(int nhp) {
    hp = nhp;
}

void Enemy::die(std::vector<Bullet> &bullets) {
    hp = 0;
    alive = false;
    std::random_device rd;
    for (int i = 0; i <= 3+rd()%7; i++){
        float debrisAngle = fmod(rd(),2*PI);
        bullets.push_back(Bullet(getXPos(), getYPos(), debrisAngle, 5 + rd()%10, 0, true));
    }
}

bool Enemy::isAlive() {
    return alive;
}

void Enemy::move(){
    if (moveTimer >= 0){
        moveTimer --;
        setVelo(moveDir, speed);
    }
    else {
        pauseTimer --;
        if (pauseTimer <= 0){
            std::random_device rd;
            pauseTimer = 10 + rd()%10;
            moveTimer = 30 + rd()%10;
            float var = fmod(rd(),PI/8);
            if (rd()%2 == 0){
                var *= -1;
            }
            moveDir = targetDir + var;
        }
    }
}