/* address.h */
/* IPv4 network address definition and helper functions */

#ifndef ADDRESS_H
#define ADDRESS_H

#define ADDR_STRLEN 16
#define ADDR_BITS 32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "consts.h"

typedef unsigned long Address; /* At least 32 bits */

char addrclass(Address* addr);
int addr_classbits(char class);
int addr_valid(Address* addr);
void addr_str(Address* addr, char* str);
int addr_from_str(char* str, Address* addr);
void addr_to_netaddr(Address* addr, Address* netaddr);

#endif