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
	
	double anim_timer;
	int curr_frame;
	int anim_direction;
} Bird;

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect rect;
	
	float x;
} Ground;

typedef struct
{
	SDL_Texture *texture;
	SDL_Rect rect;

	float x;
	int y_offset;
} Pipe;

bool create_bird(Bird *bird, SDL_Texture *texture);
void update_bird(Bird *bird, double deltaTime);
void update_bird_anim(Bird *bird, SDL_Texture *textures[], double deltaTime);
void bird_jump(Bird *bird);
bool create_ground(Ground *ground, SDL_Texture *texture);
void update_grounds(Ground *ground_l, Ground *ground_r, double deltaTime);
bool create_pipe(Pipe *pipe, SDL_Texture *texture, int start_x_pos);
void update_pipe(Pipe *pipe, Pipe *last_pipe, double deltaTime);
Pipe get_last_pipe(Pipe pipes[]);

#endif
