#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <raylib.h>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_ENTITIES 1000     // Max number of entities allowed to spawn.s
#define GRAVITY -9.8f         // Gravity constant (m/s²)
#define BOUNCE 0.3f          // Bounciness coefficient
#define SPEED 10.0f

typedef struct {
    Vector3 min;
    Vector3 max;
} AABB;

typedef struct {
    bool is_running;
    int entity_count;
    float velocities[MAX_ENTITIES]; // Only y velocities for now. (Applying gravity.)
    float rotation[MAX_ENTITIES];
    AABB aabbs[MAX_ENTITIES];
    Model models[MAX_ENTITIES];
    ModelAnimation *animations[MAX_ENTITIES];
    int animation_indices[MAX_ENTITIES];
    Vector3 positions[MAX_ENTITIES];
    Vector3 scales[MAX_ENTITIES];
    Vector3 offset[MAX_ENTITIES];
    Camera camera;
} State;

State state = { 0 };

static void move_player(Vector3 *position, float *rotation, int *animation_index) {
    int key = GetKeyPressed();
    if (!key) *animation_index = 5;

    // Update rotation.
    if (IsKeyDown(KEY_L)) *rotation = 0.0f;
    if (IsKeyDown(KEY_J)) *rotation = 180.0f;
    if (IsKeyDown(KEY_I)) *rotation = 90.0f;
    if (IsKeyDown(KEY_K)) *rotation = -90.0f;

    // Update position.
    if (IsKeyDown(KEY_I)) {
        position->x += GetFrameTime() * SPEED;
        *animation_index = 0;
    }
    if (IsKeyDown(KEY_K)) {
        position->x -= GetFrameTime() * SPEED;
        *animation_index = 0;
    }
    if (IsKeyDown(KEY_J)) {
        position->z -= GetFrameTime() * SPEED;
        *animation_index = 0;
    }
    if (IsKeyDown(KEY_L)) {
        position->z += GetFrameTime() * SPEED;
        *animation_index = 0;
    }
}

static void close_on_esc() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        state.is_running = false;
    }
}

static void print_animation(ModelAnimation *animation) {
    printf("ANIMATION NAME: %s\n", animation[0].name);
}

static inline void init_entities() {
    // INIT player manually.
    // Load player model and texture.
    Vector3 player_position = {0.0f, 0.0f, 0.0f};
   	Model billy = LoadModel("resources/billy.glb");
	Texture2D texture = LoadTexture("resources/Billy_baseColor.png");
	printf("CHANGING MODEL MATERIALS. \n");
	billy.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	assert(IsModelReady(billy));

	// Create bounding box for model.
	AABB billy_aabb = {
	   (Vector3){-4.0f, 0.0f, -4.0f},
	   (Vector3){6.0f, 4.0f, 6.0f}
	};

	// Load player model animations.
	int anim_count = 0;
	ModelAnimation *animations = LoadModelAnimations("resources/billy.glb", &anim_count);

	// Set entity system index. (Player will always be 0)
	state.entity_count += 1;
	state.animation_indices[0] = 5;
	state.aabbs[0] = billy_aabb;
	state.models[0] = billy;
	state.animations[0] = animations;
	state.positions[0] = player_position;
	state.velocities[0] = 0.0;
	state.rotation[0] = -90.0f;
	state.scales[0] = (Vector3){5.0f, 5.0f, 5.0f};
	state.offset[0] = (Vector3){ 0 };

	// Load and add coffee entity.
	Vector3 coffee_position = {5.0f, 0.0f, 0.0f};
	Model coffee = LoadModel("resources/coffee.glb");
	assert(IsModelReady(coffee));

	state.entity_count += 1;
	state.models[1] = coffee;
	state.positions[1] = coffee_position;
	state.velocities[1] = 1.0;
	state.rotation[1] = 0.0;
	state.scales[1] = (Vector3){0.1f, 0.1f, 0.1f};
	state.offset[1] = (Vector3){0.0f, 0.4f, 0.0f};

	// Load and add McDonald's bag entity.
	Vector3 mc_position = {10.0f, 0.0f, 0.0f};
	Model mc = LoadModel("resources/mcdonalds_bag.glb");
	Texture2D mc_texture = LoadTexture("resources/mcdonalds_bag.png");
	printf("CHANGING MODEL MATERIALS. \n");
	mc.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = mc_texture;
	assert(IsModelReady(mc));

	state.entity_count += 1;
	state.models[2] = mc;
	state.positions[2] = mc_position;
	state.velocities[2] = 1.0;
	state.rotation[2] = 0.0;
	state.scales[2] = (Vector3){1.0f, 1.0f, 1.0f};
	state.offset[2] = (Vector3){0.0f, 1.0f, 0.0f};
}

