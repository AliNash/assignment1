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

	// This function will concatenate all the values of an array and return a char*
	// This is mainly used to retrieve the commands and run them
	char* concatenate (char* array[], char* com)
	{
		int i = 0;
		while (array[i] != NULL && array[i+1]!= NULL)
		{
			strcat(com, array[i]);
			i++;
		}
    return com;
	}

	// retreives the desired command from the history using index i
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
	    for (int i = 0; i < jobs->currentjob; i++)
	    {
	      char* jobList = (char*) malloc(1000 *sizeof(char));
	      printf("Command %d is %s \n",i+1, concatenate(jobs->buffer[i%10], jobList));
	    }
	}

	void redirect (char* file)
	{
		close(1);
		open (file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		printf("file is %s", file);
	}

	void piping (char* piping_args[])
	{
		pid_t ppid; 
		int pipefdes[2]; //pipe file descriptor

		pipe(pipefdes);

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