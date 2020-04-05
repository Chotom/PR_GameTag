#ifdef _WIN32
#include "SDL.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#endif

#include "Game.h"

#include "TextureLoader.h"

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {
}

bool Game::is_running() {
	return running;
}

void Game::init(const char* title, int width, int height) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	running = true;

	player = new GameObject("assets/ghost.png");
}

void Game::update() {
	//SDL_SetRenderDrawColor(renderer, r++, 255, 255, 255);
	//player->update();
}

void Game::render() {
	SDL_RenderClear(renderer);
	player->render();
	
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::events() {
	SDL_Event event;
	SDL_PollEvent(&event);
	
	switch (event.type) {
	case SDL_QUIT:
		running = false;
		break;
	default:
		break;
	}
}
