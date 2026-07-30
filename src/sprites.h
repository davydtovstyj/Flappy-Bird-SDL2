#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct{
	SDL_Texture *texture;
	SDL_Rect rect;

	float y;
	float y_vel;
} Bird;

typedef struct{
	SDL_Texture *texture;
	SDL_Rect rect;
} Ground;

bool create_bird(Bird *bird, SDL_Texture *texture);
void update_bird(Bird *bird);
void bird_jump(Bird *bird);
bool create_ground(Ground *ground, SDL_Texture *texture);
void update_grounds(Ground *ground_l, Ground *ground_r);

#endif
