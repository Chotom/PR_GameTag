#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#include "TextureLoader.h"

#include "Game.h"

SDL_Texture* TextureLoader::load_texture(const char* path) {
	SDL_Surface* temp_surface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	return texture;
}
