#ifndef bullet_h
#define bullet_h

#include "actor.h"
#include "graphics.h"
#include <string>

class Bullet : public Actor {
private:
    double speed;
    double direction;
    bool friendly;
    int damage;
    double maxVelo = 50;
public:
    // constructors
    Bullet();
    Bullet(double x, double y, double angle, int size, int dmg, bool frnd);

    // getters
    double getDirection();
    bool isFriendly();
    int getDamage();
    double getSpeed();

    // setters
    void setDirection(double angle);
    void setSpeed(double spd);

    // Accelerates the bullet at the angle it's fired
    void update() override;

};

#endif // bullet_h