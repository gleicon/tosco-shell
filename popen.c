#include <stdio.h>
#include <string.h> // memset ()

int pipeme (char *left_side, char *right_side)
{
	FILE *in, *out;
	unsigned char line[1024];

	in = popen (left_side, "r");
	out = popen (right_side, "w");

	while (!feof (in)) {
		memset (line, 0, sizeof (line));
		fread (line, sizeof (line), 1, in);
		fwrite (line, sizeof (line), 1, out);
	}

	fclose (in); fclose (out);

	return 0;
}
