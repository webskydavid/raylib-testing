#include "ship.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include "./../constants.h"

Ship ship;

const float SCALE = 25.0f;
const float SHIP_SPEED = 300.0f;
const float ROTATION_SPEED = 3.0f;
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
    ship = (Ship){
        .position = Vector2Init(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
        .velocity = Vector2Init(0.0f, 0.0f),
        .direction = Vector2Init(0.0f, 0.0f),
        .rotation = 0.0f,
        .scale = 1.0f,
        .isDead = false,
    };
};

void UpdateShip() {
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
    const int point_amount = 5;
    Vector2 pts[point_amount] = {{-0.4, -0.5}, {0.0, 0.5}, {0.4, -0.5}, {0.3, -0.4}, {-0.3, -0.4}};
    DrawLines(origin, pts, point_amount, scale, rotation);
}
