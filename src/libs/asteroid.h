#ifndef ASTEROID_H
#define ASTEROID_H

#include <raylib.h>

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float size;
    int seed;
    bool isDead;
    int points_amount;
    Vector2 *points;
} Asteroid;

extern Asteroid *asteroids;
extern bool isColliding;

float RandRangeF(float min, float max);
void GenerateAsteroid(Vector2 origin, Vector2 *points, int point_amount, float size, int seed);

void InitAsteroids();

void UpdateAsteroid();

void DrawAsteroid(Asteroid asteroid);
void DrawAsteroids();

void ResetAsteroids();
#endif  // !ASTEROID_H
