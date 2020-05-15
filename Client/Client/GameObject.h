#pragma once
class GameObject {
	SDL_Texture* texture;
	SDL_Rect src_rect, dst_rect;

	int height = 32;
	int width = 32;
public:
	GameObject(const char* texture_path);

	void update(int x, int y);
	void render();
};

