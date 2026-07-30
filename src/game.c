#include "game.h"
#include "render.h"
#include "input.h"
#include "assets.h"

void game_loop(Game *game)
{
	Action action;
	update_input(&action);
	
	if (action.quit)
		game->is_running = false;
	
	render_frame(game->renderer, game->background);
}

bool game_create(Game *game)
{
	// Initialize game values
	game->state = PLAYING;
	game->is_running = true;
	game->window = NULL;
	game->renderer = NULL;
	
	// Initialize all SDL2 systems
	if (!init_sdl())
	{
		return false;
	}
	
	// Initialize all window and render related
	game->window = create_window();
	if (game->window == NULL)
	{
		game_destroy(game);
		return false;
	}

	game->renderer = create_renderer(game->window);
	if (game->renderer == NULL)
	{
		game_destroy(game);
		return false;
	}

	game->background = load_background(game->renderer);
	if (game->background == NULL)
	{
		game_destroy(game);
		return false;
	}
	
	return true;
}

void game_destroy(Game *game)
{
	// TODO: assets struct for managing textures and surfaces
	destroy_assets(game->background);
	destroy_renderer(game->renderer);
	destroy_window(game->window);
	deinit_sdl();
}
