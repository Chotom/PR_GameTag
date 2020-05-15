#pragma once
#include "../../consts/consts.h"

class GameObject {
	SDL_Texture* texture;
	SDL_Rect src_rect, dst_rect;

	int height = PLAYER_SIZE;
	int width = PLAYER_SIZE;
public:
	GameObject(const char* texture_path);

	void update(int x, int y);
	void render();
};

