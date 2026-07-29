#include "render.h"
#include "config.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

bool init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	
	return true;
}

void deinit_sdl()
{
	SDL_Quit();
}

bool create_window(Game *game)
{
	game->window = SDL_CreateWindow(
		WINDOW_TITLE, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FLAGS
		);
	if (game->window == NULL)
	{
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return false;
	}
	
	return true;
}

void destroy_window(Game *game)
{
	SDL_DestroyWindow(game->window);
}

bool create_renderer(Game *game)
{
	game->renderer = SDL_CreateRenderer(game->window, -1, RENDER_FLAGS);
	if (game->renderer == NULL)
	{
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return false;
	}
	
	return true;
}

void destroy_renderer(Game *game)
{
	SDL_DestroyRenderer(game->renderer);
}
