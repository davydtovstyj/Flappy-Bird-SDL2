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
	if (sprite != NULL)
		SDL_RenderCopyEx(renderer, sprite, NULL, sprite_rect, angle, NULL, flip);
}

void render_background(SDL_Renderer *renderer, SDL_Texture *background)
{
	SDL_RenderCopy(renderer, background, NULL, NULL);
}

void update_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, SDL_Texture **out_texture, SDL_Rect *out_rect)
{
	if (*out_texture != NULL) {
			SDL_DestroyTexture(*out_texture);
	}

	SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
	if (!surface) 
		return;

	*out_texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (*out_texture != NULL)
	{
		out_rect->x = x;
		out_rect->y = y - surface->h;
		out_rect->w = surface->w;
		out_rect->h = surface->h;
	}

	SDL_FreeSurface(surface);
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

	if (TTF_Init() != 0)
	{
		fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
		return false;
	}
	
	return true;
}

void deinit_sdl()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Window *create_window()
{
	SDL_Window *window = SDL_CreateWindow(
		WINDOW_TITLE, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH/3,  // TODO: calculate window scale
		WINDOW_HEIGHT/3, // depending on screen size
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

bool update_fps_counter(double deltaTime, int *out_fps)
{
	static double timer = 0.0;
	static int frames = 0;

	timer += deltaTime;
	frames++;

	if (timer >= 1.0)
	{
		*out_fps = frames;

		timer -= 1.0;
		frames = 0;
		
		return true;
	}

	return false;
}

void debug_draw_hitbox(SDL_Renderer *renderer, SDL_Rect *hitbox)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, hitbox);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
