/* parser.h */
/* String parser function implementations */

#include "parser.h"

int parse_addr(char* str, char* err, void* arg, void* addr)
{
	Address tmp;
	char class;

	if(addr_from_str(str, &tmp))
	{
		/* Can only subnet class A, B, or C */
		class = addrclass(&tmp);
		if (class != 'A' && class != 'B' && class != 'C')
		{
			sprintf(err, "Invalid address class (%c). ", class);
			return FALSE;
		}
		else
		{
			memcpy((Address*)addr, &tmp, sizeof(Address));
			strcpy(err, "");
			return TRUE;
		}
	}
	else
	{
		strcpy(err, "Invalid address. ");
		return FALSE;
	}
}

int parse_bb(char* str, char* err, void* netw, void* nBits)
{
	char* end;
	long int bits;
	int classBits = addr_classbits(addrclass(&((Network*)netw)->addr));
	
	bits = strtol(str, &end, 10);
	if (end != str)
	{
		if (netw_borrowed_valid((Network*)netw, bits))
		{
			/* The number of borrowed bits is valid. Convert to network bits */
			*(unsigned char*)nBits = (int)bits + classBits;
			strcpy(err, "");
			return TRUE;
		}
		else
		{
			sprintf(err, "Invalid number of borrowed bits (%d). ", (int)bits);
			return FALSE;
		}
	}
	else
	{
		strcpy(err, "Invalid entry. ");
		return FALSE;
	}
}

int parse_sm(char* str, char* err, void* netw, void* nBits)
{
	Address mask;

	if(addr_from_str(str, &mask))
	{
		/* Make sure the entered mask is within bounds */
		if (!netmask_valid((Network*)netw, &mask))
		{
			strcpy(err, "Invalid mask. ");
			return FALSE;
		}
		else
		{
			*(unsigned char*)nBits = netbits_from_mask(&mask);
			strcpy(err, "");
			return TRUE;
		}
	}
	else
	{
		strcpy(err, "");
		return FALSE;
	}
}

int parse_bitsm(char* str, char* err, void* netw, void* nBits)
{
	if (parse_sm(str, err, netw, nBits))
		return TRUE;
	else if (!strlen(err))
		return parse_bb(str, err, netw, nBits);
	else
		return FALSE;
}

int parse_slaship(char* str, char* err, void* arg, void* netw)
{
	char *tok, *ipStr, *numBitsString, *end;
	long int numBits;
	int tokCount = 0, pass;
	
	/* Get ip and number of network bits from string */
	tok = strtok(str, "/");
	while (tok != NULL)
	{
		++tokCount;

		if (tokCount == 1) ipStr = tok;
		else if (tokCount == 2) numBitsString = tok;
		else return FALSE; /* Too many tokens */

		tok = strtok(NULL, "/");
	}

	/* Too few tokens */
	if (tokCount == 0)
	{
		strcpy(err, "Invalid input. ");
		return FALSE;
	}

	/* Token 1 = IP */
	if (!parse_addr(ipStr, err, NULL, &((Network*)netw)->addr))
		return FALSE;
	
	/* Token 2 = network bits */
	if (tokCount == 2)
	{
		/* Subtract normal network bit count to get borrowed bits */
		numBits = strtol(numBitsString, &end, 10);
		if (numBitsString != end)
		{
			numBits -= addr_classbits(addrclass(&((Network*)netw)->addr));
			sprintf(numBitsString, "%d", (int)numBits);
		}

		/* Check with existing function */
		pass = parse_bb(numBitsString, err, netw,
						&((Network*)netw)->nBits);
		if (!pass)
		{
			strcpy(err, "Invalid number of network bits. ");
			return FALSE;
		}
	}
	strcpy(err, "");
	return TRUE;
}

int parse_choice(char* str, char* err, void* choices, void* choice)
{
	unsigned int i;
	char *c;

	strcpy(err, "");
	if (strlen(str) == 0) return FALSE;

	/* Check for valid options */
	for (i = 0; i < strlen((char*)choices); ++i)
	{
		/* Be case-insensitive */
		c = (char*)(memchr(str, ((char*)choices)[i], 1));
		c = !c ? (char*)memchr(str, toupper(((char*)choices)[i]), 1) : c;

		if (c)
		{
			*(char*)choice = tolower(*c);
			return TRUE;
		}
	}
	return FALSE; /* no valid choice found */
}

int parse_subnum(char* str, char* err, void* netw, void* subnum)
{
	char* end;
	long int num = strtol(str, &end, 10);

	if (str != end)
	{
		/* Validate subnet number */
		if (!netw_subnum_valid((Network*)netw, num))
		{
			sprintf(err, "Invalid soobnet (%ld). ", num);
			return FALSE;
		}
		else
		{
			strcpy(err, "");
			*(int*)subnum = (int)num;
			return TRUE;
		}
	}
	else
	{
		strcpy(err, "Invalid entry. ");
		return FALSE;
	}
}

int parse_max_subnum(char* str, char* err, void* netw, void* nBits)
{
	char bitStr[16];
	char* end;
	int bits, pass;
	long int num = strtol(str, &end, 10);

	if (str != end)
	{
		/* Get required number of network bits and validate */
		bits = netw_subcount_bits(num, addrclass(&((Network*)netw)->addr));
		sprintf(bitStr, "%d",
			    bits - addr_classbits(addrclass(&((Network*)netw)->addr)));
		pass = parse_bb(bitStr, err, netw, &((Network*)netw)->nBits);

		if (!pass)
		{
			sprintf(err, "Invalid number of soobnets (%ld). ", num);
			return FALSE;
		}
		else
		{
			strcpy(err, "");
			return TRUE;
		}
	}
	else
	{
		strcpy(err, "Invalid entry. ");
		return FALSE;
	}
}

int parse_max_hostnum(char* str, char* err, void* netw, void* nBits)
{
	char bitStr[16];
	char* end;
	long int num = strtol(str, &end, 10);
	int bits, pass;

	if (str != end)
	{
		/* Get required number of network bits and validate */
		bits = netw_hostcount_bits(num);
		sprintf(bitStr, "%d",
			    bits - addr_classbits(addrclass(&((Network*)netw)->addr)));
		pass = parse_bb(bitStr, err, netw, &((Network*)netw)->nBits);

		if (!pass)
		{
			sprintf(err, "Invalid number of hosts/soobnet (%ld). ", num);
			return FALSE;
		}
		else
		{
			strcpy(err, "");
			return TRUE;
		}
	}
	else
	{
		strcpy(err, "Invalid entry. ");
		return FALSE;
	}
}