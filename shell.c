#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE 64                 // The maximum length command
#define MAX_COMMANDS 5              // Max size of commands in history

// Prototype
void getArguments(char *line, char **argv);
void runArguments(char **argv, bool shouldWait);
void addToHistory(char args[]);		

// Global
char showHistory [MAX_COMMANDS][MAX_LINE];
int cmdCount = 0;

int main(void) {
    char args[MAX_LINE];                                    // the input line
    char *argv[MAX_LINE];                                   // the command line argument
    bool should_run = true;                                 // flag to determine when to exit program
    bool shouldIWait = true;                                // flag to determine if a parent should wait for child
    int upCmd, i;		 			    					// iterate through history

    while (should_run) {                                    // will run until exit is found
        printf("osh>");                                     // print shell prompt
        //gets(args);                                       // get user input
        scanf("%s", args);

        if (strstr(args, "&") != NULL) {                    // checks if arguments should run in the background
            shouldIWait = false;                            // to be used to in runArguments function to determine whether it should wait for the child
            int i = strlen(args);                           // get the length of the string
            args[i-2] = '\0';                               // remove the white space before &
            args[i-1] = '\0';                               // remote the &
        }
        if (strcmp(args, "exit") == 0) {                    // if the arguments are exit
            should_run = false;                             // change should_run to false
        }

	else if (strncmp(args,"history", 7) == 0) {	    		// if argument is history
			if (cmdCount < MAX_COMMANDS)        
				upCmd = cmdCount;
			else 
				upCmd = MAX_COMMANDS;	    				// only allows max commands
			
			for (i = 0; i < upCmd; i++) {	    			// displays history
				printf("%d \t %s\n", i, showHistory[i]);
			}
			continue;
		}
        else {                                              // else
            getArguments(args,argv);                        // parse arguments
			addToHistory(args);				    			// add to history
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