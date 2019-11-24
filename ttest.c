#include "term.h"

int main ()
{
	int llen;
	static str_t sd;
	static char line[TERM_BUFLEN];

	initterm (&sd);
	
	for (;;) {
		memset (line, 0, sizeof (line));
		llen = getstr (line, sizeof (line), &sd);
		if (!strncmp (line, "exit", 4)) break;

		fprintf (stderr, "[%d] %s\n", llen, line);
	}

	finishterm (&sd);
	return 0;
}
