#include "player.h"
#include <iostream>
#include "circle.h"
#include <random>


Player::Player() : Actor({0,0,0},{1,1,1},0,0,30) {
    // Sets the current weapon to mega but then instantly cycles it to the standard one
    // to reinitialize the delay and delay timers
    currentWeapon = mega;
    cycleWeapon();
    speed = 2;
    targetDir = (5*PI)/4;
    alive = true;
    hp = 100;
}

Player::Player(double x,double y) : Actor({0,0,0},{1,1,1},x,y,30) {
    currentWeapon = mega;
    cycleWeapon();
    speed = 2;
    targetDir = (5*PI)/4;
    alive = true;
    hp = 100;
}

void Player::moveLeft(){
    setXVelo(getXVelo() - speed);
}
void Player::moveRight(){
    setXVelo(getXVelo() + speed);
}
void Player::moveUp(){
    setYVelo(getYVelo() - speed);
}
void Player::moveDown(){
    setYVelo(getYVelo() + speed);
}

void Player::draw(int mx, int my) const {
    drawWeapon(mx, my);
    Actor::draw();
}

void Player::drawWeapon(int mx, int my) const {
    if (currentWeapon == mega){
        int chargeRad = (delay*.3)+30;
        Circle chargeBorder = Circle( {.9,.9,.9}, getXPos(), getYPos(), chargeRad);
        Circle chargeFill = Circle( {1,1,1}, getXPos(), getYPos(), chargeRad*.8);
        chargeBorder.draw();
        chargeFill.draw();
    }

    int length = 150;
    int thickness = 3;
    if (currentWeapon == rapid){
        length = 120;
    }
    if (currentWeapon == mega){
        length = 180;
    }

    float angle = getAngle(mx, my);
    float lnx = length * cos(angle);
    float lny = length * sin(angle);
    float nx = getXPos() + lnx;
    float ny = getYPos() + lny;
    glColor3f(.7, .7, .7);
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(getXPos(), getYPos());
    glVertex2f(nx, ny);
    glEnd();
}

float Player::getAngle(int mx, int my) const {
    // I scraped the depths of my brain to try and remember how geometry works and this is the result
    // It's probably horribly unoptimized but it *does* work
    float dx = getXPos() - mx;
    float dy = getYPos() - my;
    float angle = atan(dy / dx);
    if (dx >= 0){
        angle += PI;
    }
    return angle;
}

void Player::shoot(std::vector<Bullet> &bullets, int mx, int my) {
    int bulletSize = 15;
    int power = 15;
    if (currentWeapon == rapid){
        bulletSize = 10;
        power = 5;
    }
    if (currentWeapon == mega){
        bulletSize = 30;
        power = 100;
    }

    if (delay <= 0) {
        delay = delayTimer;
        targetDir = getAngle(mx, my);
        float launchX = getXPos() + bulletSize*1.5 * cos(targetDir);
        float launchY = getYPos() + bulletSize*1.5 * sin(targetDir);
        bullets.push_back(Bullet(launchX, launchY, targetDir, bulletSize, power, true));
        setXVelo(getXVelo() - (log(power)/15)*bulletSize * cos(targetDir));
        setYVelo(getYVelo() - (log(power)/15)*bulletSize * sin(targetDir));
    }
}

bool Player::update(std::vector<Bullet> &bullets) {
    int bLoc = 0;
    for (Bullet &b : bullets){
        if (isTouching(b) && !b.isFriendly()){
            hp -= b.getDamage();
            bullets.erase(bullets.begin()+bLoc);
        }
        bLoc ++;
    }
    if (hp <= 0){
        die(bullets);
        return false;
    }
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

void Player::cycleWeapon(){
    if (currentWeapon == standard){
        currentWeapon = rapid;
        delayTimer = 5;
        delay = delayTimer;
    }
    else if(currentWeapon == rapid){
        currentWeapon = mega;
        delayTimer = 150;
        delay = delayTimer;
    }
    else{
        currentWeapon = standard;
        delayTimer = 13;
        delay = delayTimer;
    }
}

int Player::getHP() {
    return hp;
}

void Player::setHP(int nhp) {
    hp = nhp;
}

void Player::die(std::vector<Bullet> &bullets) {
    hp = 0;
    alive = false;
    std::random_device rd;
    for (int i = 0; i <= 3+rd()%7; i++){
        float debrisAngle = fmod(rd(),2*PI);
        bullets.push_back(Bullet(getXPos(), getYPos(), debrisAngle, 5 + rd()%10, 50, true));
    }
}

bool Player::isAlive() {
    return alive;
}

std::string Player::getWeapon() {
    if (currentWeapon == standard) {
        return "Standard";
    } else if (currentWeapon == rapid){
        return "Rapid Fire";
    }
    else {
        return "Mega Blast";
    }
}