#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct 
{
	bool jump;
	bool quit;
	bool restart;
} Action;

void update_input(Action *action);

#endif
