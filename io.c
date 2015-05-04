/* io.c */
/* Program input/output helper function implementations */

#include "io.h"

void io_get_netw(Network* netw, Network* subnet)
{
	char choice;
	subnet->nBits = 0;

	prompt("Enter address: ", parse_slaship, NULL, &subnet->addr);
	if (!subnet->nBits) /* don't prompt for mask/bits if network bits are known */
	{
		/* Prompt/calculate for mask from subnet or host count */
		prompt("Find mask automagically? [y/n]", parse_choice, "yn", &choice);
		if (choice == 'y')
		{
			prompt("Use number of usable soobnets or number of usable hosts/soobnet? [s/h]",
				   parse_choice, "sh", &choice);
			
			if (choice == 's')
				prompt("Enter required number of soobnets: ",
					   parse_max_subnum, subnet, &subnet->nBits);
			else
				prompt("Enter required number of hosts/soobnet: ",
					   parse_max_hostnum, subnet, &subnet->nBits);
		}

		/* Fallback: have the user enter the mask or number of bits borrowed */
		else prompt("Enter borrowed bits or soobnet mask: ", parse_bitsm,
				    subnet, &subnet->nBits);
	}

	addr_to_netaddr(&subnet->addr, &netw->addr);
	netw->nBits = subnet->nBits;
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

void io_print_netw(Network* netw, Network* subnet)
{
	char buf[512], addrStr[ADDR_STRLEN];
	Network tmp;
	int subNum = netw_subnum(subnet);

	/* Print network summary */
	addr_str(&subnet->addr, addrStr);
	netsummary_str(netw, buf);	
	printf("\nAddress: %s/%d\n\n%s\n\n\n", addrStr,
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