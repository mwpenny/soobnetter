/*** 
	Soobnetter 
	Matt Penny 2015 (original version 2013)

	A lightweight, portable IPv4 subnet calculator.
	This is the last port/rewrite, I swear!
***/

#include "args.h"
#include "list.h"
#include "splash.h"
#include "network.h"
#include "io.h"

/* Whether or not to interactively view subnets */
int interactive = 1;

int apply_args(List* argList, Network* netw)
{
	char err[PROMPT_BUF_SIZE];
	int success = 1, syntaxShown = 0;
	ListNode* n = argList->head;
	Argument* a;

	/* Apply every command-line argument */
	while (n != NULL)
	{
		a = (Argument*)n->data;

		/* Non-interactive mode */
		if (!strcmp(a->method, "q"))
			interactive = 0;

		/* Help */
		else if (!strcmp(a->method, "?") || !strcmp(a->method, "-help"))
		{
			syntaxShown = 1;
			interactive = 0;
			io_print_syntax();
		}
		else if (success)
		{
			/* Don't break out of the loop on fail.
			The args need to be cleaned up */
			success = io_apply_arg(a, netw, err);
			if (!success)
				printf("Error: %s\nUse argument -? or --help for syntax.\n", err);
		}

		arg_free(a);
		n = n->next;
	}

	/* We need these to do anything */
	if (!success) return FALSE;
	else if (syntaxShown) return TRUE;
	else if (!netw->addr)
	{
		puts("Address not specified.");
		return FALSE;
	}
	else if (!netw->nBits)
	{
		puts("Network bits not specified.");
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char** argv)
{
	List argList;
	Network n, subnet = {0,0};
	int firstSub, lastSub, success;

	splash_show(); /* Love me some ASCII art */

	/* Parse args if they are present */
	if (argc > 1)
	{
		args_enumerate(argc-1, argv+1, &argList);
		success = apply_args(&argList, &subnet);
		list_free(&argList);
		if (!success) return EXIT_FAILURE;
		addr_to_netaddr(&subnet.addr, &n.addr);
		n.nBits = subnet.nBits;
	}
	else
	{
		/* Get host/subnet address from the user */
		io_get_netw(&subnet);
		addr_to_netaddr(&subnet.addr, &n.addr);
		n.nBits = subnet.nBits;
		io_print_netw(&n, &subnet);
	}

	/* Soobnetting time! */
	while (interactive && io_get_subnetting())
	{
		io_get_subnet_range(&n, &firstSub, &lastSub);
		io_print_subnets(&n, firstSub, lastSub);
	}

	return EXIT_SUCCESS;
}