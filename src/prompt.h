/* prompt.h */
/* Input prompt and definition */

#ifndef PROMPT_H
#define PROMPT_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define PROMPT_BUF_SIZE 64

void prompt(char* promptStr, StrParser check, void* arg, void* out);

#endif