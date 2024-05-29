#ifndef enemy_h
#define enemy_h

#include "actor.h"
#include "graphics.h"
#include "bullet.h"
#include <string>
#include <vector>
#include <random>

class Enemy : public Actor {
private:
    float speed;
    float targetDir;
    int delayTimer;
    int delay;
    int hp;
    bool alive;
    float moveDir;
    int moveTimer;
    int pauseTimer;
public:

    // Constructors
    Enemy();
    Enemy(double x, double y);

    // Moves erratically towards the player
    void move();

    // Shoots at the player
    void shoot(std::vector<Bullet> &bullets);

    // Explodes into harmless bullets and sets dead to true
    void die(std::vector<Bullet> &bullets);

    // Calls move, decides when to shoot, and responds to being shot
    bool update(std::vector<Bullet> &bullets, Actor &player);

    // getters and setters
    int getHP();
    void setHP(int nhp);
    bool isAlive();

    // Sets targetDir to the angle pointing to the other actor
    void target(Actor &other);
};

#endif //enemy_h