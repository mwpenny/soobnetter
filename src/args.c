/* args.c */
/* Command-line argument helper function implementations */

#include "args.h"

void arg_copy_argv(Argument* arg, char** argv, int startIndex, int endIndex)
{
	int i;

	/* Allocate space for and copy argvs in [startIndex, endIndex] */
	arg->argc = endIndex-startIndex+1;
	arg->argv = (char**)malloc(arg->argc * sizeof(char*));

	for (i = startIndex; i < startIndex + arg->argc; ++i)
	{
		arg->argv[i-startIndex] = (char*)malloc(strlen(argv[i])+1);
		strcpy(arg->argv[i-startIndex], argv[i]);
	}
}

void args_enumerate(int argc, char** argv, List* argList)
{
	int i, lastArgIndex = 0;
	Argument* currArg;
	list_init(argList);

	/* Store each command line argument in the arg list */
	for (i = 0; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			/* This is a new arg. All previous strings in argv are parameters
			   for the previous argument. Copy them into it */
			if (i - lastArgIndex > 1)
				arg_copy_argv(currArg, argv, lastArgIndex+1, i-1);

			/* Create a new argument */
			currArg = (Argument*)malloc(sizeof(Argument));
			currArg->method = argv[i]+1; /* Strip the '-' */
			currArg->argc = 0;
			currArg->argv = NULL;
			lastArgIndex = i;
			list_push(argList, currArg);
		}
		 /* Loop ending. Copy remaining params if there is an arg */
		else if (i == argc-1 && argList->size > 0)
			arg_copy_argv(currArg, argv, lastArgIndex+1, i);
	}
}

int arg_check_argc(Argument* arg, int validArgc, char* err)
{
	/* Ensure the arg has the correct number of parameters */
	if (arg->argc == validArgc)
		strcpy(err, "");
	else if (arg->argc < validArgc)
		sprintf(err, "Not enough parameters for argument '%s'.",
					 arg->method);
	else
		sprintf(err, "Too many parameters for argument '%s'.",
					 arg->method);
	return !strlen(err);
}

void arg_free(Argument* arg)
{
	/* Release memory allocated for arg params */
	int i;
	for (i = 0; i < arg->argc; ++i)
		free(arg->argv[i]);
	free(arg->argv);
}