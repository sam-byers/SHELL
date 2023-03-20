#include <stdlib.h>
#include <stdio.h>  /* for printf */
#include <string.h> /* for strlen */
#include <unistd.h> /*for read and write and pipe*/
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <signal.h>

#define _GNU_SOURCE
#define promptchar '#'

void updatetime(time_t timedata, struct tm *info) // update the time and print the prompt
{
    char buffer[80];
    time(&timedata);
    info = localtime(&timedata);
    strftime(buffer, 80, "%d/%m %H:%M", info);
    printf("[%s]%c ", buffer, promptchar);
}

void signalhandle(int signum) // handle the signal, flush the input and output, return carrage
{
    fflush(stdout);
    fflush(stdin);
    printf("\r");
}

int main()
{
    signal(SIGINT, signalhandle);
    FILE *termpoint;    // initalise the terminal pointer
    time_t timedata;    // initalise the timedata variable
    struct tm *info;    // and the time info struct that will hold the timedata
    char cwd[100];      // current working directory array, limited to 100 chars
    int customstat = 0; // custom status int, if the command being executed is 'custom', do not execvp
    int status;         // the status of the execvp command
    char *command[50];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        exit(1);
    }
    int end = 0;
    int pid;
    char *dataEnt = NULL;
    char *token, *savepointer;   // the token pointer and the pointer that contains the rest of the input string
    char delim[2] = {' ', '\0'}; // the deliminator that is used when tokenising the input string
    termpoint = stdin;

    if (termpoint == NULL)
    {
        exit(EXIT_FAILURE);
    }

    size_t len = 0;
    ssize_t nread;
    updatetime(timedata, info);
    while (nread = getline(&dataEnt, &len, termpoint) != -1) // take user input into the dataEnt array
    {
        customstat = 0;                           // customstat defaults to 0
        for (int i = 0; i < strlen(dataEnt); i++) // change the newline to a null char
        {
            if (dataEnt[i] == '\n')
                (dataEnt[i] = '\0');
        }
        for (int j = 0;; j++, dataEnt = NULL)
        {
            token = __strtok_r(dataEnt, delim, &savepointer);

            if (token == NULL) // when we hit the last token in the dataEnt pointer print a newline
            {
                printf("\n");
                break;
            }
            command[j] = token;    // put the token in place
            command[j + 1] = NULL; // command must be terminated with a null pointer
        }
        if (command[0] != NULL)
        {
            if (strcmp(command[0], "exit") == 0) // if the command is exit, exit
                exit(0);
            if (strcmp(command[0], "cd") == 0) // if the command is CD
            {
                customstat = 1;
                if (command[1] != NULL)
                {
                    chdir(command[1]);
                }
                else
                {
                    printf("\nNo argument given for directory!\nChanging directory to home\n");
                    chdir(getenv("HOME"));
                }
            }
        }
        if (fork() == 0)
        {
            // If fork returns a zero value, child process

            if (customstat == 0)
                status = execvp(command[0], command); // exe the command, this will kill the child
            else
            {
                status = 1; // status is 1 if the command is custom, assume succuess of command
                return 0;   // kill the child
            }
            if (status == -1) // if command failed
            {
                printf("Invalid command: %s\n", command[0]); // print invalid command
                return 1;                                    // kill the child
            }
        }
        else
        {
            // If fork returns a non zero value
            // reset the command array to NULL so that pressing enter does not run the last command again
            for (int i = 0; command[i] != NULL; i++)
                command[i] = NULL;
            wait(NULL);                 // voodoo witchcraft, This blocks the parent process until a child process has completed
            updatetime(timedata, info); // print the next prompt for user input
        }
    }

    return 0;
}