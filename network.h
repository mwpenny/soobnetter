/* network.h */
/* Network definition and helper functions */

#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "consts.h"
#include "address.h"

typedef struct
{
	Address addr; /* 32-bit; IPv4 */
	unsigned char nBits;
} Network;

int netw_bits_borrowed(Network* netw);
int netw_borrowed_valid(Network* netw, int borrowed);
void netmask(Network* netw, Address* mask);
int netmask_valid(Network* netw, Address* mask);
int netbits_from_mask(Address* mask);
void netbroadcast(Network* netw, Address* broadcast);
int netw_maxsubs(Network* netw);
int netw_maxhosts(Network* netw);
void netsummary_str(Network* netw, char* str);

void netw_subnet(Network* netw, int subnum, Network* subnet);
int netw_subnum(Network* subnet);
int netw_subnum_valid(Network* netw, int subnum);
void netw_subsummary_str(Network* subnet, char* str);

int netw_subcount_bits(int subcount, char class);
int netw_hostcount_bits(int hostcount);

#endif