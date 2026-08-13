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
	update_input(&game->action, game->game_over_overlay.restart_text_rect, game->menu_overlay.start_text_rect);
	
	if (game->action.quit)
		game->is_running = false;

	game->last_time = game->curr_time;
	game->curr_time = SDL_GetPerformanceCounter();

	game->deltaTime = (double)((game->curr_time - game->last_time) / (double)SDL_GetPerformanceFrequency()); // Delta Time in seconds
	if (game->deltaTime > 0.05) 
		game->deltaTime = 0.05; // Min 20 fps

	render_clear(game->renderer);

	switch (game->state)
	{
		case PLAYING:
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
				break;
			}

			update_game_over(game);
			render_game_over(game);
			
			break;

		case MENU:
			if (game->action.start)
			{
				game->state = PLAYING;
				break;
			}
		
			update_menu(game);
			render_menu(game);

			break;
		
		default:
			break;
	}

	// Render FPS
	if (update_fps_counter(game->deltaTime, &game->fps) && DEBUG_ENABLED)
	{
		char fps_text[32];
		snprintf(fps_text, sizeof(fps_text), "FPS: %d", game->fps);

		SDL_Color gray = {125, 125, 125, 255};
		update_text_texture(game->renderer, game->assets.fonts.main_font, fps_text, gray, 20, WINDOW_HEIGHT - 20, &game->assets.text_cache.fps_texture, &game->assets.text_cache.fps_rect);
	}
	
	render_present(game->renderer);
}

void update_game(Game *game)
{
	update_grounds(&game->world.grounds[0], &game->world.grounds[1], game->deltaTime);
	update_bird(&game->world.bird, game->deltaTime);
	update_bird_hitbox(&game->world.bird);
	update_bird_anim(&game->world.bird, game->assets.textures.bird, game->deltaTime);

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		Pipe *last_pipe = get_last_pipe(game->world.pipes);
		update_pipe(&game->world.pipes[i], last_pipe, game->deltaTime);
	}

	if (has_collision(game))
	{
		play_sound(game->assets.sounds.bird_hit);
		game->state = GAME_OVER;
		bird_jump(&game->world.bird);
		update_game_over_overlay(game);
	}

	if (update_scoring(game))
	{
		char score_text[32];
		snprintf(score_text, sizeof(score_text), "%d", game->score);

		SDL_Color white = {255, 255, 255, 255};
		update_text_texture(game->renderer, game->assets.fonts.main_font, score_text, white, (WINDOW_WIDTH / 2) - (game->assets.text_cache.score_rect.w / 2), 
												150, &game->assets.text_cache.score_texture, &game->assets.text_cache.score_rect);
		game->assets.text_cache.score_rect.x = (WINDOW_WIDTH - game->assets.text_cache.score_rect.w) / 2;
	}
}

bool update_scoring(Game *game)
{
	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		if (!game->world.pipes[i].passed && game->world.bird.hitbox.x > game->world.pipes[i].rect.x + (game->world.pipes[i].rect.w / 2))
		{
			game->score++;
			game->world.pipes[i].passed = true;
			play_sound(game->assets.sounds.plus_score);
			
			return true;
		}
	}
	
	return false;
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

	game->score = 0;
	SDL_Color white = {255, 255, 255, 255};
	update_text_texture(game->renderer, game->assets.fonts.main_font, "0", white, 0, SCORE_TEXT_Y_OFFSET, &game->assets.text_cache.score_texture, &game->assets.text_cache.score_rect);
	game->assets.text_cache.score_rect.x = (WINDOW_WIDTH - game->assets.text_cache.score_rect.w) / 2;
}

