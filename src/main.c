#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "libs/asteroid.h"
#include "libs/ship.h"

#define ROTATION_SPEED 1.0f

bool isDebug = true;
bool pause = false;

Vector2 ast = Vector2Init(0.0f, 0.0f);
Vector2 ast1 = Vector2Init(0.0f, 0.0f);

void Update() {
    if (IsKeyPressed(KEY_P)) {
        pause = !pause;
    }

    if (pause) {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDebug = !isDebug;
    }

    UpdateShip();
    UpdateAsteroid();
}

void Debug() {
    if (isDebug) {
        DrawText(TextFormat("%f", ship.rotation), 30, 10, 12, YELLOW);
        DrawText(TextFormat("%f , %f", ship.position.x, ship.position.y), 30, 30, 12, YELLOW);
        DrawText(TextFormat("%f , %f", ship.velocity.x, ship.velocity.y), 30, 50, 12, WHITE);
        DrawText(TextFormat("%f , %f", ship.direction.x, ship.direction.y), 30, 70, 12, WHITE);

        DrawLineEx(ship.position, Vector2Add(ship.velocity, ship.position), 1, RED);
        DrawCircleLines(ship.position.x, ship.position.y, SCALE / 2, GREEN);

        Vector2 dir = Vector2Scale(ship.direction, 100.0f);
        DrawLineEx(ship.position, Vector2Add(dir, ship.position), 2, GREEN);

        DrawText(TextFormat("Is colliding: %s", isColliding ? "true" : "false"), 30, 90, 12, LIGHTGRAY);

        DrawFPS(30, GetScreenHeight() - 30);
        // Asteroid a = *asteroids;
        // for (size_t i = 0; i < asteroidAmount; i++) {
        //     DrawCircleLines(a[i].position.x, a[i].position.y, a[i].size / 2, ORANGE);
        // }
    }
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    Debug();
    DrawShip(ship.position, SCALE, ship.rotation);
    DrawAsteroids();

    // for (size_t i = 0; i < asteroidAmount; i++) {
    //     DrawAsteroid(asteroids[i]);
    // }

    EndDrawing();
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(700, 700, "raylib");

    InitShip();
    InitAsteroid();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
