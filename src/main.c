#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir
#include "state.h"

#define Vector2Init(a, b) \
    (Vector2) { a, b }

#define ROTATION_SPEED 1.0f

const float TAU = (PI * 2.0f);
const float SHIP_SPEED = 220.0f;

bool isDebug = true;
Vector2 shipPosition = {};
Vector2 shipVelocity = {};
Vector2 shipDirection = {};
float shipRotation = 0.0f;

Vector2 Vector2Direction(Vector2 from, Vector2 to) {
    Vector2 direction = Vector2Subtract(to, from);
    direction = Vector2Normalize(direction);
    return direction;
}

Vector2 Vector2DirFromRotation(float rotation) {
    Vector2 direction = {cosf(rotation), sinf(rotation)};
    return direction;
}

typedef enum {
    ASTEROID_SMALL,
    ASTEROID_MEDIUM,
    ASTEROID_LARGE
} AsteroidType;

float sizes[3] = {30.0f, 50.0f, 100.0f};

void DrawAsteroid(Vector2 position, AsteroidType size, int seed) {
    srand(seed);

    int point_amount = 4 + rand() % 12;
    Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));
    float step = TAU / (float)point_amount;

    for (size_t i = 0; i < point_amount; i++) {
        float radius = 30.0f + rand() % 50;
        float angle = step * (float)i;

        points[i] = Vector2Add(position, Vector2Scale(Vector2DirFromRotation(angle), radius));
    }

    for (size_t i = 0; i < point_amount; i++) {
        DrawLineEx(points[i], points[(i + 1) % point_amount], 1, WHITE);
    }
}

void DrawShip(Vector2 origin, float scale, float rotation) {
    Vector2 lines[5] = {{-0.4, -0.5}, {0.0, 0.5}, {0.4, -0.5}, {0.3, -0.4}, {-0.3, -0.4}};
    for (size_t i = 0; i < 5; i++) {
        Vector2 rotated1 = Vector2Rotate(lines[i], rotation);
        Vector2 scaled1 = Vector2Scale(rotated1, scale);
        Vector2 added1 = Vector2Add(scaled1, origin);

        Vector2 rotated2 = Vector2Rotate(lines[(i + 1) % 5], rotation);
        Vector2 scaled2 = Vector2Scale(rotated2, scale);
        Vector2 added2 = Vector2Add(scaled2, origin);

        DrawLineEx(added1, added2, 1, WHITE);
    }
}

void Update() {
    if (IsKeyPressed(KEY_F)) {
        isDebug = !isDebug;
    }

    if (IsKeyDown(KEY_D)) {
        shipRotation += ROTATION_SPEED * TAU * GetFrameTime();
    }
    if (IsKeyDown(KEY_A)) {
        shipRotation -= ROTATION_SPEED * TAU * GetFrameTime();
    }

    float rotation = shipRotation + PI / 2.0f;
    shipDirection = Vector2DirFromRotation(rotation);

    if (IsKeyDown(KEY_W)) {
        shipVelocity = Vector2Add(shipVelocity, Vector2Scale(shipDirection, SHIP_SPEED * GetFrameTime()));
    }

    // This will scale the velocity by the drag factor
    // Every frame it takes the current vel and multiplies it by 0.98
    // This will make the ship slow down over time
    shipVelocity = Vector2Scale(shipVelocity, 0.995f);

    // Ship constant velocity
    shipPosition = Vector2Add(shipPosition, Vector2Scale(shipVelocity, GetFrameTime()));

    shipPosition.x = fmodf(shipPosition.x + GetScreenWidth(), GetScreenWidth());
    shipPosition.y = fmodf(shipPosition.y + GetScreenHeight(), GetScreenHeight());
}

void Debug() {
    if (isDebug) {
        DrawText(TextFormat("%f", shipRotation), 30, 10, 12, YELLOW);
        DrawText(TextFormat("%f , %f", shipPosition.x, shipPosition.y), 30, 30, 12, YELLOW);
        DrawText(TextFormat("%f , %f", shipVelocity.x, shipVelocity.y), 30, 50, 12, WHITE);
        DrawText(TextFormat("%f , %f", shipDirection.x, shipDirection.y), 30, 70, 12, WHITE);

        DrawLineEx(shipPosition, Vector2Add(shipVelocity, shipPosition), 1, RED);

        Vector2 dir = Vector2Scale(shipDirection, 100.0f);
        DrawLineEx(shipPosition, Vector2Add(dir, shipPosition), 2, GREEN);
    }
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    Vector2 origin = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);

    DrawShip(shipPosition, 60.0f, shipRotation);
    DrawAsteroid(Vector2Init(300, 300), ASTEROID_MEDIUM, 2975);
    DrawAsteroid(Vector2Init(100, 200), ASTEROID_MEDIUM, 2175);

    Debug();
    EndDrawing();
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(700, 700, "Hello Raylib");

    shipPosition = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