void render_game(Game *game)
{
	render_background(game->renderer, game->assets.textures.background);

	render_sprite_ex(game->renderer, game->world.bird.texture, &game->world.bird.rect, game->world.bird.angle, SDL_FLIP_NONE);

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		render_sprite(game->renderer, game->world.pipes[i].texture, &game->world.pipes[i].rect);
		
		SDL_Rect upper_pipe_rect = game->world.pipes[i].rect;
		upper_pipe_rect.y -= upper_pipe_rect.h + PIPE_GAP;
		
		render_sprite_ex(game->renderer, game->world.pipes[i].texture, &upper_pipe_rect, 0, SDL_FLIP_VERTICAL);
	}

	render_sprite(game->renderer, game->world.grounds[0].texture, &game->world.grounds[0].rect);
	render_sprite(game->renderer, game->world.grounds[1].texture, &game->world.grounds[1].rect);

	render_sprite(game->renderer, game->assets.text_cache.score_texture, &game->assets.text_cache.score_rect);

	// Debug
	if (DEBUG_ENABLED)
	{
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		debug_draw_hitbox(game->renderer, &game->world.bird.hitbox);

		SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
		for (int i = 0; i < MAX_PIPES_COUNT; i++)
		{
			debug_draw_hitbox(game->renderer, &game->world.pipes[i].rect);
		}

		SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
		debug_draw_hitbox(game->renderer, &game->world.grounds[0].rect);
		debug_draw_hitbox(game->renderer, &game->world.grounds[1].rect);

		render_sprite(game->renderer, game->assets.text_cache.fps_texture, &game->assets.text_cache.fps_rect);
	}
}

void render_game_over(Game *game)
{
	render_background(game->renderer, game->assets.textures.background);

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		render_sprite(game->renderer, game->world.pipes[i].texture, &game->world.pipes[i].rect);
		
		SDL_Rect upper_pipe_rect = game->world.pipes[i].rect;
		upper_pipe_rect.y -= upper_pipe_rect.h + PIPE_GAP;
		
		render_sprite_ex(game->renderer, game->world.pipes[i].texture, &upper_pipe_rect, 0, SDL_FLIP_VERTICAL);
	}

	render_sprite(game->renderer, game->world.grounds[0].texture, &game->world.grounds[0].rect);
	render_sprite(game->renderer, game->world.grounds[1].texture, &game->world.grounds[1].rect);

	render_sprite_ex(game->renderer, game->world.bird.texture, &game->world.bird.rect, game->world.bird.angle, SDL_FLIP_NONE);

	render_overlay_background(game->renderer, game->game_over_overlay.background_color, (Uint8)game->game_over_overlay.overlay_alpha);

	render_sprite(game->renderer, game->game_over_overlay.restart_text_texture, &game->game_over_overlay.restart_text_rect);
	render_sprite(game->renderer, game->game_over_overlay.score_text_texture, &game->game_over_overlay.score_text_rect);
}

void update_game_over(Game *game)
{
	if (game->game_over_overlay.overlay_alpha < GAME_OVER_MAX_ALPHA)
	{
		game->game_over_overlay.overlay_alpha += (GAME_OVER_FADE_SPEED * game->deltaTime);

		if (game->game_over_overlay.overlay_alpha > GAME_OVER_MAX_ALPHA)
				game->game_over_overlay.overlay_alpha = GAME_OVER_MAX_ALPHA;
	}

	if (game->world.bird.rect.y < WINDOW_HEIGHT + 50)
		update_bird_hit_anim(&game->world.bird, game->deltaTime);
}

bool has_collision(Game *game)
{
	// Check for ground collision
	if((game->world.bird.hitbox.y + game->world.bird.hitbox.h) >= game->world.grounds[0].rect.y)
		return true;

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		SDL_Rect upper_pipe = game->world.pipes[i].rect;
		upper_pipe.y -= game->world.pipes[i].rect.h + PIPE_GAP;

		if (SDL_HasIntersection(&game->world.bird.hitbox, &game->world.pipes[i].rect))
				return true;

		if (SDL_HasIntersection(&game->world.bird.hitbox, &upper_pipe))
				return true;
	}

	return false;
}

bool game_create(Game *game)
{
	srand((int)time(NULL));

	init_game_values(game);
	
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

	if (!create_game_world(game))
	{
		game_destroy(game);
		return false;
	}

	create_game_over_overlay(game);
	create_menu(game);

	game_restart(game);
	
	return true;
}

void init_game_values(Game *game)
{
	game->state = MENU;
	game->is_running = true;
	game->action.quit = false;
	game->action.jump = false;
	game->window = NULL;
	game->renderer = NULL;
	game->deltaTime = 0;
	game->curr_time = SDL_GetPerformanceCounter();;
	game->last_time = game->curr_time;
	game->fps = 0;
	game->score = 0;
}

bool create_game_world(Game *game)
{
	if (!create_bird(&game->world.bird, game->assets.textures.bird[0]))
		return false;
	
	if (!create_ground(&game->world.grounds[0], game->assets.textures.ground))
		return false;
	if (!create_ground(&game->world.grounds[1], game->assets.textures.ground))
		return false;

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		if (!create_pipe(&game->world.pipes[i], game->assets.textures.pipe, WINDOW_WIDTH))
			return false;
	}

	return true;
}

