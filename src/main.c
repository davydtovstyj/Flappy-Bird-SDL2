#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(void)
{	
	Game game;
	
	if (!game_create(&game))
		return EXIT_FAILURE;
	
	while (game.is_running)
	{
		game_loop(&game);
	}
	
	game_destroy(&game);
	
	printf("Exited successfully!\n");
	return EXIT_SUCCESS;
}
