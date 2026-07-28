#include "game.h"
#include <SDL2/SDL.h>
#include <stdio.h>

bool game_create(Game *game)
{
	if (!init_sdl(game))
	{
		deinit_sdl(game);
		return false;
	}
	
	return true;
}

void game_destroy(Game *game)
{
	deinit_sdl(game);
}

bool init_sdl(Game *game)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL: %s", SDL_GetError());
		return false;
	}
	
	return true;
}

void deinit_sdl(Game *game)
{
	SDL_Quit();
}
