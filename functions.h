// Function Declarations:

typedef struct
{
  char* buffer[10][20];
  int currentcmd;
} history_t;

typedef struct
{
  char* buffer[1000][20];
  int currentjob;
} jobList;

	void printCommand (history_t* history);
	char* concatenate (char* array[], char* commandList);
	void add_spaces(char *dest, int num_of_spaces);
	void redirect (char* file);
	void piping (char* piping_args[]);
	char* retrieveCommand (history_t* history, int i);
	void printJobs (jobList* jobs);
	void printHistory (history_t* history);

