#include "input.h"
#include <stdbool.h>

void update_input(Action *action, SDL_Rect restart_button_rect)
{
	action->jump = false;
	action->quit = false;
	action->restart = false;
	
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

					case SDL_SCANCODE_R:
						action->restart = true;
						break;

					default:
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (event.button.x > restart_button_rect.x && event.button.x < restart_button_rect.x + restart_button_rect.w &&
							event.button.y > restart_button_rect.y && event.button.y < restart_button_rect.y + restart_button_rect.h)
					{
						action->restart = true;
					}
				}
				break;
			
			default:
				break;
		}
	}
}
