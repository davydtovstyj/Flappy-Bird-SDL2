#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "audio.h"
#include "config.h"

void play_sound(Mix_Chunk *sound)
{
	Mix_PlayChannel(-1, sound, 0);
}

Mix_Chunk *load_sound_wav(const char *path)
{
	Mix_Chunk *sound = NULL;
	sound = Mix_LoadWAV(path);
	
	if (sound == NULL)
	{
		fprintf(stderr, "Error loading sound \"%s\": %s\n", path, Mix_GetError());
		return NULL;
	}

	return sound;
}

void destroy_sound_wav(Mix_Chunk *sound)
{
	Mix_FreeChunk(sound);
}

bool init_audio()
{
	if ((Mix_Init(MIX_INIT_FLAGS) & MIX_INIT_FLAGS) != MIX_INIT_FLAGS)
	{
		fprintf(stderr, "Error initializing mixer: %s\n", Mix_GetError());
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIX_CHUNCK_SIZE) != 0)
	{
		fprintf(stderr, "Error opening audio device: %s\n", Mix_GetError());
		return false;
	}

	return true;
}

void deinit_audio()
{
	Mix_CloseAudio();
	Mix_Quit();
}
