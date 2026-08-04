#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "assets.h"
#include "config.h"

bool load_assets(SDL_Renderer *renderer, Assets *assets)
{
	assets->background = NULL;
	assets->ground = NULL;
	assets->bird[0] = NULL;
	assets->bird[1] = NULL;
	assets->bird[2] = NULL;
	assets->pipe = NULL;
	
	assets->background = load_texture(renderer, "assets/backgrounds/background-day.png");
	if (assets->background == NULL)
		return false;

	assets->ground = load_texture(renderer, "assets/sprites/base.png");
	if (assets->ground == NULL)
		return false;

	assets->bird[0] = load_texture(renderer, "assets/sprites/bird0.png");
	if (assets->bird[0] == NULL)
		return false;

	assets->bird[1] = load_texture(renderer, "assets/sprites/bird1.png");
	if (assets->bird[1] == NULL)
		return false;

	assets->bird[2] = load_texture(renderer, "assets/sprites/bird2.png");
	if (assets->bird[2] == NULL)
		return false;

	assets->pipe = load_texture(renderer, "assets/sprites/pipe.png");
	if (assets->pipe == NULL)
		return false;

	return true;
}

SDL_Texture *load_texture(SDL_Renderer *renderer, char *path)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path);
	if (texture == NULL)
	{
		fprintf(stderr, "Error loading image \"%s\": %s\n", path, IMG_GetError());
		return NULL;
	}

	return texture;
}

void destroy_assets(Assets *assets)
{
	SDL_DestroyTexture(assets->background);
	SDL_DestroyTexture(assets->ground);
	SDL_DestroyTexture(assets->bird[0]);
	SDL_DestroyTexture(assets->bird[1]);
	SDL_DestroyTexture(assets->bird[2]);
	SDL_DestroyTexture(assets->pipe);
}
