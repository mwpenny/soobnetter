/* io.c */
/* Program input/output helper function implementations */

#include "io.h"

void io_get_netw(Network* netw)
{
	char choice;
	netw->nBits = 0;

	prompt("Enter address: ", parse_slaship, NULL, &netw->addr);
	if (!netw->nBits) /* don't prompt for mask/bits if network bits are known */
	{
		/* Prompt/calculate for mask from subnet or host count */
		prompt("Find mask automagically? [y/n]", parse_choice, "yn", &choice);
		if (choice == 'y')
		{
			prompt("Use number of usable soobnets or number of usable hosts/soobnet? [s/h]",
				   parse_choice, "sh", &choice);
			
			if (choice == 's')
				prompt("Enter required number of soobnets: ",
					   parse_max_subnum, netw, &netw->nBits);
			else
				prompt("Enter required number of hosts/soobnet: ",
					   parse_max_hostnum, netw, &netw->nBits);
		}

		/* Fallback: have the user enter the mask or number of bits borrowed */
		else prompt("Enter borrowed bits or soobnet mask: ", parse_bitsm,
				    netw, &netw->nBits);
	}
	puts("");
}

int io_get_subnetting()
{
	char choice;
	prompt("View soobnets? [y/n]", parse_choice, "yn", &choice);
	return (choice == 'y');
}

void io_get_subnet_range(Network* netw, int* firstSub, int* lastSub)
{
	char choice;

	prompt("View a specific soobnet, or a range? [s/r]",
		   parse_choice, "sr", &choice);
	if (choice == 's')
	{
		prompt("Enter soobnet number: ", parse_subnum, netw, firstSub);
		*lastSub = *firstSub;
	}
	else
	{
		prompt("Enter first soobnet number: ", parse_subnum, netw, firstSub);
		prompt("Enter last soobnet number: ", parse_subnum, netw, lastSub);
	}
	puts("");
}

