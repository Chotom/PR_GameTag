#ifdef _WIN32
#include "SDL.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#endif

#include "Game.h"
#include "GameObject.h"
#include "TextureLoader.h"

GameObject::GameObject(const char* texture_path) {
	texture = TextureLoader::load_texture(texture_path);
}

void GameObject::update() {
	x_pos++;
	y_pos++;
	
	src_rect.x = x_pos;
	src_rect.y = y_pos;
	
	src_rect.h = height;
	src_rect.w = width;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, texture, NULL, NULL);
}
