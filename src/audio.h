#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

bool init_audio();
void deinit_audio();
Mix_Chunk *load_sound_wav(const char *path);
void destroy_sound_wav(Mix_Chunk *sound);
void play_sound(Mix_Chunk *sound);

#endif
