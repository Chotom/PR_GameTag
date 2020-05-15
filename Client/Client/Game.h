#pragma once
#include "GameObject.h"
#include "Socket.h"

class Game {
private:
	SDL_Window* window;
	GameObject* players[CLIENTS_COUNT];

	Socket* socket;
	char latest_key = '\0';

	int player_id = 0;

	bool running = false;
public:
	Game(const char* title, int width, int height);
	~Game();
	bool is_running();
	void update();
	void render();
	void events();

	static SDL_Renderer* renderer;
};
 
