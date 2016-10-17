/*  
	Done by: Ali Alnashashibi
	Student ID Number: 260520965
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>
#include "functions.h"

#define HISTORY_SIZE 10
#define ARGS_ARRAY_SIZE 20

	// Creating instances for history and jobs
	// to store all the commands
	history_t* history;
	jobList* jobs;

int getcmd(char *prompt, char *args[], int *bg, int *historyflag, int *outputflag, int *pipingflag, char* line1)
{
	 int length, i = 0;
	 char *token, *loc;
	 char *line = NULL;
	 size_t linecap = 0;
	 printf("%s", prompt);
	 int flag = 0;
	 length = getline(&line, &linecap, stdin);
	 line1 = line;
	 if (length <= 0) {
	 exit(-1);
 }
 // Check if background is specified..
	 if ((loc = index(line, '&')) != NULL)
	 {
	   *bg = 1;
	   *loc = ' ';
	 }
	 if ((loc = index(line, '!')) != NULL)
	 {
	   *historyflag = 1;
	   *loc = ' ';
	 }
	 if ((loc = index(line, '>')) != NULL)
	 {
	   *outputflag = 1;
	 }
	 if ((loc = index(line, '|')) != NULL)
	 {
	   *pipingflag = 1;
	 }
	 while ((token = strsep(&line, " \t\n")) != NULL) {
	   for (int j = 0; j < strlen(token); j++)
	     if (token[j] <= 32)
	     token[j] = '\0';
	     if (strlen(token) > 0)
	     // adding the arguments to the history
	     args[i++] = token;
	 }
	 return i;
}

int main(void)
{
	 char *args[20];

	 // Flags to detect the opeartors
	 int bg;
	 int historyflag;
	 int outputflag;
	 int pipingflag;
	 int fd;

	 // File Descriptors:
	 int old_stdout;

	 char* wordir;
	 char* l;
   	 int status;
	 // Initializing the history parameters:
	 history_t* history = (history_t*) malloc(sizeof(history_t));
	 history->currentcmd = 0;

	 // Initializing the job parameters:
	 jobList* jobs = (jobList*) malloc(sizeof(jobList));
	 jobs->currentjob = 0;

	 pid_t pids[1000];
	 while(1)
	 {
	   // Initializing all flags to zero
	   bg = 0;
	   historyflag = 0;
	   outputflag = 0;
	   pipingflag = 0;

	   int cnt = getcmd("\n", args, &bg, &historyflag, &outputflag, &pipingflag, l);
	   
	   // add this to make sure it's a null-terminated array
	   args[cnt] = NULL;

	   // Storing commands in history buffer
	   for (int i = 0; i < cnt; i++)
	   {
	     history->buffer[(history->currentcmd%10)][i*2] =  args[i];
	     history->buffer[(history->currentcmd%10)][i*2+1] = " ";
	   }
	   history->currentcmd = history->currentcmd + 1;
	   if (bg)
       {
       		for (int i = 0; i < cnt; i++)
			   {
			     jobs->buffer[(jobs->currentjob)][i*2] =  args[i];
			     jobs->buffer[(jobs->currentjob)][i*2+1] = " ";
			   }
			   	 jobs->currentjob = jobs->currentjob + 1;
           		// add to job list
       }

	   pid_t pid = fork();

	   if (pid == 0)
	   {
	   	// Child process runs here
	   	if (historyflag)
	   	{
	   		execvp(retrieveCommand (history, atoi(args[0])),args);
	   	}
	   	int i = 0;
	   	if (outputflag)
        {
	    	old_stdout = dup(1);
			  while (strcmp(args[i], ">") != 0)
			  {
			  	i++;
			  }
			  redirect(args[i+1]);
			  args[i] = NULL;
    	}
    	if (pipingflag)
        {
        	while (strcmp(args[i], "|") != 0)
			  {
			  	i++;
			  }
       		args[i] = NULL; 
       		piping(args+i+1); 
        }
	     execvp(args[0],args);
	     exit(1);
	   }
	   else
	   {
	   	 // Parent process runs here
	     // checking if we need to run it in the background
	   	if (bg)
        {
        	printf("%d", bg);
        	waitpid(pid,&status,0);
        } 
        else
        {
	   	 if (strcmp(args[0],"exit")==0)
	     {
	         exit(0);
	     }
	     if (strcmp(args[0],"cd")==0)
	     {
	         chdir(args[1]);
	     }
	     if (strcmp(args[0],"history")==0)
	     {
	       history->currentcmd = history->currentcmd - 1;
	       printHistory(history);
	     }
	     if (strcmp(args[0],"jobs")==0)
	     {
	       jobs->currentjob = jobs->currentjob;
	       printJobs(jobs);
	     }
	     // parent
	   }
	 }
   }
}