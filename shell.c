#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE 64                 // The maximum length command
#define MAX_COMMANDS 10             // Max size of commands in history

// Prototype
void getArguments(char *line, char **argv);
void runArguments(char **argv, bool shouldWait);
void addToHistory(char args[]);

// Global
char showHistory [MAX_COMMANDS][MAX_LINE];
int cmdCount = 0;

int main(void) {
    char args[MAX_LINE];                                    // the input line
    char tmpArgs[MAX_LINE];
    char *argv[MAX_LINE];                                   // the command line argument
    bool should_run = true;                                 // flag to determine when to exit program
    bool shouldIWait = true;                                // flag to determine if a parent should wait for child
    int upCmd;		 			    					// iterate through history

    while (should_run) {                                    // will run until exit is found
        printf("osh>");                                     // print shell prompt
        gets(args);                                         // get user input
        strcpy(tmpArgs,args);

        if (strstr(args, "!") != NULL) {
            if (cmdCount < MAX_COMMANDS) {
                upCmd = cmdCount;
            } else {
                upCmd = MAX_COMMANDS;                        // only allows max commands
            }

            if (strstr(args, "!!") != NULL) {
                strcpy(args, showHistory[upCmd - 1]);
		        strcpy(tmpArgs,args);
		}
	     else {                                             // allows user to choose from history list

		if(strstr(args, "!1")) {                            // history  list option 1
			strcpy(args, showHistory[0]);
			strcpy(tmpArgs,args);
		} else if(strstr(args, "!2")) {                     // history  list option 2
			strcpy(args, showHistory[1]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!3")) {                     // history  list option 3
            strcpy(args, showHistory[2]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!4")) {                     // history  list option 4
            strcpy(args, showHistory[3]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!5")) {                     // history  list option 5
            strcpy(args, showHistory[4]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!6")) {                     // history  list option 6
            strcpy(args, showHistory[5]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!7")) {                     // history  list option 7
            strcpy(args, showHistory[6]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!8")) {                     // history  list option 8
            strcpy(args, showHistory[7]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!9")) {                     // history  list option 9
            strcpy(args, showHistory[8]);
            strcpy(tmpArgs,args);
        } else if(strstr(args, "!10")) {                    // history  list option 10
            strcpy(args, showHistory[9]);
            strcpy(tmpArgs,args);
        }

            }
            printf("[%s]\n",args);
            //strcpy(tmpArgs,args);
        }
        if (strstr(args, "&") != NULL) {                    // checks if arguments should run in the background
            shouldIWait = false;                            // to be used to in runArguments function to determine whether it should wait for the child
            int i = strlen(args);                           // get the length of the string
            args[i-2] = '\0';                               // remove the white space before &
            args[i-1] = '\0';                               // remote the &
        }
        if (strcmp(args, "exit") == 0) {                    // if the arguments are exit
            should_run = false;                             // change should_run to false
        }

	    else if (strncmp(args,"history", 7) == 0) {            // if argument is history
            addToHistory("history");                           // adds "history" command to history list
            if (cmdCount < MAX_COMMANDS) {
                upCmd = cmdCount;
            } else {
                upCmd = MAX_COMMANDS;                        // only allows max commands
            }
			for (int i = upCmd-1; i >= 0; i--) {	    			// displays history
				printf("%d \t %s\n", i+1, showHistory[i]);
			}
			continue;
		}
        else {                                              // else
            getArguments(args,argv);                        // parse arguments
			addToHistory(tmpArgs);				    			// add to history
            runArguments(argv, shouldIWait);                // run arguments
        }
        shouldIWait = true;                                 // change shouldIWait back to true
    }
    fflush(stdin);
    return 0;
}

void getArguments(char *line, char **argv) {
    while (*line != '\0') {                                 // if not the end of line
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';                                 // replace white spaces with 0
        *argv++ = line;                                     // save the argument position
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++;                                         // skip the argument until
    }
    *argv = '\0';                                           // mark the end of argument list
}

void runArguments(char **argv, bool shouldWait) {
    pid_t  pid;                                             // variable for process id
    int    status;                                          // integer for status of child process

    if ((pid = fork()) < 0) {                               // fork a child process
        printf("*** ERROR: forking child process failed\n");// print error message if forking fails
        exit(1);
    }
    else if (pid == 0) {                                    // for the child process
        if (execvp(*argv, argv) < 0) {                      // execute the command
            printf("*** ERROR: exec failed\n");             // print error message if execution of commands fails
            exit(1);
        }
    }
    else {                                                  // for the parent
        if(shouldWait) {                                    // should the parent wait
            while (wait(&status) != pid) { }                // wait for completion
        }
    }
}

void addToHistory(char args[]) {
	// char array to hold history
	char history[MAX_COMMANDS][MAX_LINE];

	// iterate args
	int i = 0;
	
	// add the command to history
	strcpy(history[cmdCount % MAX_COMMANDS], args);
	
	// add the display command to history
	while (args[i] != '\n' && args[i] != '\0') {
		showHistory[cmdCount % MAX_COMMANDS][i] = args[i];
		i++;
	}
	showHistory[cmdCount % MAX_COMMANDS][i] = '\0';
	
	++cmdCount;

	return;
}
