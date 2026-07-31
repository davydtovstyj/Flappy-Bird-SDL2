#include "render.h"
#include "config.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void render_present(SDL_Renderer *renderer)
{	
	SDL_RenderPresent(renderer);
}

void render_clear(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);
}

void render_sprite(SDL_Renderer *renderer, SDL_Texture *sprite, SDL_Rect *sprite_rect, const double angle, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(renderer, sprite, NULL, sprite_rect, angle, NULL, flip);
}

void render_background(SDL_Renderer *renderer, SDL_Texture *background)
{
	SDL_RenderCopy(renderer, background, NULL, NULL);
}

bool init_sdl()
{
	if (SDL_Init(SDL_INIT_FLAGS) != 0)
	{
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	if ((IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS) != IMG_INIT_FLAGS)
	{
		fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
		return false;
	}
	
	return true;
}

void deinit_sdl()
{
	IMG_Quit();
	SDL_Quit();
}

SDL_Window *create_window()
{
	SDL_Window *window = SDL_CreateWindow(
		WINDOW_TITLE, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH/2,
		WINDOW_HEIGHT/2,
		WINDOW_FLAGS
		);
	if (window == NULL)
	{
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return NULL;
	}
	
	return window;
}

void destroy_window(SDL_Window *window)
{
	SDL_DestroyWindow(window);
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, RENDER_FLAGS);
	if (renderer == NULL)
	{
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return NULL;
	}

	// Keep game aspect ratio same
	if (SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT) != 0)
	{
		fprintf(stderr, "Error setting logical size for renderer: %s\n", SDL_GetError());
		return NULL;
	}
	
	return renderer;
}

void destroy_renderer(SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
}
