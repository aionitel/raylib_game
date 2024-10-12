#include <stdio.h>
#include <assert.h>
#include <raylib.h>

// Controller input.
#define XBOX_360_LEGACY_NAME_ID "Xbox 360 Controller"
#define XBOX_360_NAME_ID "Xbox 360 Controller"
#define PS3_NAME_ID "Sony Playstaion(R)3 Controller"

typedef struct {
    Vector3 position;
} Player;

void move_player(Vector3 *position) {
    if (IsKeyDown(KEY_S)) {
        position->x -= 20;
    }

    int key = GetKeyPressed();
    switch (key) {
        case KEY_W:
            position->y++;
            break;
        case KEY_S:
            position->y--;
            break;
        case KEY_D:
            position->x++;
            break;
        case KEY_A:
            position->x--;
            break;
        case KEY_F:
            position->x += 10;
            break;
        default:
            break;
    }
}

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

    // Player logic and model.
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Player player = {position};

	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");
	printf("CHANGING MODEL MATERIALS. \n");
	billy.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	Model cola = LoadModel("resources/cola_can.glb");

	assert(IsModelReady(billy));
	assert(IsModelReady(cola));

	// Load model animations.
	int anim_count = 0;
	unsigned int anim_index = 5;
	unsigned int start_frame = 0;
	unsigned int current_frame = 0;
	ModelAnimation *animations = LoadModelAnimations("resources/billy.glb", &anim_count);
	ModelAnimation anim = animations[anim_index];

	DisableCursor();
	SetTargetFPS(60);

    // Main game loop.
    while (!WindowShouldClose()) {
        // Update camera.
        UpdateCamera(&camera, CAMERA_FREE);

        move_player(&player.position);

        // Update model animation
        current_frame = (current_frame + 1) % anim.frameCount;
        if (current_frame >= 50) {
            //current_frame = start_frame;
        }
        UpdateModelAnimation(billy, anim, current_frame);

        BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(camera);
				DrawGrid(999, 10.0f);
				DrawModel(billy, position, 50.0f, WHITE);
			EndMode3D();

        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadModel(billy);
    UnloadModel(cola);
    CloseWindow();
    return 0;
}
