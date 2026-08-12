#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct 
{
	bool jump;
	bool quit;
	bool restart;
} Action;

void update_input(Action *action, SDL_Rect restart_button_rect);

#endif
