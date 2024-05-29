#ifndef player_h
#define player_h

#include "actor.h"
#include "graphics.h"
#include "bullet.h"
#include <string>
#include <vector>

class Player : public Actor {
private:
    enum weaponType{standard, rapid, mega};
    weaponType currentWeapon;
    float speed;
    float targetDir;
    int delayTimer;
    int delay;
    int hp;
    void drawWeapon(int mx, int my) const;
    bool alive;
public:
    // Constructors
    Player();
    Player(double x, double y);

    // Moves the player in the corresponding directions
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    // Draws but needs the mouse coords for the target line location
    void draw(int mx, int my) const;

    // Shoots
    void shoot(std::vector<Bullet> &bullets, int mx, int my);

    // Explodes and sets dead to true
    void die(std::vector<Bullet> &bullets);

    // Gets the angle from the player to (usually the mouse) x and y coords
    float getAngle(int mx, int my) const;

    // Handles collisions and actually moves the player the distance calculated from the move functions
    bool update(std::vector<Bullet> &bullets);

    // Cycles the current weapon and resets cooldowns
    void cycleWeapon();

    // Getters and setters
    int getHP();
    void setHP(int nhp);
    bool isAlive();
    std::string getWeapon();
};

#endif //player_h