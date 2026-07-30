#include "game.h"
#include "render.h"
#include "input.h"
#include "assets.h"
#include "sprites.h"
#include "config.h"

void game_loop(Game *game)
{
	Action action;
	update_input(&action);
	
	if (action.quit)
		game->is_running = false;
	if (action.jump)
		bird_jump(&game->bird);

	update(game);

	render_clear(game->renderer);
	
	render_background(game->renderer, game->assets.background);

	render_sprite(game->renderer, game->bird.texture, &game->bird.rect);
	render_sprite(game->renderer, game->grounds[0].texture, &game->grounds[0].rect);
	render_sprite(game->renderer, game->grounds[1].texture, &game->grounds[1].rect);
	
	render_present(game->renderer);
	
	SDL_Delay(16); // TODO: use delta time in update calculations
}

void update(Game *game)
{
	if (game->state != GAME_OVER)
	{
		update_grounds(&game->grounds[0], &game->grounds[1]);
		update_bird(&game->bird);

		if (is_colide(game))
		{
			game->state = GAME_OVER;
		}
	}
}

bool is_colide(Game *game)
{
	// Check for ground collision
	if((game->bird.rect.y + game->bird.rect.h) >= game->grounds[0].rect.y)
		return true;

	return false;
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
	
	if (!load_assets(game->renderer, &game->assets))
	{
		game_destroy(game);
		return false;
	}

	create_bird(&game->bird, game->assets.bird);
	
	create_ground(&game->grounds[0], game->assets.ground);
	create_ground(&game->grounds[1], game->assets.ground);
	
	return true;
}

void game_destroy(Game *game)
{
	destroy_assets(&game->assets);
	destroy_renderer(game->renderer);
	destroy_window(game->window);
	deinit_sdl();
}
