/* network.c */
/* Network helper function implementations */

#include "network.h"

int netw_bits_borrowed(Network* netw)
{
	return netw->nBits - addr_classbits(addrclass(&netw->addr));
}

int netw_borrowed_valid(Network* netw, int borrowed)
{
	/* Are borrowed bits in bounds? */
	int classBits = addr_classbits(addrclass(&netw->addr));
	return (borrowed > -1 && borrowed + classBits < ADDR_BITS-1);
}

void netmask(Network* net, Address* mask)
{
	*mask = ~((1<<(ADDR_BITS-net->nBits)) - 1);
}

int netmask_valid(Network* netw, Address* mask)
{
	int classBits = addr_classbits(addrclass(&netw->addr));
	unsigned long minMask = ~(((1<<(ADDR_BITS-classBits))-1));

	/* 3 = 00000011 */
	return (*mask > minMask-1 && *mask < (Address)~(3)+1 &&
		    !(~*mask & ((~*mask)+1))); /* Valid masks are all 1s followed by all 0s */
}

int netbits_from_mask(Address* mask)
{
	/* Relies on the fact that masks are all 1s followed by all 0s
	   Count number of host bits to find network bits (avoids overflow) */
	return (int)(ADDR_BITS - log(~*mask+1.)/log(2.));
}

void netbroadcast(Network* netw, Address* broadcast)
{
	/* Make all the network's host bits 1 to get the broadcast address */
	*broadcast = netw->addr | ((1<<(ADDR_BITS-netw->nBits)) - 1);
}

int netw_maxsubs(Network* netw)
{
	/* 2^(borrowed bits) */
	return (1 << netw_bits_borrowed(netw));
}

int netw_maxhosts(Network* netw)
{
	/* 2^(host bits) - 2 */
	return (1<<(ADDR_BITS-netw->nBits))-2;
}

void netsummary_str(Network* netw, char* str)
{
	char addrStr[ADDR_STRLEN], maskStr[ADDR_STRLEN];
	Address mask;

	/* Generate strings for the network's information */
	addr_str(&netw->addr, addrStr);

	netmask(netw, &mask);
	addr_str(&mask, maskStr);
	
	/* Display network information */
	sprintf(str, "%-16s%-9s%-11s%-16s%-13s%s\n", "Network", "Class", "Bits",
					"Soobnet", "Maximum", "Usable");
	sprintf(str + strlen(str), "%-25s%-11s%-16s%-13s%s\n", "address", "borrowed",
					"mask", "soobnets", "hosts/soobnet");
	sprintf(str + strlen(str), "%-16s%-9s%-11s%-16s%-13s%s\n", "-------",
					"-----", "--------", "-------",	"--------", "-------------");
	sprintf(str + strlen(str), "%-16s%-9c%-11d%-16s%-13d%d", addrStr,
							   addrclass(&netw->addr), netw_bits_borrowed(netw),
							   maskStr, netw_maxsubs(netw), netw_maxhosts(netw));
}

void netw_subnet(Network* netw, int subnum, Network* subnet)
{
	/* OR the binary subnet number with the borrowed bits portion of the address */
	subnet->addr = netw->addr | subnum << (ADDR_BITS-netw->nBits);
	subnet->nBits = netw->nBits;
}

int netw_subnum(Network* subnet)
{
	int borrowed = netw_bits_borrowed(subnet);
	unsigned long subNumMask = ((1<<borrowed)-1) << (ADDR_BITS-subnet->nBits);

	/* Isolate the borrowed bits of the address */
	return (subnet->addr & subNumMask) >> (ADDR_BITS-subnet->nBits);
}

int netw_subnum_valid(Network* netw, int subnum)
{
	return (subnum > -1 && subnum < netw_maxsubs(netw));
}

void netw_subsummary_str(Network* subnet, char* str)
{
	Address firstHost, broadcast, lastHost;
	char addrStr[ADDR_STRLEN], firstHostStr[ADDR_STRLEN],
		 lastHostStr[ADDR_STRLEN], broadcastStr[ADDR_STRLEN],
		 hostRangeStr[2*ADDR_STRLEN+2];

	/* Generate strings for the subnet's information */
	addr_str(&subnet->addr, addrStr);

	firstHost = subnet->addr+1;
	addr_str(&firstHost, firstHostStr);

	netbroadcast(subnet, &broadcast);
	addr_str(&broadcast, broadcastStr);

	lastHost = broadcast-1;
	addr_str(&lastHost, lastHostStr);

	sprintf(hostRangeStr, "%s - %s", firstHostStr, lastHostStr);


	/* Display subnet information */
	sprintf(str, "%-10d%-16s%-35s%s\n", netw_subnum(subnet),
							   addrStr, hostRangeStr, broadcastStr);
}

int netw_subcount_bits(int subcount, char class)
{
	/* Borrowed bits + network bits for the class.
	   Number of bits needed to accommodate subcount subnets */
	return (int)ceil((log((double)subcount)/log(2.))) + 
		   addr_classbits(class);
}

int netw_hostcount_bits(int hostcount)
{
	/* Number of bits needed to accommodate hostcount hosts */
	return ADDR_BITS - (int)ceil((log(hostcount+2.)/log(2.)));
}