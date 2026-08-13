#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void render_present(SDL_Renderer *renderer);
void render_clear(SDL_Renderer *renderer);
void render_background(SDL_Renderer *renderer, SDL_Texture *background);
void render_sprite(SDL_Renderer *renderer, SDL_Texture *sprite, SDL_Rect *sprite_rect);
void render_sprite_ex(SDL_Renderer *renderer, SDL_Texture *sprite, SDL_Rect *sprite_rect, const double angle, SDL_RendererFlip flip);
void update_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, SDL_Texture **out_texture, SDL_Rect *out_rect);
void render_overlay_background(SDL_Renderer *renderer, SDL_Color color, int alpha);
bool init_sdl();
void deinit_sdl();
SDL_Window *create_window();
void destroy_window(SDL_Window *window);
SDL_Renderer *create_renderer(SDL_Window *window);
void destroy_renderer(SDL_Renderer *renderer);
bool update_fps_counter(double deltaTime, int *out_fps);
void debug_draw_hitbox(SDL_Renderer *renderer, SDL_Rect *hitbox);

#endif
