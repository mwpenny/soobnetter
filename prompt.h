/* prompt.h */
/* Input prompt and validation function definitions */

#ifndef PROMPT_H
#define PROMPT_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "address.h"
#include "network.h"
#include "consts.h"

typedef int (*StrParser)(char* str, char* err, void* arg, void* out);
void prompt(char* promptStr, StrParser check, void* arg, void* out);

int parse_addr(char* str, char* err, void* arg, void* addr);
int parse_bb(char* str, char* err, void* netw, void* nBits);
int parse_bitsm(char* str, char* err, void* netw, void* nBits);
int parse_slaship(char* str, char* err, void* arg, void* netw);
int parse_choice(char* str, char* err, void* choices, void* choice);
int parse_subnum(char* str, char* err, void* netw, void* subnum);
int parse_max_subnum(char* str, char* err, void* netw, void* nBits);
int parse_max_hostnum(char* str, char* err, void* netw, void* nBits);

#endif