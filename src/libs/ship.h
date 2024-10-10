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

const int PROJECTILE_AMOUNT;
const float SCALE;
const float SHIP_SPEED;
const float ROTATION_SPEED;

Vector2 Vector2Direction(Vector2 from, Vector2 to);
Vector2 Vector2DirFromRotation(float rotation);

void InitShip();
void ShootShip();
void UpdateProjectiles();
void UpdateShip();
void DrawShip(Vector2 origin, float scale, float rotation);
void DrawProjectiles();

#endif
