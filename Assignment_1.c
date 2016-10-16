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

#define HISTORY_SIZE 10
#define ARGS_ARRAY_SIZE 20


	/*
	for (int i = 0; i < HISTORY_SIZE; i++)
		{
	         for (int j = 0; j < ARGS_ARRAY_SIZE; j++)
	         {
	         	if (history->buffer[i][j+1] != NULL)
	         	{
	         		printf ("String 1 is %s \n", history->buffer[i][j]);
	         		printf ("String 2 is %s \n", history->buffer[i][j+1]);
	         		//printf ("Concatenated String: %s \n", strcat(history->buffer[i][j], history->buffer[i][j+1]));
	       			//commandList[i] = strcat(history->buffer[i][j], history->buffer[i][j+1]);
	         	}
	         	if ( history->buffer[i][j] != NULL)
	         	{
	        	   printf("buffer[%d][%d] is: %s\n",i, j , history->buffer[i][j]);

	         	}
	         }
		}

		char* commandList = (char*) malloc(100 *sizeof(char));
		//concatenate(testArray, 0 ,commandList);
		for (int i = 0; i < 9; i++)
		{
	         printf("Command %d is %s \n ", i, concatenate(history->buffer[i],0,commandList));
		}
		*/

typedef struct
{
  char* buffer[10][20];
  int currentcmd;
} history_t;

	// Function Declarations:
	void printCommand (history_t* history);
	char* concatenate (char* array[], char* commandList);
	void add_spaces(char *dest, int num_of_spaces);

history_t* history;


	char* concatenate (char* array[], char* com)
	{
		int i = 0;
		while (array[i] != NULL && array[i+1]!= NULL)
		{
			printf("Elements of array %s\n", array[i] );
			strcat(com, array[i]);
			i++;
		}
		
		// for (int i = 0; i < 4; i++)
		// {
		// 	strcat(commandList, array[i]);
		// }
					
	}

	void printCommand (history_t* history)
	{
		char* commandList = (char*) malloc(1000 *sizeof(char));
		printf("Concatenated Array is %s \n",concatenate(history->buffer[0], commandList));

	}
	//
// This code is given for illustration purposes. You need not include or follow this
// strictly. Feel free to writer better or bug free code. This example code block does not
// worry about deallocating memory. You need to ensure memory is allocated and deallocated
// properly so that your shell works without leaking memory.
//
int getcmd(char *prompt, char *args[], int *background, char* line1)
{
	 int length, i = 0;
	 char *token, *loc;
	 char *line = NULL;
	 size_t linecap = 0;
	 printf("%s", prompt);
	 length = getline(&line, &linecap, stdin);
	 line1 = line;
	 if (length <= 0) {
	 exit(-1);
 }
 // Check if background is specified..
	 if ((loc = index(line, '&')) != NULL)
	 {
	   *background = 1;
	   *loc = ' ';
	 }
	 if ((loc = index(line, '>')) != NULL)
	 {
	   printf("%s\n", "> detected" );
	   //execvp();
	 }
	 if ((loc = index(line, '|')) != NULL)
	 {
		   printf("%s\n", "| detected" );
	 }
	 else
	 *background = 0;
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

// void printin (history_t* his)
// {
//            char* commandList = (char*) malloc(100 *sizeof(char));
// 		   concatenate(his->buffer[0], 0 ,commandList);
// 	       printf("%s\n", commandList);
// }

int main(void)
{
	 char *args[20];
	 int bg;
	 char* wordir;
	 char* l;
	 history_t* history = (history_t*) malloc(sizeof(history_t));
	 history->currentcmd = 0;
	 pid_t pids[1000];
	 while(1)
	 {
	   bg = 0;
	   int cnt = getcmd("\n>> ", args, &bg, l);
	   // add method here that copies commands to history array
	   args[cnt] = NULL;

	   for (int i = 0; i < cnt; i++)
	   {
	     history->buffer[history->currentcmd][i*2] =  args[i];
	     history->buffer[history->currentcmd][i*2+1] = " ";
	     printf("history->buffer[history->currentcmd][%d] is %s \n",i*2,  history->buffer[history->currentcmd][i*2]);
	     printf("history->buffer[history->currentcmd][%d] is %s \n",i*2 +1,  history->buffer[history->currentcmd][i*2 +1]);

	   }
	   history->currentcmd = history->currentcmd + 1;
	   //printCommand(history);
	   //printf("currentcmd is %d\n", history->currentcmd );


	   pid_t pid = fork();

	   if (pid==0)
	   {
	     //child
	     execvp(args[0],args);
	     exit(1);
	   }
	   else
	   {
	    if(strcmp("jobs",args[0])==0){
	        int i;
	        int counter = 0;
	        int *status;
	        status = malloc(1000);
	        for(i=0;pids[i]!=0;i++){
	            pid_t r = waitpid(pids[i],status,1);
	            if(r!=-1){
	                printf("%d,\n",r);
	                counter++;
	            }
	        }
	        //enterExec = 0;

	    }
	   	 if (strcmp(args[0],"exit")==0)
	     {
	         exit(0);
	     }
	     if (strcmp(args[0],"pwd")==0)
	     {
	         printf("%s\n", getcwd(wordir,150));
	     }
	     if (strcmp(args[0],"cd")==0)
	     {
	         chdir(args[1]);
	     }
	     if (strcmp(args[0],"history")==0)
	     {
	       history->currentcmd = history->currentcmd - 1;
	       printCommand(history);
	     }
	     // parent
	   }
	   /* the steps can be..:
	  (1) fork a child process using fork()
	  (2) the child process will invoke execvp()
	  (3) if background is not specified, the parent will wait,
	  otherwise parent starts the next command... */
	 }
}
