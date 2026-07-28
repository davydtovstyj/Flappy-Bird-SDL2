#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(void)
{
	printf("Hello CMake!\n");
	
	Game game = {
		.state = MENU,
		.is_running = true
	};
	
	if (!game_create(&game))
		return EXIT_FAILURE;
	game_destroy(&game);
	
	return EXIT_SUCCESS;
}
