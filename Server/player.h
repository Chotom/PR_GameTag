#ifndef PR_GAMETAG_PLAYER_H
#define PR_GAMETAG_PLAYER_H

struct Player {
    char direction;
    int is_tagged;
    int pos_x;
    int pos_y;

};

// Constructor (without allocation)
void Player__init(struct Player* self, int player_id) {
    if(player_id == 0) {
        self->pos_x = 0;
        self->pos_y = 0;
    }
    else if(player_id == 1) {
        self->pos_x = MAP_SIZE_X - PLAYER_SIZE;
        self->pos_y = MAP_SIZE_Y - PLAYER_SIZE;
    }
    else {
        self->pos_x = (MAP_SIZE_X - PLAYER_SIZE)/2;
        self->pos_y = (MAP_SIZE_Y - PLAYER_SIZE)/2;
    }
}

// Change position of player inside map
void Player__AddPos(struct Player *self, int x, int y) {
    if (self->pos_x + x >= 0 && self->pos_x + x <= MAP_SIZE_X - PLAYER_SIZE) {
        self->pos_x += x;
    }
    if (self->pos_y + y >= 0 && self->pos_y + y <= MAP_SIZE_Y - PLAYER_SIZE) {
        self->pos_y += y;
    }
}

// Check collision between players
int Player__CheckCollision(struct Player *self, struct Player *tagged) {
    if (self == tagged) return 0;

    int tag_pos_x = (int)(tagged->pos_x + tagged->pos_x + PLAYER_SIZE) / 2;
    int tag_pos_y = (int)(tagged->pos_y + tagged->pos_y + PLAYER_SIZE) / 2;

    if ((tag_pos_x >= self->pos_x)
    && (tag_pos_x <= self->pos_x + PLAYER_SIZE)
    && (tag_pos_y >= self->pos_y)
    &&  (tag_pos_y <= self->pos_y + PLAYER_SIZE)) {
        printf("CATCH!\n");
        return 1;
    }
}


#endif
