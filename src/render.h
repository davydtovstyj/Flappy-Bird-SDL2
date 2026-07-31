#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void render_present(SDL_Renderer *renderer);
void render_clear(SDL_Renderer *renderer);
void render_background(SDL_Renderer *renderer, SDL_Texture *background);
void render_sprite(SDL_Renderer *renderer, SDL_Texture *sprite, SDL_Rect *sprite_rect, const double angle, SDL_RendererFlip flip);
bool init_sdl();
void deinit_sdl();
SDL_Window *create_window();
void destroy_window(SDL_Window *window);
SDL_Renderer *create_renderer(SDL_Window *window);
void destroy_renderer(SDL_Renderer *renderer);

#endif
