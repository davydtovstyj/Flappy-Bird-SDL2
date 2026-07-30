#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *load_background(SDL_Renderer *renderer)
{
	SDL_Texture *background = NULL;

	background = IMG_LoadTexture(renderer, "assets/backgrounds/background-day.png");
	if(background == NULL)
	{
		fprintf(stderr, "Error loading background: %s\n", IMG_GetError());
		return NULL;
	}

	return background;
}

void destroy_assets(SDL_Texture *background)
{
	SDL_DestroyTexture(background);
}
