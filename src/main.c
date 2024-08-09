#include <stdio.h>
#include <assert.h>
#include "../include/raylib.h"

void draw_texture() {
	const int scale = 300;

	// Load image, edit it, then turn it into a texture to be rendered.
	Image image = LoadImage("resources/walk.png");
	assert(IsImageReady(image));
	ImageResize(&image,scale * 4, scale);
	Texture2D texture = LoadTextureFromImage(image);
	DrawTexture(texture, 50, 50, WHITE);
}

int main() {
	const int screen_width = 1080;
    const int screen_height = 720;
	float scale = 2.;

	// Character cutout is 32x32.
	Rectangle cutout = { 0.0f, 0.0f, 256.0f, 256.0f };
	Rectangle dest = { screen_width / 2.0f, screen_height / 2.0f, 32.0f, 32.0f };
	Vector2 position = { screen_width / 2, screen_height / 2 };

	// Init window,OpenGL context.
	InitWindow(screen_width, screen_height, "Game");
    SetTargetFPS(60);
	assert(IsWindowReady());

    // Load texture.
    Texture2D texture = LoadTexture("resources/walk.png");
	SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR); // Makes texture smoother when upscaling.

    // Main game loop.
    while (!WindowShouldClose()) {
        BeginDrawing();
			DrawFPS(10, 10);
			draw_texture();
			ClearBackground(WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
