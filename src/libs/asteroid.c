#include "asteroid.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "ship.h"

float sizes[3] = {10.0f, 25.0f, 40.0f};

bool isColliding = false;
Asteroid *asteroids;

float RandRangeF(float min, float max) {
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

void GenerateAsteroid(Vector2 origin, Vector2 *points, int point_amount, float size, int seed) {
    srand(seed);
    float step = (PI * 2.0f) / (float)point_amount;
    for (size_t i = 0; i < point_amount; i++) {
        float radius = RandRangeF(0.3, 1.0);
        float angle = step * (float)i;
        Vector2 scaled = Vector2Scale(Vector2DirFromRotation(angle), radius);

        points[i] = Vector2Scale(scaled, size);
    }
}

void InitAsteroid() {
    asteroids = MemAlloc(asteroidAmount * sizeof(Asteroid));
    for (size_t i = 0; i < asteroidAmount; i++) {
        float rot = rand() % 360 / DEG2RAD;
        int point_amount = 5 + rand() % 16;
        Vector2 rand_pos = Vector2Init(rand() % GetScreenWidth(), rand() % GetScreenHeight());
        Vector2 dir = Vector2DirFromRotation(rot);
        Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));

        asteroids[i] = (Asteroid){
            .position = rand_pos,
            .velocity = Vector2Scale(dir, (rand() % 60)),
            .rotation = rot,
            .size = sizes[rand() % 3],
            .seed = rand() % 1000000,
            .points_amount = point_amount,
        };

        GenerateAsteroid(asteroids[i].position, points, point_amount, asteroids[i].size, asteroids[i].seed);

        asteroids[i].points = points;
    }
}

void UpdateAsteroid() {
    for (size_t i = 0; i < asteroidAmount; i++) {
        Vector2 newPos = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
        asteroids[i].position.x = fmodf(asteroids[i].position.x + GetScreenWidth(), GetScreenWidth());
        asteroids[i].position.y = fmodf(asteroids[i].position.y + GetScreenHeight(), GetScreenHeight());

        asteroids[i].position = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
        isColliding = CheckCollisionCircles(ship.position, SCALE / 2, asteroids[i].position, asteroids[i].size / 2);
        if (isColliding) {
            ship.isDead = true;
        }
    }
}
void DrawAsteroid(Asteroid asteroid) {
    for (size_t i = 0; i < asteroid.points_amount; i++) {
        Vector2 pos1 = Vector2Add(asteroid.position, asteroid.points[i]);
        Vector2 pos2 = Vector2Add(asteroid.position, asteroid.points[(i + 1) % asteroid.points_amount]);
        DrawLineEx(pos1, pos2, 2.0f, WHITE);
    }
}

void DrawAsteroids() {
    for (size_t i = 0; i < asteroidAmount; i++) {
        DrawAsteroid(asteroids[i]);
    }
}