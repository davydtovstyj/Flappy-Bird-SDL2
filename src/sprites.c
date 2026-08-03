#include "sprites.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

bool create_bird(Bird *bird, SDL_Texture *texture)
{
	bird->texture = texture;
	bird->y_vel = BIRD_START_Y_VEL;
	bird->angle = 0;

	if (SDL_QueryTexture(texture, NULL, NULL, &bird->rect.w, &bird->rect.h) != 0)
	{
		fprintf(stderr, "Error quering bird texture: %s\n", SDL_GetError());
		return false;
	}

	bird->rect.w *= BIRD_SCALE;
	bird->rect.h *= BIRD_SCALE;

	bird->rect.x = BIRD_X_MARGIN;
	bird->rect.y = (WINDOW_HEIGHT / 2) - (bird->rect.h / 2); // Center of the screen
	bird->y = (float)bird->rect.y;

	return true;
}

void update_bird(Bird *bird, double deltaTime)
{
	bird->y_vel += BIRD_GRAVITY_FORCE * deltaTime;

	// Clip max velocity
	if (bird->y_vel > BIRD_MAX_Y_VEL)
		bird->y_vel = BIRD_MAX_Y_VEL;

	bird->angle = bird->y_vel * BIRD_ANGLE_COEF;
	
  if (bird->angle < -25.0) 
		bird->angle = -25.0;
  if (bird->angle > 45.0)  
		bird->angle = 45.0;
	
	bird->y += bird->y_vel * deltaTime;
	bird->rect.y = (int)bird->y;
}

void bird_jump(Bird *bird)
{
	if (bird->rect.y > 0)
	{
		bird->y_vel = BIRD_JUMP_FORCE;
	}
}

bool create_ground(Ground *ground, SDL_Texture *texture)
{
	ground->texture = texture;

	if (SDL_QueryTexture(texture, NULL, NULL, &ground->rect.w, &ground->rect.h) != 0)
	{
		fprintf(stderr, "Error quering ground texture: %s\n", SDL_GetError());
		return false;
	}

	float scale = (float)WINDOW_WIDTH / ground->rect.w;

	ground->rect.w = WINDOW_WIDTH;
	ground->rect.h *= scale;

	ground->rect.x = 0;
	ground->x = 0;
	ground->rect.y = WINDOW_HEIGHT - ground->rect.h;

	return true;
}

void update_grounds(Ground *ground_l, Ground *ground_r, double deltaTime)
{
	ground_l->x += FOREGROUND_X_SPEED * deltaTime;

	if (fabs(ground_l->rect.x) > ground_l->rect.w)
	{
		ground_l->x += ground_l->rect.w;
	}
	
	ground_r->x = ground_l->x + ground_l->rect.w;

	ground_l->rect.x = (int)ground_l->x;
	ground_r->rect.x = (int)ground_r->x;
}

bool create_pipe(Pipe *pipe, SDL_Texture *texture, int start_x_pos)
{
	pipe->texture = texture;
	
	pipe->y_offset = rand() % MAX_PIPE_Y_OFFSET;
	if (rand() % 2 == 0)
		pipe->y_offset *= -1;

	if (SDL_QueryTexture(texture, NULL, NULL, &pipe->rect.w, &pipe->rect.h) != 0)
	{
		fprintf(stderr, "Error quering pipe texture: %s\n", SDL_GetError());
		return false;
	}

	pipe->rect.w *= PIPE_SCALE;
	pipe->rect.h *= PIPE_SCALE;

	pipe->rect.x = start_x_pos;
	pipe->x = start_x_pos;
	pipe->rect.y = WINDOW_HEIGHT / 2 + pipe->y_offset;

	return true;
}

void update_pipe(Pipe *pipe, Pipe *last_pipe, double deltaTime)
{
	pipe->x += FOREGROUND_X_SPEED * deltaTime;

	if (pipe->x + pipe->rect.w < 0)
	{
		pipe->x = last_pipe->x + last_pipe->rect.w + DISTANCE_BETWEEN_PIPES;
		pipe->y_offset = rand() % MAX_PIPE_Y_OFFSET;
		if (rand() % 2 == 0)
			pipe->y_offset *= -1;
		pipe->rect.y = WINDOW_HEIGHT / 2 + pipe->y_offset;
	}

	pipe->rect.x = (int)pipe->x;
}

Pipe get_last_pipe(Pipe pipes[])
{
	int max_x = pipes[0].rect.x;
	Pipe last_pipe = pipes[0];

	for (int i = 0; i < MAX_PIPES_COUNT; i++)
	{
		if (pipes[i].rect.x > max_x)
		{
			max_x = pipes[i].rect.x;
			last_pipe = pipes[i];
		}
	}

	return last_pipe;
}

