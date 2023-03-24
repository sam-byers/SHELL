/*--------------------------------------------*/
/* This file contains all the functions that  */
/* are used in the main.c file                */
/*
- © - Sam Byers - 2023 - sam.byers@ucdconnect.ie - 21315391
- https://github.com/sam-byers/SHELL
- VERSION 1.0 - [23 - 3 - 23]                 */
/*--------------------------------------------*/

#include "functions.h" //include function prototypes and include path

// Static variables
static time_t timedata; // initalise the timedata variable
static struct tm *info; // and the time info struct that will hold the timedata

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
    printf("\n");
    updatetime();
    fflush(stdout);
    fflush(stdin);
}

int CustomFunction(char *command[]) // Process custom functions CD and exit, return values to determine subsiquent behavior
{
    int DirError = 0;                    // Define Directory Error integer
    if (strcmp(command[0], "exit") == 0) // if the command is exit, exit
        return 2;                        // we should now exit
    if (strcmp(command[0], "cd") == 0)   // if the command is CD
    {
        if (command[1] != NULL) // If we have an arguemnt for the CD command
        {
            DirError = chdir(command[1]); // Attempt to change the directory
            if (DirError == -1)           // If there is an error
            {
                fprintf(stderr, "cd: %s: ", command[1]); // print to stderr
                perror("");                              // Print the error using perror (witchcraft)
            }
        }
        else // otherwise we dont have input arguments for CD
        {
            printf("\nNo argument given for directory!\nChanging directory to home\n");
            DirError = chdir(getenv("HOME")); // So we set the directory to the HOME directory
            if (DirError == -1)               // If there is an error
            {
                fprintf(stderr, "cd: %s: ", getenv("HOME")); // print to stderr
                perror("");                                  // Print the error using perror (witchcraft)
            }
        }
        return 1; // command is custom and cannot be execvp'ed
    }
    return 0; // command is not custom
}

int ChildExe(char *command[], int customstat) // Execute the command in a child process, choose the return value in error and custom cases
{
    int status; // define the status variable
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
    return 0;   //This should never be hit, but is here as a precaution
}