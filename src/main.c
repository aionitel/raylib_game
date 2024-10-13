#include <stdio.h>
#include <assert.h>
#include <raylib.h>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

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
    const float scale = 50.0;
    Vector3 SCALE = {scale, scale, scale};

	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");
	printf("CHANGING MODEL MATERIALS. \n");
	billy.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	Model coffee = LoadModel("resources/coffee.glb");
	//coffee.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = coffee.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture;

	assert(IsModelReady(billy));
	assert(IsModelReady(coffee));

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
        if (current_frame >= anim.frameCount) {
            current_frame = start_frame;
        }
        UpdateModelAnimation(billy, anim, current_frame);

        BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(camera);
				DrawGrid(999, 10.0f);
				DrawPoint3D(
				    (Vector3){30.0f, 30.0f, 0.0f},
					RED
				);
				DrawModelEx(
				    billy,
					position,
					(Vector3){0.0f, 0.0f, 0.0f},
					0.0f,
					SCALE,
					WHITE
				);
				DrawModelEx(
				    coffee,
					(Vector3){20.0f, 0.0f, 0.0f,},
   					(Vector3){0.0f, 0.0f, 0.0f},
   					0.0f,
   					(Vector3){1.0f, 1.0f, 1.0f},
   					WHITE
				);
			EndMode3D();

        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadModel(billy);
    UnloadModel(coffee);
    CloseWindow();
    return 0;
}
