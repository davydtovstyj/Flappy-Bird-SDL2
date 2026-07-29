#ifndef RENDER_H
#define RENDER_H

#include "game.h"

bool init_sdl();
void deinit_sdl();
bool create_window(Game *game);
void destroy_window(Game *game);
bool create_renderer(Game *game);
void destroy_renderer(Game *game);

#endif
