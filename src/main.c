#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <raylib.h>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif
#define MAX_ENTITIES 1000

typedef struct {
    bool is_running;
    int entity_count;
    int entities[MAX_ENTITIES];
    Model models[MAX_ENTITIES];
    ModelAnimation *animations[MAX_ENTITIES];
    Vector3 positions[MAX_ENTITIES];
    float velocities[MAX_ENTITIES]; // Only y velocities for now. (Applying gravity.)
    Camera camera;
} State;

static inline void move_player(Vector3 *position) {
    int key = GetKeyPressed();
    const float SPEED = 190.0;

    switch (key) {
        case KEY_W:
            position->z-- * SPEED;
            break;
       case KEY_S:
            position->z++ * SPEED;
            break;
        case KEY_A:
            position->x-- * SPEED;
            break;
        case KEY_D:
            position->x++ * SPEED;
            break;
        default:
            break;
    }
}

State state = { 0 };

void close_on_esc() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        state.is_running = false;
    }
}

static inline void print_animation(ModelAnimation *animation) {
    printf("ANIMATION NAME: %s\n", animation[0].name);
}

void init() {
    state.is_running = true;

   	const int HEIGHT = 720;
	const int WIDTH = 1280;
	InitWindow(WIDTH, HEIGHT, "ENTROPY");

	// Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 100.0f, 100.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

	DisableCursor();
	SetTargetFPS(60);

    // INIT player manually.
    // Load player model and texture.
    Vector3 player_position = {0.0f, 0.0f, 0.0f};
   	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");
	printf("CHANGING MODEL MATERIALS. \n");
	billy.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	// Load player model animations.
	// unsigned int anim_index = 5;
	// unsigned int start_frame = 0;
	// unsigned int current_frame = 0;
	int anim_count = 0;
	ModelAnimation *animations = LoadModelAnimations("resources/billy.glb", &anim_count);

	// Set entity system index. (Player will always be 0)
	state.entities[0] = 0;
	state.entity_count += 1;
	state.models[0] = billy;
	state.animations[0] = animations;
	state.positions[0] = player_position;
	state.velocities[0] = 0.0f;
	state.camera = camera;
}

unsigned int current_frame = 0;

void update_player_animation(ModelAnimation *animations) {
   	// Load model animations.
    int player_animation_index = 0;
    int animation_index = 5;
	int anim_count = 0;
	unsigned int start_frame = 0;
	Model model = state.models[player_animation_index];
	ModelAnimation anim = animations[animation_index];
	printf("Animation name: %s\n", anim.name);

    // Update model animation
    current_frame = (current_frame + 1) % anim.frameCount;
    if (current_frame >= anim.frameCount) {
        current_frame = start_frame;
    }

    UpdateModelAnimation(model, anim, current_frame);
}

void update() {
    close_on_esc();

    update_player_animation(state.animations[0]);
    //UpdateCamera(&state.camera, CAMERA_FREE);

    move_player(&state.positions[0]);
}

void draw() {
    DrawGrid(999, 10.0f);

    // Draw player (index = 0) model.
    DrawModel(
        state.models[0],
        state.positions[0],
        50.0f,
        WHITE
    );
}

int main() {
	init();

    // Main game loop.
    while (state.is_running) {
        update();

        BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(state.camera);
				draw();
			EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
