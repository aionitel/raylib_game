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

void init_player(Player *player, Vector3 *position) {
    player->position = *position;
}

void move_player(Vector3 *position) {
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
	const float scale = 40.0;
	const Vector3 SCALE = {scale, scale, 0.0f};

	InitWindow(WIDTH, HEIGHT, "Litterc");

	// Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 100.0f, 100.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    // Player logic and model.
    Player player;
	init_player(&player, &(Vector3){0.0f, 0.0f, 0.0f});

	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");
	printf("CHANGING MODEL MATERIALS. \n");
	billy.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	Model coffee = LoadModel("resources/coffee_mug.glb");

	assert(IsModelReady(billy));
	assert(IsModelReady(coffee));

	// Load model animations.
	int anim_count = 0;
	unsigned int anim_index = 0;
	unsigned int start_frame = 35;
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
            current_frame = start_frame;
        }
        UpdateModelAnimation(billy, anim, current_frame);

        BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(camera);
				DrawGrid(999, 10.0f);
				DrawModelEx(billy, player.position, (Vector3){0.0f, 0.0f, 0.0f}, 0.0f, SCALE, WHITE);
				DrawModelEx(coffee, (Vector3){10.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, 0.0f}, 0.0f, SCALE, WHITE);
			EndMode3D();

        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadModel(billy);
    UnloadModel(coffee);
    CloseWindow();
    return 0;
}
