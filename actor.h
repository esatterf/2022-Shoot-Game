#ifndef actor_h
#define actor_h

#include "circle.h"
#include <string>

class Actor {
private:
    float xVelo, yVelo;
    float angle;
    float speed;
    Circle innerCircle, outerCircle;
    float friction = .85;

public:
    Actor();
    Actor(color border, color fill, double x, double y, double radius);

    // Updates the actor's location
    virtual void update();

    // Draws the actor
    void draw() const;

    // Getters and setters
    float getXPos() const;

    float getYPos() const;

    float getXVelo() const;

    float getYVelo() const;

    float getAngle() const;

    double getRadius() const;

    bool isTouching(Actor &other);

    float getDistance(Actor &other);

    float getFriction();

    void setVelo(float dir, float spd);

    void setXVelo(float nx);

    void setYVelo(float ny);

    void setFriction(float nf);

    void setXPos(int nxp);

    void setYPos(int nyp);

};


#endif //actor_h
