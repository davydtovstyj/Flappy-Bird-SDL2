#include "game.h"
#include "render.h"
#include "assets.h"
#include "sprites.h"
#include "audio.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

void game_loop(Game *game)
{
	update_input(&game->action);
	
	if (game->action.quit)
		game->is_running = false;

	game->last_time = game->curr_time;
	game->curr_time = SDL_GetPerformanceCounter();

	game->deltaTime = (double)((game->curr_time - game->last_time) / (double)SDL_GetPerformanceFrequency()); // Delta Time in seconds

	render_clear(game->renderer);

	switch (game->state)
	{
		case PLAYING:
			if (game->action.restart)
				game_restart(game);
		
			if (game->action.jump)
			{
				bird_jump(&game->world.bird);
				play_sound(game->assets.sounds.bird_jump);
			}

			update_game(game);
			render_game(game);

			break;
			
		case GAME_OVER:
			if (game->action.restart)
			{
				game->state = PLAYING;
				game_restart(game);
			}
			
			break;
		
		default:
			break;
	}

	update_fps_counter(game->deltaTime);
	
	render_present(game->renderer);
}

void update_game(Game *game)
{
	update_grounds(&game->world.grounds[0], &game->world.grounds[1], game->deltaTime);
	update_bird(&game->world.bird, game->deltaTime);
	update_bird_anim(&game->world.bird, game->assets.textures.bird, game->deltaTime);

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		Pipe last_pipe = get_last_pipe(game->world.pipes);
		update_pipe(&game->world.pipes[i], &last_pipe, game->deltaTime);
	}

	if (has_collision(game))
	{
		play_sound(game->assets.sounds.bird_hit);
		game->state = GAME_OVER;
	}
}

void game_restart(Game *game)
{
	reset_bird(&game->world.bird);
	reset_ground(&game->world.grounds[0]);
	reset_ground(&game->world.grounds[1]);

	reset_pipe(&game->world.pipes[0], WINDOW_WIDTH + DISTANCE_BETWEEN_PIPES);

	for (int i = 1; i < MAX_PIPES_COUNT; i++)
	{
		reset_pipe(&game->world.pipes[i], game->world.pipes[i-1].x + game->world.pipes[i-1].rect.w + DISTANCE_BETWEEN_PIPES);
	}
}

void render_game(Game *game)
{
	render_background(game->renderer, game->assets.textures.background);

	render_sprite(game->renderer, game->world.bird.texture, &game->world.bird.rect, game->world.bird.angle, SDL_FLIP_NONE);

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		render_sprite(game->renderer, game->world.pipes[i].texture, &game->world.pipes[i].rect, 0, SDL_FLIP_NONE);
		
		SDL_Rect upper_pipe_rect = game->world.pipes[i].rect;
		upper_pipe_rect.y -= upper_pipe_rect.h + PIPE_GAP;
		
		render_sprite(game->renderer, game->world.pipes[i].texture, &upper_pipe_rect, 0, SDL_FLIP_VERTICAL);
	}

	render_sprite(game->renderer, game->world.grounds[0].texture, &game->world.grounds[0].rect, 0, SDL_FLIP_NONE);
	render_sprite(game->renderer, game->world.grounds[1].texture, &game->world.grounds[1].rect, 0, SDL_FLIP_NONE);
}

bool has_collision(Game *game)
{
	// Check for ground collision
	if((game->world.bird.rect.y + game->world.bird.rect.h) >= game->world.grounds[0].rect.y)
		return true;

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		SDL_Rect upper_pipe = game->world.pipes[i].rect;
		upper_pipe.y -= game->world.pipes[i].rect.h + PIPE_GAP;

		if (SDL_HasIntersection(&game->world.bird.rect, &game->world.pipes[i].rect))
				return true;

		if (SDL_HasIntersection(&game->world.bird.rect, &upper_pipe))
				return true;
	}

	return false;
}

bool game_create(Game *game)
{
	// Initialize game values
	game->state = PLAYING;
	game->is_running = true;
	game->action.quit = false;
	game->action.jump = false;
	game->window = NULL;
	game->renderer = NULL;
	game->deltaTime = 0;
	game->curr_time = SDL_GetPerformanceCounter();;
	game->last_time = game->curr_time;
	
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

	if (!init_audio())
	{
		game_destroy(game);
		return false;
	}
	
	if (!load_assets(game->renderer, &game->assets))
	{
		game_destroy(game);
		return false;
	}

	create_bird(&game->world.bird, game->assets.textures.bird[0]);
	
	create_ground(&game->world.grounds[0], game->assets.textures.ground);
	create_ground(&game->world.grounds[1], game->assets.textures.ground);

	srand((int)time(NULL));

	create_pipe(&game->world.pipes[0], game->assets.textures.pipe, WINDOW_WIDTH + DISTANCE_BETWEEN_PIPES);
	for (int i = 1; i < MAX_PIPES_COUNT; i++)
	{
		create_pipe(&game->world.pipes[i], game->assets.textures.pipe, game->world.pipes[i-1].rect.x + game->world.pipes[i-1].rect.w + DISTANCE_BETWEEN_PIPES);
	}
	
	return true;
}

void game_destroy(Game *game)
{
	deinit_audio();
	destroy_assets(&game->assets);
	destroy_renderer(game->renderer);
	destroy_window(game->window);
	deinit_sdl();
}
