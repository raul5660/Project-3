#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE 64 /* The maximum length command */

void getArguments(char *line, char **argv);
void runArguments(char **argv, bool shouldWait);

int main(void) {
    char args[MAX_LINE];             /* the input line                 */
    char *argv[MAX_LINE];        /* the command line argument      */
    int should_run = 1;           /* flag to determine when to exit program */
    bool shouldIWait = true;

    while (should_run) {
        printf("osh>");
        gets(args);

        if (strstr(args, "&") != NULL) {
            shouldIWait = false;
            int i = strlen(args);
            args[i-2] = '\0';
            args[i-1] = '\0';
        }
        if (strcmp(args, "exit") == 0) {
            return 0;
        }
        else {
            getArguments(args,argv);
            runArguments(argv, shouldIWait);
        }
    }
}

void getArguments(char *line, char **argv) {
    while (*line != '\0') {       /* if not the end of line ....... */
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';     /* replace white spaces with 0    */
        *argv++ = line;          /* save the argument position     */
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++;             /* skip the argument until ...    */
    }
    *argv = '\0';                 /* mark the end of argument list  */
}

void runArguments(char **argv, bool shouldWait) {
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else {                                  /* for the parent:      */
        if(shouldWait) {
            while (wait(&status) != pid) { }       /* wait for completion  */
        }
    }
}