#include "functions.h"
#include <stdlib.h> /* for standard library functions*/
#include <stdio.h>  /* for printf */
#include <string.h> /* for strlen */
#include <unistd.h> /*for read and write and pipe*/
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

static int skipsignal = 0; // if this is 1, do not print the prompt, global variable
static time_t timedata;    // initalise the timedata variable
static struct tm *info;    // and the time info struct that will hold the timedata

void getCWD()
{
    char cwd[100];                        // current working directory array, limited to 100 chars
    if (getcwd(cwd, sizeof(cwd)) != NULL) // call getcwd
    {
        printf("Current working dir: %s\n", cwd); // print CWD
    }
    else
    {
        perror("getcwd() error"); // if there is an error with getcwd print an error
        exit(1);
    }
}

void updatetime() // update the time and print the prompt
{
    time_t timedata;                           // initalise the timedata variable
    struct tm *info;                           // and the time info struct that will hold the timedata
    char buffer[80];                           // make an 80 character buffer (plenty big)
    time(&timedata);                           // update the timedata
    info = localtime(&timedata);               // and write data to the info struct in many formats with localtime (witchcraft)
    strftime(buffer, 80, "%d/%m %H:%M", info); // Define the format that we want to print the timedata, write this to the buffer array
    printf("[%s]%c ", buffer, promptchar);     // print the prompt for user input in the correct format with the prompt char
}

void signalhandle(int signum) // handle the signal, flush the input and output, print a new line, skipsignal high
{
    fflush(stdout);
    fflush(stdin);
    skipsignal = 1;
}

int skipcheck() // Checks if a skip needs to occur when a signal is encountered, returns 1 if yes
{
    if (skipsignal == 1) // if we need to skip
    {
        skipsignal = 0; // reset the skipsignal
        return 1;
    }
    else
        return 0;
}