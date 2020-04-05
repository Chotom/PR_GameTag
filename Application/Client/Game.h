#pragma once

class Game {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool running = false;
	unsigned char r = 0;
public:
	Game();
	bool is_running();
	void init(const char* title, int width, int height);
	void update();
	void render();
	void clean();
	void events();
};

