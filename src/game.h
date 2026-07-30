#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "assets.h"
#include "sprites.h"

typedef enum 
{
//	MENU,
	PLAYING,
	GAME_OVER
} State;

typedef struct 
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	Assets assets;

	Bird bird;
	Ground grounds[2];
	
	State state;
	bool is_running;
} Game;

bool game_create(Game *game);
void game_destroy(Game *game);
void game_loop(Game *game);
void update(Game *game);
bool is_colide(Game *game);

#endif
