/* tosco shell exec functions */
/* (c) 1999 gleicon          */
		

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "event_types.h"

//#define DEBUG

																																																																																																																																																									 
extern char **environ;

void exec_ext (char **args);

execute (char **args) {
	
	int i, pido, how_much=0, pipe_pos=0;
	char **tmp_ptr;
	char **envs=environ;
	
	char **left_side, **right_side;
	
	
	#ifdef DEBUG
	char **debug_ptr=args;
	#endif
	
	char **count_args=args;
	int debug_i=0;
	
  	#ifdef DEBUG
	while (*debug_ptr != NULL) fprintf (stdout, "args[%d] - %s\n", debug_i++,*debug_ptr++);
	#endif
	
	/* prevent segz */
	
	if ( *args==NULL ) return;


		
	/* tell me if the first arg is |, <,>,<<,>> (it cant happen) */

	if (!strncmp(args[0],"|",1) || !strncmp (args[0],"<",1) 
	 	|| !strncmp(args[0],">", 1) || !strncmp(args[0],"<<", 1) 
	 	|| !strncmp(args[0],">>", 1) || !strncmp (args[0],".",1)
	 	|| !strncmp(args[0],"..", 1) || !strncmp (args[0],"/",1)) { 
		
		fprintf (stdout,"Syntax error near %s \n", args[0]);
	  	return;
	  	}
	
	/* exit ( this is not the cleanest exit ever, but...) */



	/* tell me the position of the last arg */
	
	while (*count_args != NULL) { 
		how_much++;
		count_args++;
		}
	how_much--;
	
	#ifdef DEBUG
	fprintf (stdout, "argumentos: %d\n", how_much);
	#endif
	
	
	/* tell me the pipe position (if it exists )*/
	
	count_args=args;
	
	while (*count_args != NULL ) {
		if (how_much < 1) {
			pipe_pos=-1;
			break;
			}
		
		if ( strncmp (args[pipe_pos], "|", 1) == NULL ) break;
		count_args++;
		pipe_pos++;
		if (pipe_pos > how_much) {
			pipe_pos = -1;
			break;
			}
		}
	
	
		
	 #ifdef DEBUG
	 fprintf (stdout, "pipe_pos: %d\n", pipe_pos);
	 #endif
	
	
  	
  	/*
   	 * check exit.
   	 */

  
  	if ( *args==NULL ) return;


	/* echo */
   
  	if ( strncmp (args[0], "echo" , 4) == 0) {
  		if (!args[1]) {
  		fprintf (stdout,"\n");
  		return;
  		}
    	fprintf (stdout, "%s\n", args[1]);
  	return;
  	}

	
	/* exit ( this is not the cleanest exit ever, but...) */
	
      
  	if ( strncmp (args[0], "exit" , 4) == 0) {
  		if (event(EXIT_SH)) {
  			put_error ("Error on finish event !!\n");
  			}
  		fprintf (stdout, "Quit. \n");
  		exit ();
  		}

    
	/* chdir - a.k.a. cd */

  	if ( strncmp (args[0], "cd", 2) == 0 ) {
  		if (!args[1]) args[1]=(char *) getuserdir();
   		chdir (args[1]);
  		return;
  		}
 
	/* setenv */

  	if ( strncmp (args[0], "setenv", 6) == 0 ) {
  	  	if ( !args[2] ) {
  			fprintf (stdout, "I need a value to set into it !\n");
  			return;
  			}
  		fprintf (stdout, "%s=%s\n",args[1], args[2]);
  		setenv (args[1], args[2], 1);
  		return;
  		}
  	
  	/* getenv */
  	
  	if ( strncmp (args[0], "getenv", 6) == 0 ) {
 		if (!args[1]) {
			while (*envs != NULL) fprintf (stdout, "%s\n", *envs++);
			return;
			}
		fprintf (stdout, "%s\n", getenv(args[1]));
  		return ;
  		}
  	
	/* go to bg son */
	
  	if ( args[how_much]!=NULL && strncmp (args[how_much], "&", 1) == 0 ) {
  		args[how_much]=NULL;
  		if ((pido=background(args))==-1) {
  			fprintf (stdout, "Err:couldn't sento to bg\n");
  			return;
  			}
  		fprintf (stdout, "[%d]\n",pido);
  		return;
  		}
   
 	/* pipe-o */
 	
 	if (pipe_pos != -1) {
 		args[pipe_pos]=NULL;
 		left_side=args[0];
 		right_side=args[pipe_pos+1];
 		pipeme(left_side, right_side);
 		return;
 		}
 	
 
  	
  	/* built-in exec */
  	
  	if ( strncmp (args[0], "exec", 4) ==0 ) {
  		tmp_ptr=&args[1];
  		exec_ext (tmp_ptr);
  		return;
  		}
  
  // anything else...
  exec_ext (args);
  return;
  }
 

void exec_ext (char **args) {
 
	int pid, status;
  	/*
   	 * Get a child process to execute external commands .
   	 */
  
  	if ((pid = fork ()) < 0) {
  		perror ("fork");
		exit (1);
    		}

  	/*
   	 * The child executes the code inside the if.
   	 */
  
  	 if (pid == 0) {
      		execvp (*args, args);
      		perror (*args);
      		exit (1);
   		}

  
  	/*
   	 * The parent executes the wait.
   	 */
  
  	while (wait (&status) != pid)
    
    	/* empty */ ;
}
