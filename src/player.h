typedef struct {
    Vector3 position;
} Player;

void init_player(Player *player, Vector3 *position);
void move_player(Vector3 *position);
