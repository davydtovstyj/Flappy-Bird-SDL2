#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
	SDL_Texture *background;
	SDL_Texture *ground;
	SDL_Texture *bird[BIRD_ANIM_FRAMES_COUNT];
	SDL_Texture *pipe;
} Textures;

typedef struct {
	Mix_Chunk *bird_jump;
	Mix_Chunk *bird_hit;
	Mix_Chunk *plus_score;
} Sounds;

typedef struct {
	TTF_Font *main_font;
	TTF_Font *title_font;
} Fonts;

typedef struct
{
	SDL_Texture *fps_texture;
  SDL_Rect fps_rect;
  SDL_Texture *score_texture;
  SDL_Rect score_rect;
} TextCache;

typedef struct {
	Textures textures;
	Sounds sounds;
	Fonts fonts;
	TextCache text_cache;
} Assets;

bool load_assets(SDL_Renderer *renderer, Assets *assets);
void destroy_assets(Assets *assets);
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path);
TTF_Font *load_font(const char *path, int ptsize);

#endif
