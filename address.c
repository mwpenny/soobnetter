/* address.c */
/* IPv4 network address helper function implementations */

#include "address.h"

char addrclass(Address* addr)
{
	/* Get the class using the first octet of the network address */
	unsigned char oct1 = (*addr & 0xFF000000) >> 24;
	if (oct1 > 0 && oct1 < 127) return 'A';
	if (oct1 > 127 && oct1 < 192) return 'B';
	if (oct1 > 191 && oct1 < 224) return 'C';
	if (oct1 > 223 && oct1 < 240) return 'D';
	if (oct1 > 239 && oct1 < 256) return 'E';
	return '?';
}

int addr_classbits(char class)
{
	switch(class)
	{
	case 'A':
		return 8;
	case 'B':
		return 16;
	case 'C':
		return 24;
	default:
		return 0;
	}
}

void addr_str(Address* addr, char* str)
{
	/* Use decimal octet values for dotted decimal notation */
	sprintf(str, "%lu.%lu.%lu.%lu", (*addr & 0xFF000000) >> 24,
									(*addr & 0x00FF0000) >> 16,
									(*addr & 0x0000FF00) >> 8,
									*addr & 0x000000FF);
}

int addr_from_str(char* str, Address* addr)
{
	char *tok, *end;
	long int num;
	int oct = 1;
	Address out = 0;

	tok = strtok(str, ".");
	while (tok != NULL)
	{
		if (oct > 4) return FALSE; /* Too many octets */

		/* Parse octets in the dotted decimal string */
		num = strtol(tok, &end, 10);
		if (end != tok)
		{
			if (num > -1 && num < 256)
				out |= num << (ADDR_BITS-(oct++*8));
			else /* Octet not a positive 8-bit number */
				return FALSE;
		}
		else /* String could not be parsed as a long int */
			return FALSE;
		tok = strtok(NULL, ".");
	}
	if (oct < 5) return FALSE; /* Too few octets */
	else
	{
		memcpy(addr, &out, sizeof(Address));
		return TRUE;
	}
}

void addr_to_netaddr(Address* addr, Address* netaddr)
{
	/* Only keep network bits */
	int classBits = addr_classbits(addrclass(addr));
	*netaddr = *addr & ~((1<<(ADDR_BITS-classBits))-1);
}