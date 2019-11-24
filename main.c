/* tosco shell main  */
/* (c) 1999 gleicon, alex fiori  */

#include "tosco.h"
									  /* cc -o fork fork.c */

void main () {
  
 // char buf[1024];
 // char *args[64];
 // char pwd[128];
  
  /* initialize term and stuffs */
  
  if (event (INIT_SH)) {
  	put_error("Error on init event !! Givin' up . \n");
  	exit (0);
  	}
  
  
  init_banner();
   
  signal (SIGINT, recvsig);
//  signal (SIGTSTP, recvsig);
  endless_loop();
    
}

void endless_loop(){
    
  char buf[1024];
  char *args[64];
  char pwd[128];
  char curr[512];
	
  for (;;)
    {
      /*
       * Prompt for and read a command.
       */
      
      // user@host[pwd] : 
      printf ("%s@%s[%s]: ", getusername() , gethost(), getcwd(pwd, sizeof(pwd)) );
      memset (curr,0,sizeof(curr));
      sprintf (curr,"%s@%s[%s]: ", getusername() , gethost(), getcwd(pwd, sizeof(pwd)) );
      set_xterm_title(curr);
         
      
      if (gets (buf) == NULL)
	{
	  printf ("\n");
	  exit (0);
	}

      /*
       * Split the string into arguments.
       */
      
      parse (buf, args);

      /*
       * Execute the command.
       */
      
      execute (args);
    }
}
