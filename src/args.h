/* args.h */
/* Command-line argument data structure and helper function definitions */

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct
{
	char* method;
	int argc;
	char** argv;
} Argument;

void arg_copy_argv(Argument* arg, char** argv, int startIndex, int endIndex);
void args_enumerate(int argc, char** argv, List* argList);
int arg_check_argc(Argument* arg, int validArgc, char* err);
void arg_free(Argument* arg);

#endif