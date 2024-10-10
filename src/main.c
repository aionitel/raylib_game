#include <stdio.h>
#include <assert.h>
#include <raylib.h>

int main() {
	const int HEIGHT = 720;
	const int WIDTH = 1280;
	InitWindow(WIDTH, HEIGHT, "Litterc");

	// Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 100.0f, 100.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

	// Load texture and model.
	int mat_count = 2;
	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");

	// for (int i = 0; i < 10; i++) { printf("%d\n", billy.materials[i]); }
	// printf("CHANGING MATERIALS\n");
	// billy.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	// for (int i = 0; i < 10; i++) { printf("%d\n", billy.materials[i]); }

	assert(IsModelReady(billy));

	Vector2 texture_pos = {0.0f, 0.0f};
	Vector3 position = {0.0f, 0.0f, 0.0f};
	Vector3 scale = {50.0f, 50.0f, 10.0f};

	// Load model animations.
	int anim_count = 0;
	unsigned int anim_index = 0;
	unsigned int current_frame = 0;
	ModelAnimation *animations = LoadModelAnimations("resources/billy.glb", &anim_count);

	DisableCursor();
	SetTargetFPS(60);

    // Main game loop.
    while (!WindowShouldClose()) {
        // Update camera.
        UpdateCamera(&camera, CAMERA_FREE);
        // Update model animation
        ModelAnimation anim = animations[anim_index];
        current_frame = (current_frame + 1)%anim.frameCount;
        UpdateModelAnimation(billy, anim, current_frame);

        BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(camera);
				DrawGrid(10, 1.0f);
				DrawModelEx(billy, position, (Vector3){0.0f, 0.0f, 0.0f}, 0.0f, scale, WHITE);
			EndMode3D();

        EndDrawing();
    }

    UnloadModel(billy);
    CloseWindow();
    return 0;
}
