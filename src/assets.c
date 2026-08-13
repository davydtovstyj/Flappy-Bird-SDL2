#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "assets.h"
#include "audio.h"
#include "config.h"

bool load_assets(SDL_Renderer *renderer, Assets *assets)
{
	assets->textures.background = NULL;
	assets->textures.ground = NULL;
	assets->textures.bird[0] = NULL;
	assets->textures.bird[1] = NULL;
	assets->textures.bird[2] = NULL;
	assets->textures.pipe = NULL;
	
	assets->text_cache.fps_texture = NULL;
	assets->text_cache.score_texture = NULL;

	assets->sounds.bird_jump = NULL;
	assets->sounds.bird_hit = NULL;
	assets->sounds.plus_score = NULL;

	// Textures
	assets->textures.background = load_texture(renderer, "assets/backgrounds/background-day.png");
	if (assets->textures.background == NULL)
		return false;

	assets->textures.ground = load_texture(renderer, "assets/sprites/base.png");
	if (assets->textures.ground == NULL)
		return false;

	assets->textures.bird[0] = load_texture(renderer, "assets/sprites/bird0.png");
	if (assets->textures.bird[0] == NULL)
		return false;

	assets->textures.bird[1] = load_texture(renderer, "assets/sprites/bird1.png");
	if (assets->textures.bird[1] == NULL)
		return false;

	assets->textures.bird[2] = load_texture(renderer, "assets/sprites/bird2.png");
	if (assets->textures.bird[2] == NULL)
		return false;

	assets->textures.pipe = load_texture(renderer, "assets/sprites/pipe.png");
	if (assets->textures.pipe == NULL)
		return false;

	// Sounds
	assets->sounds.bird_jump = load_sound_wav("assets/sounds/wing.wav");
	if (assets->sounds.bird_jump == NULL)
		return false;

	assets->sounds.bird_hit = load_sound_wav("assets/sounds/hit.wav");
	if (assets->sounds.bird_hit == NULL)
		return false;

	assets->sounds.plus_score = load_sound_wav("assets/sounds/point.wav");
	if (assets->sounds.plus_score == NULL)
		return false;

	// Fonts
	assets->fonts.main_font = load_font("assets/fonts/flappy-font.ttf", MAIN_TEXT_SIZE);
	if (assets->fonts.main_font == NULL)
		return false;

	assets->fonts.title_font = load_font("assets/fonts/flappy-font.ttf", TITLE_TEXT_SIZE);
	if (assets->fonts.title_font == NULL)
		return false;

	return true;
}

SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path);
	if (texture == NULL)
	{
		fprintf(stderr, "Error loading image \"%s\": %s\n", path, IMG_GetError());
		return NULL;
	}

	return texture;
}

TTF_Font *load_font(const char *path, int ptsize)
{
	TTF_Font *font = TTF_OpenFont(path, ptsize);
	if (font == NULL)
	{
		fprintf(stderr, "Error loading font \"%s\": %s\n", path, TTF_GetError());
		return NULL;
	}

	return font;
}

void destroy_assets(Assets *assets)
{
	TTF_CloseFont(assets->fonts.title_font);
	TTF_CloseFont(assets->fonts.main_font);

	destroy_sound_wav(assets->sounds.bird_jump);
	destroy_sound_wav(assets->sounds.bird_hit);
	destroy_sound_wav(assets->sounds.plus_score);

	SDL_DestroyTexture(assets->text_cache.fps_texture);
	SDL_DestroyTexture(assets->text_cache.score_texture);
	
	SDL_DestroyTexture(assets->textures.background);
	SDL_DestroyTexture(assets->textures.ground);
	SDL_DestroyTexture(assets->textures.bird[0]);
	SDL_DestroyTexture(assets->textures.bird[1]);
	SDL_DestroyTexture(assets->textures.bird[2]);
	SDL_DestroyTexture(assets->textures.pipe);
}
