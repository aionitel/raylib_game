#include <stdio.h>
#include "../include/raylib.h"

int main() {
    const int window_width = 1080;
    const int window_height = 720;

    InitWindow(window_width, window_height, "Game");
    SetTargetFPS(60);

    // Main game loop.
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
