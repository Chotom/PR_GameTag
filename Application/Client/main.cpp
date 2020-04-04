#include "Game.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	Game *game = new Game();

	game->init("Berek", 600, 400);
	
	while(game->is_running()) {
		game->events();
		game->update();
		game->render();
	}

	game->clean();
	
	return 0;
}