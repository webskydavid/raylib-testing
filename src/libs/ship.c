#include "ship.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../constants.h"
#include "asteroid.h"

Projectile *projectiles;
Ship ship;

int shootRate = 0;

void DrawLines(Vector2 origin, Vector2 *points, int point_size, float scale, float rotation) {
    for (size_t i = 0; i < point_size; i++) {
        Vector2 rotated1 = Vector2Rotate(points[i], rotation);
        Vector2 scaled1 = Vector2Scale(rotated1, scale);
        Vector2 added1 = Vector2Add(scaled1, origin);

        Vector2 rotated2 = Vector2Rotate(points[(i + 1) % point_size], rotation);
        Vector2 scaled2 = Vector2Scale(rotated2, scale);
        Vector2 added2 = Vector2Add(scaled2, origin);
        DrawLineEx(added1, added2, 2.0f, GOLD);
    }
}
Vector2 Vector2Direction(Vector2 from, Vector2 to) {
    Vector2 direction = Vector2Subtract(to, from);
    direction = Vector2Normalize(direction);
    return direction;
}

Vector2 Vector2DirFromRotation(float rotation) {
    Vector2 direction = {cosf(rotation), sinf(rotation)};
    return direction;
}

void InitShip() {
    projectiles = MemAlloc(PROJECTILE_AMOUNT * sizeof(Projectile));
    ship = (Ship){
        .position = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
        .velocity = Vector2Init(0.0f, 0.0f),
        .direction = Vector2Init(0.0f, 0.0f),
        .rotation = 0.0f,
        .scale = 1.0f,
        .isDead = false,
    };

    for (size_t i = 0; i < PROJECTILE_AMOUNT; i++) {
        projectiles[0] = (Projectile){
            .position = ship.position,
            .velocity = Vector2Scale(ship.direction, 30.0f),
            .direction = ship.direction,
            .visible = false,
        };
    }
};

void ShootShip() {
    shootRate += 5;
    for (size_t i = 0; i < PROJECTILE_AMOUNT; i++) {
        if (shootRate % 15 == 0 && !projectiles[i].visible) {
            projectiles[i].position = ship.position;
            projectiles[i].direction = ship.direction;
            projectiles[i].velocity = Vector2Scale(ship.direction, 430.0f);
            projectiles[i].visible = true;

            // Will break adding new projectiles to the array when the shootRate is reached
            break;
        }
    }
}

void CheckCollisionWithAsteroids(Projectile projectile) {
    for (size_t j = 0; j < ASTEROID_AMOUNT; j++) {
        if (!asteroids[j].isDead) {
            if (CheckCollisionCircles(projectile.position, PROJECTILE_SIZE, asteroids[j].position, asteroids[j].size / 2)) {
                projectile.visible = false;
                asteroids[j].isDead = true;
            }
        }
    }
}

void UpdateProjectiles() {
    for (size_t i = 0; i < PROJECTILE_AMOUNT; i++) {
        if (projectiles[i].visible) {
            projectiles[i].direction = ship.direction;
            projectiles[i].position = Vector2Add(projectiles[i].position, Vector2Scale(projectiles[i].velocity, GetFrameTime()));

            CheckCollisionWithAsteroids(projectiles[i]);

            if (projectiles[i].position.x > GetScreenWidth() ||
                projectiles[i].position.y > GetScreenHeight() ||
                projectiles[i].position.x < 0 ||
                projectiles[i].position.y < 0) {
                projectiles[i].visible = false;
                shootRate = 0;
            }
        }
    }
}

void DrawProjectiles() {
    for (size_t i = 0; i < PROJECTILE_AMOUNT; i++) {
        if (projectiles[i].visible) {
            DrawCircleV(projectiles[i].position, PROJECTILE_SIZE, RED);
        }
    }
}

void UpdateShip() {
    if (ship.isDead) return;

    if (IsKeyDown(KEY_D)) {
        ship.rotation += ROTATION_SPEED * TAU * GetFrameTime();
    }

    if (IsKeyDown(KEY_A)) {
        ship.rotation -= ROTATION_SPEED * TAU * GetFrameTime();
    }

    float rotation = ship.rotation + TAU;
    ship.direction = Vector2DirFromRotation(rotation);

    if (IsKeyDown(KEY_W)) {
        ship.velocity = Vector2Add(ship.velocity, Vector2Scale(ship.direction, SHIP_SPEED * GetFrameTime()));
    }

    if (IsKeyDown(KEY_SPACE)) {
        ShootShip();
    }

    // This will scale the velocity by the drag factor
    // Every frame it takes the current vel and multiplies it by 0.98
    // This will make the ship slow down over time
    ship.velocity = Vector2Scale(ship.velocity, 0.995f);

    // Ship constant velocity
    ship.position = Vector2Add(ship.position, Vector2Scale(ship.velocity, GetFrameTime()));

    ship.position.x = fmodf(ship.position.x + GetScreenWidth(), GetScreenWidth());
    ship.position.y = fmodf(ship.position.y + GetScreenHeight(), GetScreenHeight());
}

void DrawShip(Vector2 origin, float scale, float rotation) {
    if (ship.isDead) return;

    const int point_amount = 5;
    Vector2 pts[point_amount] = {{-0.4, -0.5}, {0.0, 0.5}, {0.4, -0.5}, {0.3, -0.4}, {-0.3, -0.4}};
    DrawLines(origin, pts, point_amount, scale, rotation);
}
