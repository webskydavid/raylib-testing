#include "debug.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#include "../constants.h"
#include "asteroid.h"
#include "ship.h"

void InitDebug() {
    isDebug = true;
}

void ProjectilesDebug() {
    DrawText(TextFormat("Projectiles"), 30, 130, 14, WHITE);
    for (size_t i = 0; i < PROJECTILE_AMOUNT; i++) {
        bool visible = projectiles[i].visible;
        char visibleText[4];
        strcpy(visibleText, visible ? "Yes" : "No");
        int posY = 150 + (i * 20);
        DrawText(TextFormat("%s ", visibleText), 30, posY, 14, WHITE);
    }
}

void AsteroidsDebug() {
    DrawText(TextFormat("Asteroids"), 120, 130, 14, WHITE);
    for (size_t i = 0; i < asteroidAmount; i++) {
        bool isDead = asteroids[i].isDead;
        char visibleText[4];
        strcpy(visibleText, isDead ? "Yes" : "No");
        int posY = 150 + (i * 20);
        float x = asteroids[i].position.x;
        float y = asteroids[i].position.y;
        DrawText(TextFormat("A: %s ", visibleText), 120, posY, 14, WHITE);
        DrawText(TextFormat("%i", i), x, y, 14, WHITE);
        DrawText(TextFormat("%f x %f", x, y), x, y + 20, 14, WHITE);
    }
}

void DrawDebug() {
    if (isDebug) {
        DrawText(TextFormat("%f", ship.rotation), 30, 10, 12, YELLOW);
        DrawText(TextFormat("%f , %f", ship.position.x, ship.position.y), 30, 30, 12, YELLOW);
        DrawText(TextFormat("%f , %f", ship.velocity.x, ship.velocity.y), 30, 50, 12, WHITE);
        DrawText(TextFormat("%f , %f", ship.direction.x, ship.direction.y), 30, 70, 12, WHITE);
        DrawText(TextFormat("Timer: %f", shootRate), 30, 110, 14, WHITE);

        DrawLineEx(ship.position, Vector2Add(ship.velocity, ship.position), 1, RED);
        DrawCircleLines(ship.position.x, ship.position.y, SCALE / 2, GREEN);

        Vector2 dir = Vector2Scale(ship.direction, 100.0f);
        DrawLineEx(ship.position, Vector2Add(dir, ship.position), 2, GREEN);

        DrawText(TextFormat("Is colliding: %s", isColliding ? "true" : "false"), 30, 90, 12, LIGHTGRAY);

        DrawFPS(30, GetScreenHeight() - 30);

        Asteroid* a = asteroids;

        for (size_t i = 0; i < asteroidAmount; i++) {
            DrawCircleLines(a[i].position.x, a[i].position.y, a[i].size / 2, ORANGE);
        }

        ProjectilesDebug();

        AsteroidsDebug();
    }
}
