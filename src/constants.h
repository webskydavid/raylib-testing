#ifndef CONSTANTS_H
#define CONSTANTS_H

#define Vector2Init(a, b) \
    (Vector2) { a, b }

// Main
static const float TAU = PI / 2.0f;

// Ship
static const float SCALE = 25.0f;
static const float SHIP_SPEED = 300.0f;
static const float ROTATION_SPEED = 3.0f;

// Projectiles
static const int PROJECTILE_AMOUNT = 40;
static const float PROJECTILE_SIZE = 2.0f;

#endif
