/* io.h */
/* Program input/output helper function definitions */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "network.h"
#include "prompt.h"

void io_get_netw(Network* netw, Network* subnet);
int io_get_subnetting();
void io_get_subnet_range(Network* netw, int* firstSub, int* lastSub);

void io_print_netw(Network* netw, Network* subnet);
void io_print_subnet_header();
void io_print_subnets(Network* netw, int firstSub, int lastSub);

#endif