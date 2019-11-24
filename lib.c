/* tosco shell lib functions */
/* (c) 1999 gleicon          */

									  /* cc -o fork fork.c */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <strings.h> // memset ()

#define DEBUG


void recvsig (int sig) {
	fprintf (stdout,"\n");
	signal (sig, recvsig);
	endless_loop();
	return;
	}
	
	

void put_error(char *mens) {
	fprintf (stdout,"[ERROR] - %s \n",mens);
	}

void put_debug(char *mens) {
	fprintf (stdout,"[DEBUG] - %s \n",mens);
	}
 	
char *getusername() {

	static struct passwd *ent;
	static int cur_uid=-1;
	
	
	if (getuid()==cur_uid) { 
		return ent->pw_name;
		}
	if (cur_uid==-1) cur_uid=getuid();
	
	setpwent();
	ent=getpwuid(cur_uid);
	endpwent();
	
	if (!ent) return "(null)";
			
	return ent->pw_name;
	
	}

char *getuserdir() {

	static struct passwd *ent;
	static int cur_uid=-1;
	
	
	if (getuid()==cur_uid) { 
		return ent->pw_dir;
		//return "~";
		}
	
	if (cur_uid==-1) cur_uid=getuid();
	
	setpwent();
	ent=getpwuid(cur_uid);
	endpwent();
	
	if (!ent) {
//		fprintf (stdout, "whoo\n");
		
		return "/"; // a std dir
		}	
	return ent->pw_dir;
//	return "~";
	
	
	}

	 
char *gethost() {
	static char nome[100];	
	
	if (gethostname (nome, 12)) return "(null)";
	return nome;
	 
}
	
void set_xterm_title (char *wot) {
	if (!strncmp (getenv("TERM"), "xter",4)) {
		fprintf(stdout, "%c]0;%s%c", '\033', wot, '\007');
	        }
	}

	                               
void init_banner() {
	fprintf (stdout,"Tosco Shell v 0.00001 - _extended_\n\n");
	set_xterm_title ("Tosco Shell");
	}


																																																																																	     /*
																																																																																	        * parse--split the command in buf into
																																																																																	        *         individual arguments.
																																																																																	      */
parse (char *buf, char **args)  {
	
	while (*buf != (long) NULL) {
      
      		/*
       		 * Strip whitespace.  Use nulls, so
       		 * that the previous argument is terminated
       		 * automatically.
       		 */
      
      		 while ((*buf == ' ') || (*buf == '\t')) *buf++ = (long) NULL;

      		/*
       		 * Save the argument.
       		 */
      
      		 *args++ = buf;

      		/*
       		 * Skip over the argument.
       		 */
      
      		 while ((*buf != (long) NULL) && (*buf != ' ') && (*buf != '\t')) 
      		 	buf++;
    		}

  	*args = NULL;
	
	}

int background (char **args) {
	int pid, status;
        /*
         * send to bg
         */
         
         if ((pid = fork()) < 0) {
         	perror("fork");
                return(-1);
		}
	 
	 if (pid == 0) {
	 	execvp(*args, args);
	  	perror(*args);
	  	return(-1);
	        }
	 
	 return (pid);
	 
	 }




int pipeme (char **left_side, char **right_side) {
	
	FILE *in, *out;
	unsigned char line[1024];
	char lefto[512], righto[512];
	char **tmp;
	int pos_l=0, pos_r=0, temp=0;
	
	put_debug("Starting pipme\n");

	memset (lefto,0,512);
	memset (righto,0,512);
	
	
	tmp=left_side;
	
	while (*tmp != NULL) {
		pos_l++;
		tmp++;
		}
	pos_l--;
	
	
	put_debug("pos_l ok\n");
	fprintf (stdout,"pos_l == %d\n%s \n", pos_l, left_side);
	
	tmp=right_side;
	
	
	while (*tmp != NULL) {
		put_debug("inside pos_r\n");
		pos_r++;
		tmp++;
		}
	pos_r--;
		
	put_debug("pos_r ok\n");
	fprintf (stdout,"pos_r == %d\n%s \n", pos_r, right_side);
	
	
//	for (temp=0; temp = pos_l; temp++) {
		put_debug("init strcat\n");
		strcat (lefto, left_side);
		put_debug("end strcat\n");
//		strcat (lefto," ");
//		}
		
//	for (temp=0; temp = pos_r; temp++) {
		strcat (righto, right_side);
		put_debug("strcat\n");
//		strcat (righto," ");
//		}
	 	
	#ifdef DEBUG
	fprintf (stdout, "l side:%s\n", lefto);
	fprintf (stdout, "r side:%s\n", righto);
	
//	return ;
	#endif
	
	
		
	in = popen (lefto, "r");
	out = popen (righto, "w");

	while (!feof (in)) {
		memset (line, 0, sizeof (line));
		fread (line, sizeof (line), 1, in);
		fwrite (line, sizeof (line), 1, out);
	}

	fclose (in); fclose (out);

	return 0;
}
