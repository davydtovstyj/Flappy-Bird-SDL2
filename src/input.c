#include "input.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

void update_input(Action *action)
{
	action->jump = false;
	action->quit = false;
	
	SDL_Event event;

	while(SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
				action->quit = true;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
						action->quit = true;
						break;

					case SDL_SCANCODE_SPACE:
						action->jump = true;
						break;

					default:
						break;
				}
				break;
			
			default:
				break;
		}
	}
}
