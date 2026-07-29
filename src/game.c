#include "game.h"
#include "render.h"

void game_loop(Game *game)
{
	// TODO: make module for input handling
	SDL_Event event;

	while(SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
				game->is_running = false;
				break;
			default:
				break;
		}
	}
	
	// Update and Present 
	// TODO: move this to render module
	SDL_RenderClear(game->renderer);
	SDL_RenderPresent(game->renderer);
}

bool game_create(Game *game)
{
	// Initialize game values
	game->state = MENU;
	game->is_running = true;
	game->window = NULL;
	game->renderer = NULL;
	
	// Initialize all SDL2 systems
	if (!init_sdl())
	{
		return false;
	}
	
	// Initialize all window and render related
	if (!create_window(game) || !create_renderer(game))
	{
		game_destroy(game);
		return false;
	}
	
	return true;
}

void game_destroy(Game *game)
{
	destroy_renderer(game);
	destroy_window(game);
	deinit_sdl();
}
