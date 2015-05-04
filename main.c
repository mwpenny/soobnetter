/*** 
	 Soobnetter 
	 Matt Penny 2015 (original version 2013)

	 A lightweight, portable IPv4 subnet calculator.
	 This is the last port/rewrite, I swear!
***/

#include "splash.h"
#include "network.h"
#include "io.h"

int main()
{
	Network n, subnet;
	int firstSub, lastSub;
	splash_show();

	/* Get host/subnet address from user and its network */
	io_get_netw(&n, &subnet);
	io_print_netw(&n, &subnet);

	/* Soobnetting time! */
	while (io_get_subnetting())
	{
		io_get_subnet_range(&n, &firstSub, &lastSub);
		io_print_subnets(&n, firstSub, lastSub);
	}

	return 0;
}