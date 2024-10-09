#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "libs/asteroid.h"
#include "libs/debug.h"
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

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawDebug();
    DrawShip(ship.position, SCALE, ship.rotation);
    DrawAsteroids();

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
