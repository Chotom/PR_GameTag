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

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.h = height;
	dst_rect.w = width;
}

void GameObject::update(int x, int y) {
	dst_rect.x = x;
	dst_rect.y = y;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, texture, NULL, &dst_rect);
}
