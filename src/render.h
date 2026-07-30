#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void render_frame(SDL_Renderer *renderer, SDL_Texture *background);
bool init_sdl();
void deinit_sdl();
SDL_Window *create_window();
void destroy_window(SDL_Window *window);
SDL_Renderer *create_renderer(SDL_Window *window);
void destroy_renderer(SDL_Renderer *renderer);

#endif
