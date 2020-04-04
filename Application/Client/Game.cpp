#include "Game.h"

#include <SDL.h>

Game::Game() {
	
}

bool Game::is_running() {
	return running;
}

void Game::init(const char* title, int width, int height) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	running = true;
}

void Game::update() {
	SDL_SetRenderDrawColor(renderer, r++, 255, 255, 255);
}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
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
