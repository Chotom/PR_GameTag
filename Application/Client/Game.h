#pragma once
#include "GameObject.h"

class Game {
private:
	SDL_Window* window;
	GameObject* player;
	
	bool running = false;
public:
	Game();
	bool is_running();
	void init(const char* title, int width, int height);
	void update();
	void render();
	void clean();
	void events();

	static SDL_Renderer* renderer;
};
 
