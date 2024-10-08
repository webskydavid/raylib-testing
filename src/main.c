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

float sizes[3] = {10.0f, 25.0f, 40.0f};

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

const float SCALE = 25.0f;
const float TAU = (PI * 2.0f);
const float SHIP_SPEED = 300.0f;

bool isDebug = true;
Vector2 shipPosition = {};
Vector2 shipVelocity = {};
Vector2 shipDirection = {};
float shipRotation = 0.0f;
bool isColliding = false;
bool pause = false;
bool playerDead = false;

const int asteroidAmount = 4;
Asteroid asteroids[asteroidAmount] = {};

Vector2 ast = Vector2Init(0.0f, 0.0f);
Vector2 ast1 = Vector2Init(0.0f, 0.0f);

float RandRangeF(float min, float max) {
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
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

void DrawLines(Vector2 origin, Vector2 *points, int point_size, float scale, float rotation) {
    for (size_t i = 0; i < point_size; i++) {
        Vector2 rotated1 = Vector2Rotate(points[i], rotation);
        Vector2 scaled1 = Vector2Scale(rotated1, scale);
        Vector2 added1 = Vector2Add(scaled1, origin);

        Vector2 rotated2 = Vector2Rotate(points[(i + 1) % point_size], rotation);
        Vector2 scaled2 = Vector2Scale(rotated2, scale);
        Vector2 added2 = Vector2Add(scaled2, origin);
        DrawLineEx(added1, added2, 1, WHITE);
    }
}

void GenerateAsteroid(Vector2 origin, Vector2 *points, int point_amount, float size, int seed) {
    srand(seed);
    float step = TAU / (float)point_amount;
    for (size_t i = 0; i < point_amount; i++) {
        float radius = RandRangeF(0.3, 1.0);
        float angle = step * (float)i;
        Vector2 scaled = Vector2Scale(Vector2DirFromRotation(angle), radius);

        points[i] = Vector2Scale(scaled, size);
    }
}

void DrawAsteroid(Asteroid asteroid) {
    for (size_t i = 0; i < asteroid.points_amount; i++) {
        Vector2 pos1 = Vector2Add(asteroid.position, asteroid.points[i]);
        Vector2 pos2 = Vector2Add(asteroid.position, asteroid.points[(i + 1) % asteroid.points_amount]);
        DrawLineEx(pos1, pos2, 2.0f, WHITE);
    }
}

void DrawShip(Vector2 origin, float scale, float rotation) {
    const int point_amount = 5;
    Vector2 pts[point_amount] = {{-0.4, -0.5}, {0.0, 0.5}, {0.4, -0.5}, {0.3, -0.4}, {-0.3, -0.4}};
    DrawLines(origin, pts, point_amount, scale, rotation);
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
        // Vector2 newPos = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
        // asteroids[i].position.x = fmodf(asteroids[i].position.x + GetScreenWidth(), GetScreenWidth());
        // asteroids[i].position.y = fmodf(asteroids[i].position.y + GetScreenHeight(), GetScreenHeight());

        // asteroids[i].position = Vector2Add(asteroids[i].position, Vector2Scale(asteroids[i].velocity, GetFrameTime()));
        isColliding = CheckCollisionCircles(shipPosition, SCALE / 2, asteroids[i].position, asteroids[i].size / 2);
        if (isColliding) {
            playerDead = true;
        }
        printf("isColliding: %s\n", isColliding ? "true" : "false");
    }
}

void Debug() {
    if (isDebug) {
        DrawText(TextFormat("%f", shipRotation), 30, 10, 12, YELLOW);
        DrawText(TextFormat("%f , %f", shipPosition.x, shipPosition.y), 30, 30, 12, YELLOW);
        DrawText(TextFormat("%f , %f", shipVelocity.x, shipVelocity.y), 30, 50, 12, WHITE);
        DrawText(TextFormat("%f , %f", shipDirection.x, shipDirection.y), 30, 70, 12, WHITE);

        DrawLineEx(shipPosition, Vector2Add(shipVelocity, shipPosition), 1, RED);
        DrawCircleLines(shipPosition.x, shipPosition.y, SCALE / 2, GREEN);

        Vector2 dir = Vector2Scale(shipDirection, 100.0f);
        DrawLineEx(shipPosition, Vector2Add(dir, shipPosition), 2, GREEN);

        DrawText(TextFormat("Is colliding: %s", isColliding ? "true" : "false"), 30, 90, 12, LIGHTGRAY);

        DrawFPS(30, GetScreenHeight() - 30);
        for (size_t i = 0; i < asteroidAmount; i++) {
            DrawCircleLines(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].size / 2, ORANGE);
        }
    }
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    Debug();
    DrawShip(shipPosition, SCALE, shipRotation);

    for (size_t i = 0; i < asteroidAmount; i++) {
        DrawAsteroid(asteroids[i]);
    }

    EndDrawing();
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(700, 700, "raylib");

    shipPosition = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);

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

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}
