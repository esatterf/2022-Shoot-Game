#include "actor.h"
#include <iostream>

Actor::Actor() {
    xVelo = 0;
    yVelo = 0;
    innerCircle = Circle({1,1,1}, 0, 0, 10*.9);
    outerCircle = Circle({0,0,0}, 0, 0, 10);
}

Actor::Actor(color borderColor, color fillColor, double x, double y, double radius) {
    xVelo = 0;
    yVelo = 0;
    innerCircle = Circle(fillColor, x, y, radius*.9);
    outerCircle = Circle(borderColor, x, y, radius);
}

void Actor::draw() const {
    outerCircle.draw();
    innerCircle.draw();
}

void Actor::update() {
    innerCircle.move(xVelo,yVelo);
    outerCircle.move(xVelo,yVelo);
    xVelo *= friction;
    yVelo *= friction;

}

float Actor::getXPos() const {
    return innerCircle.getCenterX();
}

float Actor::getYPos() const {
    return innerCircle.getCenterY();
}

float Actor::getXVelo() const {
    return xVelo;
}

float Actor::getYVelo() const {
    return yVelo;
}

float Actor::getAngle() const{
    return angle;
}

double Actor::getRadius() const {
    return outerCircle.getRadius();
}

void Actor::setVelo(float dir, float spd) {
    angle = dir;
    speed = spd;
    xVelo = speed * cos(angle);
    yVelo = speed * sin(angle);
}

void Actor::setXVelo(float nx) {
    xVelo = nx;
}

void Actor::setYVelo(float ny) {
    yVelo = ny;
}

void Actor::setXPos(int nxp){
    innerCircle.setCenterX(nxp);
    outerCircle.setCenterX(nxp);
}

void Actor::setYPos(int nyp){
    innerCircle.setCenterY(nyp);
    outerCircle.setCenterY(nyp);
}

void Actor::setFriction(float nf) {
    friction = nf;
}

float Actor::getDistance(Actor &other) {
    float xDist = getXPos()-other.getXPos();
    float yDist = getYPos()-other.getYPos();
    return sqrt(xDist*xDist + yDist*yDist);
}

float Actor::getFriction() {
    return friction;
}

bool Actor::isTouching(Actor &other) {
    if (getDistance(other) < (outerCircle.getRadius() + other.outerCircle.getRadius())){
        return true;
    }
    return false;
}