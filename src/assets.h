#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

SDL_Texture *load_background(SDL_Renderer *renderer);
void destroy_assets(SDL_Texture *background);

#endif
