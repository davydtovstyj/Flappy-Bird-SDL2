#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct 
{
	bool jump;
	bool quit;
	bool restart;
	bool start;
} Action;

void update_input(Action *action, SDL_Rect restart_button_rect, SDL_Rect start_button_rect);

#endif
