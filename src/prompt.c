/* prompt.c */
/* Input prompt implementation */

#include "prompt.h"

void prompt(char* promptStr, StrParser check, void* arg, void* out)
{
	char buf[PROMPT_BUF_SIZE], err[PROMPT_BUF_SIZE];
	int valid = 0;
	char c;

	while(!valid)
	{
		fputs(promptStr, stdout);
		fgets(buf, sizeof(buf), stdin);

		if (buf[strlen(buf)-1] != '\n') /* flush stdin if still filled */
			while((c = getchar()) != '\n' && c != EOF);

		/* Validate the user input */
		valid = check(buf, err, arg, out);
		if (!valid) fputs(err, stdout);
	}
}