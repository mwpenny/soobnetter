/* parser.h */
/* String parser function definitions */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "consts.h"
#include "address.h"
#include "network.h"

typedef int (*StrParser)(char* str, char* err, void* arg, void* out);

int parse_addr(char* str, char* err, void* arg, void* addr);
int parse_sm(char* str, char* err, void* netw, void* nBits);
int parse_bb(char* str, char* err, void* netw, void* nBits);
int parse_bitsm(char* str, char* err, void* netw, void* nBits);
int parse_slaship(char* str, char* err, void* arg, void* netw);
int parse_choice(char* str, char* err, void* choices, void* choice);
int parse_subnum(char* str, char* err, void* netw, void* subnum);
int parse_max_subnum(char* str, char* err, void* netw, void* nBits);
int parse_max_hostnum(char* str, char* err, void* netw, void* nBits);

#endif