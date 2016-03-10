#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE 64 /* The maximum length command */

bool getArguments(char *line, char **argv);
void runArguments(char **argv, int shouldIWait);

int main(void) {
    char args[MAX_LINE];             /* the input line                 */
    char *argv[MAX_LINE];        /* the command line argument      */
	int should_run = 1;           /* flag to determine when to exit program */


	while (should_run) {
		printf("osh>");
		gets(args);
        bool containsAmpersand = getArguments(args, argv);
		if (strcmp(argv[0], "exit") == 0) {
			should_run = 0;
		} else if (containsAmpersand) {
            runArguments(argv, 0);
        } else {
            runArguments(argv, 1);
        }
	}
    fflush(stdout);
	return 0;
}

bool getArguments(char *line, char **argv) {
    bool ampersand = false;
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n') {
            *line++ = '\0';
            if (*line == '&')
            {
                ampersand = true;
                *line = '\0';
            }
        }
        *argv++ = line;
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n') {
            if (*line == '&')
            {
                ampersand = true;
                *line = '\0';
            } else {
                line++;
            }
        }
    }
    *argv = '\0';
    return ampersand;
}

void runArguments(char **argv, int shouldIWait) {
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    } else {                                  /* for the parent:      */
        if (shouldIWait != 0) {
            while (wait(&status) != pid) { }       /* wait for completion  */
        }
    }
}