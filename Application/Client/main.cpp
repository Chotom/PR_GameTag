#ifdef _WIN32
#include "SDL.h"
#endif

#ifdef linux
#include "SDL2/SDL.h"
#endif

#include "Game.h"

const int FPS = 60;
const int FRAME_TIME = 1000/FPS;

int main(int argc, char* argv[]) {
	Uint32 start_time, end_time, duration;

	Game* game = new Game();

	game->init("Berek", 600, 400);
	
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

	game->clean();
	
	return 0;
}
