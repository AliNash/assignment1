/* Ali Alnashashibi
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

#define HISTORY_SIZE 10
#define ARGS_ARRAY_SIZE 20

typedef struct
{
  char* buffer[10][20];
  int currentcmd;
} history_t;

typedef struct
{
  char* buffer[10][20];
  int currentjob;
} jobList;

	// Function Declarations:
	void printCommand (history_t* history);
	char* concatenate (char* array[], char* commandList);
	void add_spaces(char *dest, int num_of_spaces);
	void redirect (char* file);
	void piping (char* piping_args[]);
	char* retrieveCommand (history_t* history, int i);


history_t* history;
jobList* jobs;


	char* concatenate (char* array[], char* com)
	{
		int i = 0;
		while (array[i] != NULL && array[i+1]!= NULL)
		{
			//printf("Elements of array %s\n", array[i] );
			strcat(com, array[i]);
			i++;
		}
    return com;
	}

	char* retrieveCommand (history_t* history, int i)
	{
	      char* historyCommand = (char*) malloc(1000 *sizeof(char));
	      return concatenate(history->buffer[(i-1)%10], historyCommand);
	}

	void printHistory (history_t* history)
	{
	    for (int i = (history->currentcmd - (history->currentcmd%10)); i < history->currentcmd; i++)
	    {
	      char* historyList = (char*) malloc(1000 *sizeof(char));
	      printf("Command %d is %s \n",i+1, concatenate(history->buffer[i%10], historyList));
	    }
	}

	void printJobs (jobList* jobs)
	{
		printf("Inside printing jobs \n");
	    for (int i = (jobs->currentjob - (jobs->currentjob%10)); i < jobs->currentjob; i++)
	    {
	      char* jobList = (char*) malloc(1000 *sizeof(char));
	      printf("Command %d is %s \n",i+1, concatenate(jobs->buffer[i%10], jobList));
	    }
	}

	void redirect (char* file)
	{
		// printf("I'm in redirect \n");
		close(1);
		open (file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		printf("file is %s", file);
	}

	void piping (char* piping_args[])
	{
		pid_t ppid; 
		int pipefdes[2]; //pipe file descriptor

		pipe(pipefdes);
		printf("inside piping function\n");
		printf("piping_args[0] is %s\n", piping_args[0]);

		if((ppid = fork()) == 0){
			dup2(pipefdes[0], 0); 
			close(pipefdes[1]); 
			execvp(piping_args[0], piping_args);
		}
		else{
			dup2(pipefdes[1], 1);
			close(pipefdes[0]); 
		}
	}

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
	    printf(" Background value is set to 1 \n" );
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
	 int hisflag;
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

	   // reseting the values for the operators
	   bg = 0;
	   hisflag = 0;
	   outputflag = 0;
	   pipingflag = 0;

	   int cnt = getcmd("\n>> ", args, &bg, &hisflag, &outputflag, &pipingflag, l);
	   // add method here that copies commands to history array
	   args[cnt] = NULL;
	   printf("%d\n", bg);

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
			     jobs->buffer[(jobs->currentjob%10)][i*2] =  args[i];
			     jobs->buffer[(jobs->currentjob%10)][i*2+1] = " ";
			   }
			   	 jobs->currentjob = jobs->currentjob + 1;
		    printf("\n Background enabled \n\n");

           // add to background list
       }

	   pid_t pid = fork();

	   if (pid == 0)
	   {
	   	if (hisflag)
	   	{
	   		execvp(retrieveCommand (history, atoi(args[0])),args);
	   	}
	   	int i = 0;
	   	if (outputflag)
        {
	    	printf("Detected outputflag\n");
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
        	printf("in piping flag\n");
       		args[i] = NULL; 
       		piping(args+i+1); 
        }
	     execvp(args[0],args);
	     exit(1);
	   }
	   else
	   {
	   	 // parent
	   	if (bg)
        {
       	 //printf("history command");
        } 
       // the parent process waits until the child finishes
        else
        {
           // printf("\n Background not enabled \n\n");
           waitpid(pid,&status,0);
        }
	   	 if (strcmp(args[0],"exit")==0)
	     {
	         exit(0);
	     }
	     // if (strcmp(args[0],"pwd")==0)
	     // {
	     //     printf("%s\n", getcwd(wordir,150));
	     // }
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
	     	printf("Inside jobs comparison \n");
	       jobs->currentjob = jobs->currentjob;
	       printJobs(jobs);
	     }
	     // parent
	   }
	 }
}