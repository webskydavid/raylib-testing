#ifndef ASTEROID_H
#define ASTEROID_H

#include <raylib.h>

#include "ship.h"

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float size;
    int seed;
    bool isSplitted;
    bool isDead;
    int points_amount;
    Vector2 *points;

} Asteroid;

extern Asteroid *asteroids;
extern bool isColliding;
extern int ASTEROID_AMOUNT;
extern int splitted_asteroid;

float RandRangeF(float min, float max);
void GenerateAsteroid(Vector2 origin, Vector2 *points, int point_amount, float size, int seed);

void InitAsteroids();

void UpdateAsteroid();

void DrawAsteroid(Asteroid asteroid);
void DrawAsteroids();

void ResetAsteroids();

void AsteroidCollidedWithProjectile(Projectile projectile);

void SplitAsteroid(int index, Vector2 projectilePos);
#endif  // !ASTEROID_H
