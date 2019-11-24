#include <stdio.h>
extern char **environ;

int main ()
{
	char **envs = environ;

	while (*envs != NULL)
		fprintf (stderr, "%s\n", *envs++);

	return 0;
}
