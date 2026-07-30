#include "sprites.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool create_bird(Bird *bird, SDL_Texture *texture)
{
	bird->texture = texture;
	bird->y_vel = 1.0f;

	if (SDL_QueryTexture(texture, NULL, NULL, &bird->rect.w, &bird->rect.h) != 0)
	{
		fprintf(stderr, "Error quering bird texture: %s\n", SDL_GetError());
		return false;
	}

	bird->rect.w *= BIRD_SCALE;
	bird->rect.h *= BIRD_SCALE;

	bird->rect.x = 50;
	bird->rect.y = (WINDOW_HEIGHT / 2) - (bird->rect.h / 2);
	bird->y = (float)bird->rect.y;

	return true;
}

void update_bird(Bird *bird)
{
	bird->y_vel += BIRD_GRAVITY_FORCE;

	// Clip max velocity
	if (bird->y_vel > BIRD_MAX_Y_VEL)
		bird->y_vel = BIRD_MAX_Y_VEL;
	
	bird->y += bird->y_vel;
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
	ground->rect.y = WINDOW_HEIGHT - ground->rect.h;

	return true;
}

void update_grounds(Ground *ground_l, Ground *ground_r)
{
	ground_l->rect.x += FOREGROUND_X_SPEED;
	ground_r->rect.x = ground_l->rect.x + ground_l->rect.w;

	if (abs(ground_l->rect.x) > ground_l->rect.w)
	{
		ground_l->rect.x = 0;
		ground_r->rect.x = ground_l->rect.w;
	}
}

