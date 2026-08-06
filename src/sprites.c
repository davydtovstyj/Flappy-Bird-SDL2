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

	if (SDL_QueryTexture(texture, NULL, NULL, &bird->rect.w, &bird->rect.h) != 0)
	{
		fprintf(stderr, "Error quering bird texture: %s\n", SDL_GetError());
		return false;
	}

	bird->rect.w *= BIRD_SCALE;
	bird->rect.h *= BIRD_SCALE;

	bird->rect.x = BIRD_X_MARGIN;

	reset_bird(bird);

	return true;
}

void reset_bird(Bird *bird)
{
	bird->y_vel = 0.0f;
	bird->angle = 0;
	bird->curr_frame = 0;
	bird->anim_direction = 1;
	bird->anim_timer = 0.0;

	bird->rect.y = (WINDOW_HEIGHT / 2) - (bird->rect.h / 2); // Center of the screen
	bird->y = (float)bird->rect.y;
}

void update_bird(Bird *bird, double deltaTime)
{
	bird->y_vel += BIRD_GRAVITY_FORCE * deltaTime;

	// Clip max velocity
	if (bird->y_vel > BIRD_MAX_Y_VEL)
		bird->y_vel = BIRD_MAX_Y_VEL;
	
  double target_angle = 0.0;

  if (bird->y_vel < 0)
		target_angle = -25.0;
	else
		target_angle = (bird->y_vel / BIRD_MAX_Y_VEL) * 90.0;

	bird->angle += (target_angle - bird->angle) * BIRD_ROTATION_SPEED * deltaTime; // LERP
	
	bird->y += bird->y_vel * deltaTime;
	bird->rect.y = (int)bird->y;
}

void update_bird_anim(Bird *bird, SDL_Texture *textures[], double deltaTime)
{
	bird->anim_timer += deltaTime;

	if (bird->anim_timer >= BIRD_ANIM_FRAME_TIME)
	{
		if (bird->curr_frame <= 0)
			bird->anim_direction = 1;
		else if (bird->curr_frame >= BIRD_ANIM_FRAMES_COUNT - 1)
			bird->anim_direction = -1;

		bird->curr_frame += bird->anim_direction;
		bird->anim_timer -= BIRD_ANIM_FRAME_TIME;
	}
	
	bird->texture = textures[bird->curr_frame];
}

void bird_jump(Bird *bird)
{
	bird->curr_frame = 0;
	bird->anim_timer = 0;
	
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
	
	ground->rect.y = WINDOW_HEIGHT - ground->rect.h;

	reset_ground(ground);

	return true;
}

void reset_ground(Ground *ground)
{
	ground->rect.x = 0;
	ground->x = 0;
}

void update_grounds(Ground *ground_l, Ground *ground_r, double deltaTime)
{
	ground_l->x += FOREGROUND_X_SPEED * deltaTime;

	if (fabs(ground_l->x) > ground_l->rect.w)
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

	if (SDL_QueryTexture(texture, NULL, NULL, &pipe->rect.w, &pipe->rect.h) != 0)
	{
		fprintf(stderr, "Error quering pipe texture: %s\n", SDL_GetError());
		return false;
	}

	pipe->rect.w *= PIPE_SCALE;
	pipe->rect.h *= PIPE_SCALE;

	reset_pipe(pipe, start_x_pos);

	return true;
}

void reset_pipe(Pipe *pipe, int start_x_pos)
{
	pipe->y_offset = rand() % MAX_PIPE_Y_OFFSET;
	if (rand() % 2 == 0)
		pipe->y_offset *= -1;

	pipe->rect.x = start_x_pos;
	pipe->x = start_x_pos;
	pipe->rect.y = WINDOW_HEIGHT / 2 + pipe->y_offset;
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

