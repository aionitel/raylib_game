#include <stdio.h>
#include <assert.h>
#include <raylib.h>

int main() {
	const int HEIGHT = 720;
	const int WIDTH = 1280;
	InitWindow(WIDTH, HEIGHT, "Litter?");

	// Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

	// Load texture and model.
	Texture2D texture = LoadTexture("resources/idle_south.png");
	Model model = LoadModel("resources/cola_can.glb");
	Vector3 position = {0.0f, 0.0f, 0.0f};

    // Main game loop.
    while (!WindowShouldClose()) {
		camera.position.y++;
        BeginDrawing();
			ClearBackground(WHITE);
			SetTargetFPS(75);
			DrawFPS(10, 10);

			DrawTexture(texture, 100, 100, WHITE);

			BeginMode3D(camera);
				DrawModel(model, position, 50.0f, WHITE);
			EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
