#include "asteroid.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

float sizes[3] = {10.0f, 25.0f, 40.0f};

bool isColliding = false;
Asteroid *asteroids;
int ASTEROID_AMOUNT = 40;
int splitted_asteroid = 0;

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

void InitAsteroids() {
    int size = ASTEROID_AMOUNT / 2;
    asteroids = MemAlloc(ASTEROID_AMOUNT * sizeof(Asteroid));
    for (size_t i = 0; i < size; i++) {
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
            .isDead = false,
            .isSplitted = false,
            .points_amount = point_amount,
        };

        GenerateAsteroid(asteroids[i].position, points, point_amount, asteroids[i].size, asteroids[i].seed);

        asteroids[i].points = points;
    }

    for (size_t i = size + 1; i < size; i++) {
        int point_amount = 5 + rand() % 16;
        Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));
        asteroids[i] = (Asteroid){
            .position = Vector2Zero(),
            .velocity = Vector2Zero(),
            .rotation = 0,
            .size = 0,
            .seed = 0,
            .isDead = true,
            .isSplitted = true,
            .points_amount = point_amount,
            .points = NULL,
        };
        asteroids[i].points = points;
    }
}

void UpdateAsteroid() {
    for (size_t i = 0; i < ASTEROID_AMOUNT; i++) {
        if (!asteroids[i].isDead) {
            Vector2 newPos = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
            asteroids[i].position.x = fmodf(asteroids[i].position.x + GetScreenWidth(), GetScreenWidth());
            asteroids[i].position.y = fmodf(asteroids[i].position.y + GetScreenHeight(), GetScreenHeight());
            asteroids[i].position = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));

            // TODO: Should this check be here? Maybe it should be in the ship.c file
            if (!ship.isDead && CheckCollisionCircles(ship.position, SCALE / 2, asteroids[i].position, asteroids[i].size / 2)) {
                ship.isDead = true;
                ship.lives--;
                break;
            }
        }
    }
}

void DrawAsteroid(Asteroid asteroid) {
    for (size_t i = 0; i < asteroid.points_amount; i++) {
        if (asteroid.isDead) {
            continue;
        }
        Vector2 pos1 = Vector2Add(asteroid.position, asteroid.points[i]);
        Vector2 pos2 = Vector2Add(asteroid.position, asteroid.points[(i + 1) % asteroid.points_amount]);
        DrawLineEx(pos1, pos2, 2.0f, WHITE);
    }
}

void DrawAsteroids() {
    for (size_t i = 0; i < ASTEROID_AMOUNT; i++) {
        DrawAsteroid(asteroids[i]);
    }
}

void ResetAsteroids() {
    free(asteroids);
    InitAsteroids();
}

void AsteroidCollidedWithProjectile(Projectile projectile) {
    for (size_t j = 0; j < ASTEROID_AMOUNT; j++) {
        if (!asteroids[j].isDead) {
            if (CheckCollisionCircles(projectile.position, PROJECTILE_SIZE, asteroids[j].position, asteroids[j].size / 2)) {
                projectile.visible = false;
                SplitAsteroid(j, projectile.position);
            }
        }
    }
}

void SplitAsteroid(int index, Vector2 projectilePos) {
    Asteroid asteroid = asteroids[index];
    asteroids[index].isDead = true;
    printf("Split asteroid\n");

    if (asteroid.size == sizes[0]) {
        printf("Cannot split\n");
        return;
    } else if (asteroid.size == sizes[1]) {
        float rot = rand() % 360 / DEG2RAD;
        int point_amount = 5 + rand() % 16;
        Vector2 dir = Vector2DirFromRotation(rot);
        Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));

        if (splitted_asteroid == ASTEROID_AMOUNT / 2) {
            printf("Limit of splitted asteroids reached\n");
            return;
        }
        // int size = ASTEROID_AMOUNT - 1;  // 39
        // splitted_asteroid += 1;

        // asteroids[size - splitted_asteroid] = (Asteroid){
        //     .position = Vector2Add(projectilePos, Vector2Scale(dir, SCALE / 2)),
        //     .velocity = Vector2Scale(dir, (20 + rand() % 100)),
        //     .rotation = rot,
        //     .size = sizes[0],
        //     .seed = rand() % 323232,
        //     .points_amount = point_amount,
        //     .isDead = false,
        // };
        // GenerateAsteroid(
        //     asteroids[size - splitted_asteroid].position,
        //     points, point_amount,
        //     asteroids[size - splitted_asteroid].size,
        //     asteroids[size - splitted_asteroid].seed);

        // asteroids[size - splitted_asteroid].points = points;

    } else {
        float rot = rand() % 360 / DEG2RAD;
        int point_amount = 5 + rand() % 16;
        Vector2 dir = Vector2DirFromRotation(rot);
        Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));

        if (splitted_asteroid == ASTEROID_AMOUNT / 2) {
            printf("Limit of splitted asteroids reached\n");
            return;
        }

        // int size = ASTEROID_AMOUNT - 1;  // 39
        // splitted_asteroid += 1;

        // asteroids[size - splitted_asteroid] = (Asteroid){
        //     .position = Vector2Add(projectilePos, Vector2Scale(dir, SCALE / 2)),
        //     .velocity = Vector2Scale(dir, (20 + rand() % 100)),
        //     .rotation = rot,
        //     .size = sizes[1],
        //     .seed = rand() % 4444,
        //     .points_amount = point_amount,
        //     .isDead = false,
        // };
        // GenerateAsteroid(
        //     asteroids[size - splitted_asteroid].position,
        //     points, point_amount,
        //     asteroids[size - splitted_asteroid].size,
        //     asteroids[size - splitted_asteroid].seed);

        // asteroids[size - splitted_asteroid].points = points;
    }
}
