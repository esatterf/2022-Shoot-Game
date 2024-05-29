#include "bullet.h"

Bullet::Bullet() : Actor({0,0,0},{1,1,1},0,0,10){
    speed = 0;
    direction = 0;
    damage = 0;
    friendly = false;
    setFriction(1);
}

Bullet::Bullet(double x, double y, double angle, int size, int dmg, bool frnd) : Actor({0,0,0},{0,0,0},x,y,size){
    damage = dmg;
    direction = angle;
    friendly = frnd;
    speed = 10;
    setFriction(1);
}

void Bullet::update() {
    setVelo(direction, speed);
    speed += 3;
    if (speed >= maxVelo){
        speed = maxVelo;
    }
    Actor::update();
}

bool Bullet::isFriendly() {
    return friendly;
}

int Bullet::getDamage() {
    return damage;
}