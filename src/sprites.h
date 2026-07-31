#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect rect;

	float y;
	float y_vel;
	double angle;
} Bird;

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect rect;
} Ground;

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect rect;

	int y_offset;
} Pipe;

bool create_bird(Bird *bird, SDL_Texture *texture);
void update_bird(Bird *bird);
void bird_jump(Bird *bird);
bool create_ground(Ground *ground, SDL_Texture *texture);
void update_grounds(Ground *ground_l, Ground *ground_r);
bool create_pipe(Pipe *pipe, SDL_Texture *texture, int start_x_pos);
void update_pipe(Pipe *pipe, Pipe last_pipe);
Pipe get_last_pipe(Pipe pipes[]);

#endif
