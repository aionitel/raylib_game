#include <raylib.h>
#include "player.h"

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
