#pragma once
class GameObject {
	SDL_Texture* texture;
	SDL_Rect src_rect, dst_rect;

	int x_pos = 50;
	int y_pos = 50;
	int height = 32;
	int width = 32;
public:
	GameObject(const char* texture_path);

	void update();
	void render();
};

