#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "state.h"

#define Vector2Init(a, b) \
    (Vector2) { a, b }

#define ROTATION_SPEED 1.0f

typedef enum {
    ASTEROID_SMALL = 3,
    ASTEROID_MEDIUM = 2,
    ASTEROID_LARGE = 1
} AsteroidType;

float sizes[3] = {25.0f, 40.0f, 50.0f};

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float size;
    int seed;
    bool isDead;
} Asteroid;

const float SCALE = 25.0f;
const float TAU = (PI * 2.0f);
const float SHIP_SPEED = 300.0f;

bool isDebug = true;
Vector2 shipPosition = {};
Vector2 shipVelocity = {};
Vector2 shipDirection = {};
float shipRotation = 0.0f;

const int asteroidAmount = 20;
Asteroid asteroids[asteroidAmount] = {};

Vector2 Vector2Direction(Vector2 from, Vector2 to) {
    Vector2 direction = Vector2Subtract(to, from);
    direction = Vector2Normalize(direction);
    return direction;
}

Vector2 Vector2DirFromRotation(float rotation) {
    Vector2 direction = {cosf(rotation), sinf(rotation)};
    return direction;
}

void DrawLines(Vector2 origin, Vector2 *points, int point_size, float scale, float rotation) {
    for (size_t i = 0; i < point_size; i++) {
        printf("%f, %f\n", points[i].x, points[i].y);
        Vector2 rotated1 = Vector2Rotate(points[i], rotation);
        Vector2 scaled1 = Vector2Scale(rotated1, scale);
        Vector2 added1 = Vector2Add(scaled1, origin);

        Vector2 rotated2 = Vector2Rotate(points[(i + 1) % 5], rotation);
        Vector2 scaled2 = Vector2Scale(rotated2, scale);
        Vector2 added2 = Vector2Add(scaled2, origin);
        DrawLineEx(added1, added2, 1, WHITE);
    }
}

void DrawAsteroid(Vector2 position, float size, int seed) {
    srand(seed);

    int point_amount = 5 + rand() % 16;
    Vector2 *points = MemAlloc(point_amount * sizeof(Vector2));
    float step = TAU / (float)point_amount;

    for (size_t i = 0; i < point_amount; i++) {
        float radius = 30.0f + rand() % 50;
        float angle = step * (float)i;

        points[i] = Vector2Add(position, Vector2Scale(Vector2DirFromRotation(angle), radius));
    }

    for (size_t i = 0; i < point_amount; i++) {
        Vector2 scaled1 = Vector2Scale(points[i], size);
        Vector2 added1 = Vector2Add(scaled1, position);
        Vector2 scaled2 = Vector2Scale(points[(i + 1) % point_amount], size);
        Vector2 added2 = Vector2Add(scaled2, position);
        DrawLineEx(added1, added2, 1, WHITE);
    }
}

void DrawShip(Vector2 origin, float scale, float rotation) {
    const int point_amount = 5;
    Vector2 pts[point_amount] = {{-0.4, -0.5}, {0.0, 0.5}, {0.4, -0.5}, {0.3, -0.4}, {-0.3, -0.4}};
    DrawLines(origin, pts, point_amount, scale, rotation);
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

    for (size_t i = 0; i < asteroidAmount; i++) {
        Vector2 newPos = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
        asteroids[i].position.x = fmodf(asteroids[i].position.x + GetScreenWidth(), GetScreenWidth());
        asteroids[i].position.y = fmodf(asteroids[i].position.y + GetScreenHeight(), GetScreenHeight());

        asteroids[i].position = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
    }
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

    DrawShip(shipPosition, SCALE, shipRotation);

    for (size_t i = 0; i < asteroidAmount; i++) {
        DrawAsteroid(asteroids[i].position, asteroids[i].size, asteroids[i].seed);
    }

    Debug();
    EndDrawing();
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(700, 700, "raylib");

    shipPosition = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);

    for (size_t i = 0; i < asteroidAmount; i++) {
        Vector2 rand_pos = Vector2Init(rand() % GetScreenWidth(), rand() % GetScreenHeight());
        float rot = rand() % 360 / DEG2RAD;
        Vector2 dir = Vector2DirFromRotation(rot);
        asteroids[i] = (Asteroid){
            .position = rand_pos,
            .velocity = Vector2Scale(dir, (rand() % 60)),
            .rotation = rot,
            .size = sizes[rand() % 3] / 100.0f,
            .seed = rand() % 1000000,
        };
    }

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