int io_apply_arg(Argument* arg, Network* netw, char* err)
{
	Network n; /* For when the network address needs to be calculated */
	int firstSub, lastSub;
	strcpy(err, "");

	/* Handle arguments */

	/* IPv4 address */
	if (!strcmp(arg->method, "a"))
	{
		if (arg_check_argc(arg, 1, err))
			parse_slaship(arg->argv[0], err, NULL, netw);
	}

	/* Subnet mask / bits borrowed */
	else if (!strcmp(arg->method, "m") || !strcmp(arg->method, "b"))
	{
		if (arg_check_argc(arg, 1, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before network bits. ");
			else if (netw->nBits)
				strcpy(err, "Network bits specified multiple times. ");
			else parse_bitsm(arg->argv[0], err, netw, &netw->nBits);
		}
	}

	/* Subnet count */
	else if (!strcmp(arg->method, "sc"))
	{
		if (arg_check_argc(arg, 1, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before soobnet count. ");
			else if (netw->nBits)
				strcpy(err, "Network bits specified multiple times. ");
			else parse_max_subnum(arg->argv[0], err, netw, &netw->nBits);
		}
	}

	/* Host count */
	else if (!strcmp(arg->method, "hc"))
	{
		if (arg_check_argc(arg, 1, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before host count. ");
			else if (netw->nBits)
				strcpy(err, "Network bits specified multiple times. ");
			else parse_max_hostnum(arg->argv[0], err, netw, &netw->nBits);
		}
	}

	/* View specific subnet */
	else if (!strcmp(arg->method, "vs"))
	{
		if (arg_check_argc(arg, 1, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before viewing a soobnet. ");
			else if (!netw->nBits)
				strcpy(err, "Network bits must be specified before viewing a soobnet. ");
			else
			{
				addr_to_netaddr(&netw->addr, &n.addr);
				n.nBits = netw->nBits;
				parse_subnum(arg->argv[0], err, &n, &firstSub);
				if (!strlen(err))
					io_print_subnets(&n, firstSub, firstSub);
			}
		}
	}

	/* View subnet range */
	else if (!strcmp(arg->method, "vr"))
	{
		if (arg_check_argc(arg, 2, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before viewing soobnets. ");
			else if (!netw->nBits)
				strcpy(err, "Network bits must be specified before viewing soobnets. ");
			else
			{
				addr_to_netaddr(&netw->addr, &n.addr);
				n.nBits = netw->nBits;
				parse_subnum(arg->argv[0], err, &n, &firstSub);
				parse_subnum(arg->argv[1], err, &n, &lastSub);
				if (!strlen(err))
					io_print_subnets(&n, firstSub, lastSub);
			}
		}
	}

	/* Network summary */
	else if (!strcmp(arg->method, "s"))
	{
		if (arg_check_argc(arg, 0, err))
		{
			if (!netw->addr)
				strcpy(err, "Address must be specified before viewing a summary. ");
			else if (!netw->nBits)
				strcpy(err, "Network bits must be specified before viewing a summary. ");
			else
			{
				addr_to_netaddr(&netw->addr, &n.addr);
				n.nBits = netw->nBits;
				io_print_netw(&n, netw);
			}
		}
	}
	else
		sprintf(err, "Invalid arugment '%s'. ", arg->method);

	return !strlen(err); /* Was there no error? */
}

void io_print_syntax()
{
	/* Prints the usage and syntax */
	puts("Soobnets a network and calculates useful network information\n\nUsage:");
	puts("Soobneter -a ADDRESS [-m MASK | -b BITS_BORROWED | -sc SOOBNET_COUNT"
		 "| -hc HOST_COUNT] [-s] [-vs SOOBNET_NUM | -vr LOWER_BOUND UPPER_BOUND] [-q] [-? | --help]\n");

	printf("%-29sSets host address in dotted decimal or slash form.\n", "-a  ADDRESS");
	printf("%-29sSets the soobnet mask.\n", "-m  MASK");
	printf("%-29sSets the number of bits borrowed.\n", "-b  BITS_BORROWED");
	printf("%-29sSets the number of usable soobnets required.\n", "-sc SOOBNET_COUNT");
	printf("%-29sSets the number of usable hosts/soobnet required.\n", "-hc HOST_COUNT");
	printf("%-29sDisplays a network summary.\n", "-s");
	printf("%-29sSpecifies the number of the soobnet to display.\n", "-vs SOOBNET_NUM");
	printf("%-29sSpecifies the soobnet range to display, inclusive.\n", "-vr LOWER_BOUND UPPER_BOUND");
	printf("%-29sDisables interactive soobnet viewing.\n", "-q");
	printf("%-29sDisplays this help screen.\n", "-? --help");
}

void io_print_netw(Network* netw, Network* subnet)
{
	char buf[512], addrStr[ADDR_STRLEN];
	Network tmp;
	int subNum = netw_subnum(subnet);

	/* Print network summary */
	addr_str(&subnet->addr, addrStr);
	netsummary_str(netw, buf);	
	printf("Address: %s/%d\n\n%s\n\n\n", addrStr,
		subnet->nBits, buf);

	/* Print information about the subnet the user-entered address is on */
	netw_subnet(netw, subNum, &tmp);
	netw_subsummary_str(&tmp, buf);

	if (subnet->addr == tmp.addr)
	{
		printf("%s is the address of soobnet %d\n\n", addrStr, subNum);
	}
	else
	{
		netbroadcast(&tmp, &tmp.addr);
		if (subnet->addr == tmp.addr)
		{
			printf("%s is the broadcast address of soobnet %d\n\n",
				addrStr, subNum);
		}
		else
		{
			printf("%s is on soobnet %d\n\n", addrStr, subNum);
		}
	}

	io_print_subnet_header();
	printf("%s\n", buf);
}

void io_print_subnet_header()
{
	printf("%-10s%-16s%-35s%s\n", "Soobnet", "Soobnet",
		"Soobnet host", "Broadcast");
	printf("%-10s%-16s%-35s%s\n", "number", "address",
		"range", "address");
	printf("%-10s%-16s%-35s%s\n", "-------", "-------",
		"------------", "---------");
}

void io_print_subnets(Network* netw, int firstSub, int lastSub)
{
	char buf[128];
	Network tmp;
	int i, step = 1;

	/* In case the bounds are entered in the wrong order */
	if (firstSub > lastSub) step = -1;
	io_print_subnet_header();

	for (i = firstSub; i != lastSub + step; i += step)
	{
		netw_subnet(netw, i, &tmp);
		netw_subsummary_str(&tmp, buf);
		printf("%s", buf);
	}
	puts("");
}