#ifndef SHIP_H
#define SHIP_H

#include <raylib.h>

typedef struct Ship {
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;
    float rotation;
    float scale;
    bool isDead;
    float deadTimer;
    int lives;
    int score;
} Ship;

typedef struct Projectile {
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;
    bool visible;
} Projectile;

extern Projectile *projectiles;
extern Ship ship;
extern int shootRate;

Vector2 Vector2Direction(Vector2 from, Vector2 to);
Vector2 Vector2DirFromRotation(float rotation);

void InitShip();
void InitProjectiles();

void ShootShip();

void UpdateProjectiles();
void UpdateShip();

void DrawShip();
void DrawProjectiles();

void ResetShip();
void ResetProjectiles();

#endif
