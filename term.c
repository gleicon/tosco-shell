#include "term.h"

//static inline void recvsig (int sig)
//{
//	signal (sig, recvsig);
//	return;
//}

static inline void keyparse (const char *key, char *p, int len)
{
	fprintf (stderr, "%s\n", key);
}

void initterm (str_t *s)
{
//	signal (SIGINT, recvsig);
//	signal (SIGTSTP, recvsig);

	memset (s, 0, sizeof (str_t));

	tcgetattr (0, &s->oldtio);
	memcpy (&s->newtio, &s->oldtio, sizeof (struct termios));
	s->newtio.c_iflag &= ~(IXON|IXOFF|INLCR|ICRNL|ISTRIP|BRKINT);
	s->newtio.c_lflag &= ~(ICANON|ECHO);
	tcsetattr (0, TCSANOW, &s->newtio);

	return;
}

void finishterm (str_t *s)
{
	tcsetattr (0, TCSANOW, &s->oldtio);
}

int getstr (char *line, int linelen, str_t *s)
{
	char ch, *p;
	int len = 0;

	p = line;
	while (read (0, &ch, 1) > 0) {
		/* backspace */
		if (ch == '\177' && len != 0) {
			write (1, "\b", 1);
			write (1, " ", 1);
			ch = '\b';
			
			len--;
			*p-- = ' ';
		} else if ((ch == '\177' && len == 0) ||
			   (len == (linelen - 1) && ch != '\r')) {
			/* beep */
			ch = '\7';
		} else if (ch == '\r' || ch == '\n') { /* end of line */
			p[++len] = ch;
			write (1, &ch, 1);
			break;
		} else if (ch == '\33') { /* other keys */
			char newch[3];
			newch[0] = newch[1] = newch[2] = '\0';
			read (0, newch, sizeof (newch));

			ch = '\0';
		} else {
			*p++ = ch;
			len++;
		}

		write (1, &ch, 1);
	}

	p[len] = '\0';
	return len;
}
