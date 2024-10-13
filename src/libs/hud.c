#include "hud.h"

#include "raylib.h"
#include "ship.h"

void UpdateHud() {
}

void DrawHud() {
    DrawText("LIVES: ", 10, 10, 20, WHITE);
    DrawText(TextFormat("%i", ship.lives), 90, 10, 20, WHITE);
    DrawText("SCORE: ", 10, 30, 20, WHITE);
    DrawText(TextFormat("%i", ship.lives), 90, 30, 20, WHITE);
}