static inline void init() {
    state.is_running = true;

   	const int HEIGHT = 720;
	const int WIDTH = 1280;
	InitWindow(WIDTH, HEIGHT, "ENTROPY");

	// Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ -10.0f, 10.0f, 0.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    state.camera = camera;

	DisableCursor();
	SetTargetFPS(60);

	init_entities();
}

unsigned int current_frame = 0;

void update_player_animation(ModelAnimation *animations) {
   	// Load model animations.
    int player_animation_index = 0;
	int anim_count = 0;
	unsigned int start_frame = 225;
	Model model = state.models[player_animation_index];
	ModelAnimation anim = animations[state.animation_indices[0]];

    // Update model animation.
    if (state.animation_indices[0] == 0) { // Running animation.
        current_frame = (current_frame + 1) % anim.frameCount;
        if (current_frame >= 275) {
            current_frame = start_frame;
        }
    } else {
        current_frame = (current_frame + 1) % anim.frameCount;
        if (current_frame >= anim.frameCount) {
            current_frame = start_frame;
        }
    }

    UpdateModelAnimation(model, anim, current_frame);
}

void update_physics(Vector3 *position, float *velocity) {
    if (IsKeyPressed(KEY_F)) {
        position->y = 10;
    }

    float dt = GetFrameTime();
    *velocity += GRAVITY * dt;
    position->y += *velocity * dt;

    // Keep entity below ground level.
    if (position->y < 0) {
        position->y = 0;
    }

    // Apply bounce to entity.
    if (position->y <= 0) {
        *velocity = -*velocity * BOUNCE;
    }
}

static void update() {
    close_on_esc();
    move_player(&state.positions[0], &state.rotation[0], &state.animation_indices[0]);

    update_physics(&state.positions[1], &state.velocities[1]);
    update_physics(&state.positions[2], &state.velocities[2]);
    update_player_animation(state.animations[0]);
    UpdateCamera(&state.camera, CAMERA_FREE);
}

// Custom function to add two Vector3 vectors
Vector3 AddVector3(Vector3 v1, Vector3 v2) {
    Vector3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

void print_aabb(AABB aabb) {
    // Print the min and max corners of the bounding box
    printf("Bounding Box MIN: (%.2f, %.2f, %.2f)\n", aabb.min.x, aabb.min.y, aabb.min.z);
    printf("Bounding Box MAX: (%.2f, %.2f, %.2f)\n", aabb.max.x, aabb.max.y, aabb.max.z);
}

void draw_bounding_box(Vector3 position, AABB aabb, float scale, Vector3 offset) {
    print_aabb(aabb);

    float length = (aabb.min.x + aabb.max.x);
    float height = (aabb.min.y + aabb.max.y);
    float width = (aabb.min.z + aabb.max.z);

    Vector3 new_pos = {
        position.x,
        position.y + 2.0f,
        position.z
    };

    DrawCubeWires(
        new_pos,
        width,
        height,
        length,
        RED
    );
}

static void draw() {
    DrawGrid(999, 1.0f);

    for (int i = 0; i < state.entity_count; i++) {
        DrawModelEx(
            state.models[i], // Model
            AddVector3(state.positions[i], state.offset[i]), // Position
            (Vector3){ 0.0f, 1.0f, 0.0f }, // Rotation axis
            state.rotation[i], // Rotation angle
            state.scales[i], // Scale
            WHITE
        );
    }

    for (int i = 0; i < state.entity_count; i++) {
        draw_bounding_box(state.positions[i], state.aabbs[i], state.scales[i].x, state.offset[i]);
    }

    DrawPoint3D(state.positions[0], BLUE);
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

    for (int i = 0; i < state.entity_count; i++) {
        UnloadModel(state.models[i]);
    }
    CloseWindow();
    return 0;
}
