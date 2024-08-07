#include <stdio.h>
#include <assert.h>
#include "../include/raylib.h"

#define MAXHEIGHT 20;

int main() {
	const int window_width = 1080;
    const int window_height = 720;

    InitWindow(window_width, window_height, "Game");
    SetTargetFPS(60);

    // Load texture.
    Texture2D texture = LoadTexture("resources/walk.png");

    // Main game loop.
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
		DrawFPS(10, 10);

        // Draw texture to screen at position.
        DrawTexture(texture, window_width / 2 - texture.width / 2, window_height / 2 - texture.height / 2, WHITE);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
	assert(2);

    return 0;
}
