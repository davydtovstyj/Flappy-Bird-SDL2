#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "assets.h"
#include "input.h"
#include "sprites.h"
#include "config.h"

typedef enum 
{
//	MENU,
	PLAYING,
	GAME_OVER
} State;

typedef struct
{
	Bird bird;
	Ground grounds[2];
	Pipe pipes[MAX_PIPES_COUNT];
} World;

typedef struct 
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	double deltaTime;
	Uint64 curr_time;
	Uint64 last_time;
	int fps;

	Action action;
	Assets assets;
	World world;
	
	State state;
	bool is_running;

	int score;
} Game;

bool game_create(Game *game);
void game_destroy(Game *game);
void game_loop(Game *game);
void game_restart(Game *game);
void update_game(Game *game);
void update_scoring(Game *game);
bool has_collision(Game *game);
void render_game(Game *game);

#endif
