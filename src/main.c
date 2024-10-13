#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "libs/asteroid.h"
#include "libs/debug.h"
#include "libs/hud.h"
#include "libs/ship.h"

#define ROTATION_SPEED 1.0f

bool isDebug = false;
bool pause = false;

void ResetGame() {
    ResetAsteroids();
    ResetProjectiles();
    ResetShip();

    ship.score = 0;
    ship.lives = 3;
}

void ResetStage() {
    ResetShip();
}

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
    UpdateProjectiles();

    if (ship.isDead) {
        ship.deadTimer -= GetFrameTime();

        if (ship.deadTimer <= 0.0f) {
            if (ship.lives <= 0) {
                ResetGame();
            } else {
                ResetStage();
            }
        }
    }
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawDebug();

    DrawShip();

    DrawAsteroids();
    DrawProjectiles();

    DrawHud();

    EndDrawing();
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(700, 700, "raylib");

    InitShip();
    InitProjectiles();
    InitAsteroids();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