void create_game_over_overlay(Game *game)
{
	game->game_over_overlay.restart_text_texture = NULL;
	game->game_over_overlay.score_text_texture = NULL;

	SDL_Color black = {0, 0, 0, 255};
	game->game_over_overlay.background_color = black;

	SDL_Color white = {255, 255, 255, 255};
	update_text_texture(game->renderer, game->assets.fonts.main_font, "Restart", white, 0, 0, 
											&game->game_over_overlay.restart_text_texture, &game->game_over_overlay.restart_text_rect);
											
	game->game_over_overlay.restart_text_rect.x = (WINDOW_WIDTH / 2) - (game->game_over_overlay.restart_text_rect.w / 2);
	game->game_over_overlay.restart_text_rect.y = (WINDOW_HEIGHT / 2) - (game->game_over_overlay.restart_text_rect.h / 2);
}

void update_game_over_overlay(Game *game)
{
	SDL_Color white = {255, 255, 255, 255};
	game->game_over_overlay.overlay_alpha = 0.0;

	char score_text[32];
	snprintf(score_text, sizeof(score_text), "Score: %d", game->score);
	
	update_text_texture(game->renderer, game->assets.fonts.main_font, score_text, white, 0, 500, 
											&game->game_over_overlay.score_text_texture, &game->game_over_overlay.score_text_rect);

	game->game_over_overlay.score_text_rect.x = (WINDOW_WIDTH / 2) - (game->game_over_overlay.score_text_rect.w / 2);
}

void destroy_game_over_overlay(Game *game)
{
	SDL_DestroyTexture(game->game_over_overlay.restart_text_texture);
	SDL_DestroyTexture(game->game_over_overlay.score_text_texture);
}

void update_menu(Game *game)
{
	update_grounds(&game->world.grounds[0], &game->world.grounds[1], game->deltaTime);
	update_bird_anim(&game->world.bird, game->assets.textures.bird, game->deltaTime);

	
}

void render_menu(Game *game)
{
	render_background(game->renderer, game->assets.textures.background);

	render_sprite(game->renderer, game->world.grounds[0].texture, &game->world.grounds[0].rect);
	render_sprite(game->renderer, game->world.grounds[1].texture, &game->world.grounds[1].rect);

	render_sprite(game->renderer, game->world.bird.texture, &game->world.bird.rect);

	render_overlay_background(game->renderer, game->menu_overlay.background_color, game->menu_overlay.background_color.a);
	
	render_sprite(game->renderer, game->menu_overlay.start_text_texture, &game->menu_overlay.start_text_rect);
	render_sprite(game->renderer, game->menu_overlay.title_text_texture, &game->menu_overlay.title_text_rect);
}

void create_menu(Game *game)
{
	game->menu_overlay.title_text_texture = NULL;
	game->menu_overlay.start_text_texture = NULL;

	SDL_Color black = {0, 0, 0, 210};
	game->menu_overlay.background_color = black;

	SDL_Color white = {255, 255, 255, 255};
	update_text_texture(game->renderer, game->assets.fonts.main_font, "Start", white, 0, 0, 
											&game->menu_overlay.start_text_texture, &game->menu_overlay.start_text_rect);
											
	game->menu_overlay.start_text_rect.x = (WINDOW_WIDTH / 2) - (game->menu_overlay.start_text_rect.w / 2);
	game->menu_overlay.start_text_rect.y = (WINDOW_HEIGHT / 2) - (game->menu_overlay.start_text_rect.h / 2);

	update_text_texture(game->renderer, game->assets.fonts.title_font, "Flappy Bird", white, 0, 0, 
											&game->menu_overlay.title_text_texture, &game->menu_overlay.title_text_rect);
											
	game->menu_overlay.title_text_rect.x = (WINDOW_WIDTH / 2) - (game->menu_overlay.title_text_rect.w / 2);
	game->menu_overlay.title_text_rect.y = 200;
}

void destroy_menu(Game *game)
{
	SDL_DestroyTexture(game->menu_overlay.title_text_texture);
	SDL_DestroyTexture(game->menu_overlay.start_text_texture);
}

void game_destroy(Game *game)
{
	destroy_menu(game);
	destroy_game_over_overlay(game);
	deinit_audio();
	destroy_assets(&game->assets);
	destroy_renderer(game->renderer);
	destroy_window(game->window);
	deinit_sdl();
}
