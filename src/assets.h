#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
	SDL_Texture *background;
	SDL_Texture *ground;
	SDL_Texture *bird[BIRD_ANIM_FRAMES_COUNT];
	SDL_Texture *pipe;
} Assets;

bool load_assets(SDL_Renderer *renderer, Assets *assets);
void destroy_assets(Assets *assets);
SDL_Texture *load_texture(SDL_Renderer *renderer, char *path);

#endif
