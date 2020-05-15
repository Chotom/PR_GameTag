#ifdef _WIN32
#include "SDL.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#endif

#include "Game.h"

int main(int argc, char* argv[]) {
	Uint32 start_time, end_time, duration;

	Game* game = new Game("Berek", MAP_SIZE_X, MAP_SIZE_Y);

	// main game loop
	while(game->is_running()) {
		start_time = SDL_GetTicks();

		game->events();
		game->update();
		game->render();

		end_time = SDL_GetTicks();
		duration = end_time - start_time;
		if (duration < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - duration);
		}
	}

	delete game;

	return 0;
}
