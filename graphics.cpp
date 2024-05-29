#include "graphics.h"
#include "actor.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include <iostream>
#include <vector>
#include <random>
using namespace std;

GLdouble width, height;
int wd;

enum state{start, play, winRound, loose, winGame};

state gameState = start;

// Declaring global variables and vectors
int mouseX;
int mouseY;
Player p;
int level;
vector<Bullet> bullets;
vector<Enemy> foes;
bool keyPressed = false;
bool movingLeft, movingRight, movingUp, movingDown, shooting = false;

void initGame() {
    // Respawns the player if they are dead
    if (!p.isAlive() || gameState == winGame){
        p = Player();
    }

    // Heals the player for 5
    p.setHP(p.getHP()+5);
    if (p.getHP() > 100){
        p.setHP(100);
    }

    // Stops all movement in the player
    shooting = movingDown = movingUp = movingRight = movingLeft = false;
    p.setXPos(width/2);
    p.setYPos(height/2);
    p.setXVelo(0);
    p.setYVelo(0);

    // Clears the bullet and foes vectors
    foes.clear();
    bullets.clear();

    // Creates enemies in random locations. One more enemy for each level
    random_device rd;
    for (int i = 0; i < level; i++) {
        foes.push_back(Enemy(30+rd()%(1500-60), 30+rd()%(1500-60)));
    }

    // Sets the game state to start
    gameState = start;
}

// Initializes the dimensions and game for the first time
void init() {
    width = 1500;
    height = 1500;
    level = 1;
    initGame();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Black and opaque
}

// Draws text in location x y
void drawText(string text, int x, int y) {
    glColor3f(0, 0, 0);
    string label = text;
    glRasterPos2i(x, y);
    for (const char &letter : label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
    }
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);
    
    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Draws the Player
    if (p.isAlive()) {
        p.draw(mouseX, mouseY);
    }

    // Draws the Bullets
    for (Bullet &b : bullets){
        b.draw();
    }

    // Draws the Enemies
    for (Enemy &e:foes){
        if (e.isAlive()) {
            e.draw();
        }
    }

    // Draws the Player's Stats
    drawText("Level: " + to_string(level), 5, 20);
    drawText("HP: " + to_string(p.getHP()), 5, 40);
    drawText("Weapon Type: " + p.getWeapon(), 5, 60);

    // Draws the win text for a level
    if (gameState == winRound){
        drawText("Level " + to_string(level) + " Complete", width/2-55, height/2 + 60);
    }

    // Draws the loose text
    if (gameState == loose){
        drawText("You are Dead", width/2-55, height/2 + 60);
    }

    // Draws the win text for the whole game
    if (gameState == winGame){
        drawText("You Win", width/2-55, height/2+60);
    }

    // Tells the player how to restart/quit when the game has ended
    if (gameState == loose || gameState == winGame){
        drawText("Press Space to Start Over", width/2-110, height/2+80);
        drawText("Press Escape to Quit", width/2-95, height/2+100);
    }

    // Tells the player how to continue
    if (gameState == winRound){
        drawText("Press Space to Continue", width/2-90, height/2+80);
    }

    // Tells the current level and how to start it
    if (gameState == start){
        drawText("Level " + to_string(level), width/2-30, height/2+60);
        drawText("Press Space to Begin", width/2-90, height/2+80);
    }

    glFlush();  // Render now
}


void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    // Handles movement
    if (gameState == play) {
        if (key == 'a') {
            movingLeft = true;
        }
        if (key == 'd') {
            movingRight = true;
        }
        if (key == 'w') {
            movingUp = true;
        }
        if (key == 's') {
            movingDown = true;
        }
    }

    // Space advances menus when the game isn't playing and changes weapons when it is
    if (key == ' ') {
        if (gameState == play) {
            p.cycleWeapon();
        }
        if (gameState == start){
            gameState = play;
        }
        if (gameState == winRound){
            level++;
            initGame();
        }
        if (gameState == loose || gameState == winGame){
            level = 1;
            initGame();
        }
    }

    glutPostRedisplay();
}

void kbdup(unsigned char key, int x, int y) {
    // Stops movement when keys are released
    if (gameState == play) {
        if (key == 'a') {
            movingLeft = false;
        }
        if (key == 'd') {
            movingRight = false;
        }
        if (key == 'w') {
            movingUp = false;
        }
        if (key == 's') {
            movingDown = false;
        }
    }

    glutPostRedisplay();
}


void cursor(int x, int y) {
    // Updates the global mouseX and mouseY variables to be the same as the coordinates of the mouse
    mouseX = x;
    mouseY = y;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    // Holding down left click starts shooting letting go stops
    if (gameState == play) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            shooting = true;
        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            shooting = false;
        }
    }
    glutPostRedisplay();
}

void timer(int dummy) {
    if (gameState == play) {
        // Moves the player
        if (p.isAlive()) {
            if (movingLeft) {
                p.moveLeft();
            }
            if (movingRight) {
                p.moveRight();
            }
            if (movingUp) {
                p.moveUp();
            }
            if (movingDown) {
                p.moveDown();
            }
            // Shoots
            if (shooting) {
                p.shoot(bullets, mouseX, mouseY);
            }
            p.update(bullets);
        }
        else {
            // sets the game to loose if the player is dead
            gameState = loose;
        }

        // Updates the enemies
        int eLoc = 0;
        for (Enemy &e : foes) {
            if (e.isAlive()) {
                e.update(bullets, p);
            } else{
                foes.erase(foes.begin() + eLoc);
            }
            eLoc ++;
        }

        // Win the round if there are no enemies left
        if (foes.empty()){
            gameState = winRound;
            if (level >= 10)
                gameState = winGame;
        }
    }

    // Updates the bullets
    int bLoc = 0;
    for (Bullet &b : bullets){
        b.update();
        // Clears all enemy bullets when the game is not playing
        if (gameState != play && !b.isFriendly()){
            bullets.erase(bullets.begin() + bLoc);
        }

        // Clears all bullets that are far out of bounds
        if (b.getXPos() + b.getRadius() > width*1.5){
            bullets.erase(bullets.begin() + bLoc);
        }
        if (b.getYPos() + b.getRadius() > height*1.5){
            bullets.erase(bullets.begin() + bLoc);
        }
        if (b.getXPos() - b.getRadius() < width - width*1.5){
            bullets.erase(bullets.begin() + bLoc);
        }
        if (b.getYPos() - b.getRadius() < height - height*1.5){
            bullets.erase(bullets.begin() + bLoc);
        }
        bLoc++;
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Shoot Game" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press/release event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdup);
    
    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);
    
    // handles timer
    glutTimerFunc(0, timer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
