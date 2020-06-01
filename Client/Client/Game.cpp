#ifdef _WIN32
#include "SDL.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#endif

#include "Game.h"

#include "TextureLoader.h"

SDL_Renderer* Game::renderer = nullptr;

Game::Game(const char* title, int width, int height) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	running = true;

	for (int i = 0; i < CLIENTS_COUNT; i++) {
		players[i] = new GameObject("assets/ghost.png");
	}

	socket = new Socket();
}

Game::~Game() {
	for (int i = 0; i < CLIENTS_COUNT; i++) {
		delete players[i];
	}
	delete socket;
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::is_running() {
	return running;
}

void Game::update() {
	socket->receive_message(latest_key);
	InMessage* message = socket->get_message();
	for (int i = 0; i < CLIENTS_COUNT; i++) {
		int new_x = message->pos_x[i];
		int new_y = message->pos_y[i];
		int is_tagged = message->is_tagged[i];

		players[i]->update(new_x, new_y);
		if(is_tagged ==  0)
            players[i]->change_texture("assets/ghost.png");
		else
            players[i]->change_texture("assets/pacman.png");
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	for (int i = 0; i < CLIENTS_COUNT; i++) {
		players[i]->render();
	}
	
	SDL_RenderPresent(renderer);
}

void Game::events() {
	SDL_Event event;
	SDL_PollEvent(&event);
	
	switch (event.type) {
	case SDL_QUIT:
		running = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			latest_key = 'w';
			break;
		case SDLK_DOWN:
			latest_key = 's';
			break;
		case SDLK_LEFT:
			latest_key = 'a';
			break;
		case SDLK_RIGHT:
			latest_key = 'd';
			break;
		}
	}
	//SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}
