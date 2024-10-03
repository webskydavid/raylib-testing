
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir
#include "state.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(400, 400, "Hello Raylib");
    SearchAndSetResourceDir("resources");

    Texture wabbit = LoadTexture("wabbit_alpha.png");

    initAppState("Jeff");
    updateScore(10);

    char str[8];

    // game loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextFormat("Score: %i", appState.score), 30, 70, 12, WHITE);
        DrawTexture(wabbit, 30, 30, WHITE);

        EndDrawing();
    }

    UnloadTexture(wabbit);
    CloseWindow();
    return 0;
}
