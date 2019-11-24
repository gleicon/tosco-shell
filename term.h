#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#define TERM_BUFLEN 1024

typedef struct {
	struct termios oldtio;
	struct termios newtio;
} str_t;

extern void initterm (str_t *);
extern void finishterm (str_t *s);
extern int getstr (char *, int, str_t *);
