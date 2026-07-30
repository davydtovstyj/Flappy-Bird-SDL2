#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

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
	SDL_Texture *background;
	
	State state;
	bool is_running;
} Game;

bool game_create(Game *game);
void game_destroy(Game *game);
void game_loop(Game *game);

#endif
