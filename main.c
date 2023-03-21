#include <stdlib.h>
#include <stdio.h>  /* for printf */
#include <string.h> /* for strlen */
#include <unistd.h> /*for read and write and pipe*/
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#define _GNU_SOURCE
#define promptchar '#'

void updatetime(time_t timedata, struct tm *info) // update the time and print the prompt
{
    char buffer[80];                           // make an 80 character buffer (plenty big)
    time(&timedata);                           // update the timedata
    info = localtime(&timedata);               // and write data to the info struct in many formats with localtime (witchcraft)
    strftime(buffer, 80, "%d/%m %H:%M", info); // Define the format that we want to print the timedata, write this to the buffer array
    printf("[%s]%c ", buffer, promptchar);     // print the prompt for user input in the correct format with the prompt char
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
    FILE *termpoint;       // initalise the terminal pointer
    char *filename = NULL; // If writing to a file, this will hold the filename
    time_t timedata;       // initalise the timedata variable
    struct tm *info;       // and the time info struct that will hold the timedata
    char cwd[100];         // current working directory array, limited to 100 chars
    int customstat = 0;    // custom status int, if the command being executed is 'custom', do not execvp
    int status = 0, f;        // the status of the execvp command and the file pointer if we are writing to a file
    char *command[50];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error"); // if there is an error with getcwd print an error
        exit(1);
    }
    char *dataEnt = NULL;
    char *token, *savepointer;   // the token pointer and the pointer that contains the rest of the input string
    char delim[2] = {' ', '\0'}; // the deliminator that is used when tokenising the input string
    termpoint = stdin;           // Termpoint points to the standard input

    if (termpoint == NULL) // If that did not work for whatever reason
    {
        exit(EXIT_FAILURE); // Exit with EXIT_FAILIURE
    }

    size_t len = 0;                                          // will contian the number of chars wrote
    ssize_t nread;                                           // will contain the number of chars read
    updatetime(timedata, info);                              // print the prompt for user input
    while (nread = getline(&dataEnt, &len, termpoint) != -1) // take user input into the dataEnt array
    {
        customstat = 0; // customstat defaults to 0
        if (dataEnt[strlen(dataEnt)] == '\n') // change the newline to a null char
            (dataEnt[strlen(dataEnt)] = '\0');
        
        for (int j = 0;; j++, dataEnt = NULL) // this will run forever, until a null token is read, each loop j++ and dataEnt = NULL
        {
            token = __strtok_r(dataEnt, delim, &savepointer); // tokenise the input, ie get new token
            if (token == NULL)                                // when we hit the last token in the dataEnt pointer, break
                break;
            if (strcmp(token, ">") == 0) // If we have > and not >> in the token
            {
                filename = __strtok_r(dataEnt, delim, &savepointer); // get the filename from the next token
                command[j] = NULL;                                   // set the command to NULL
                break;                                               // break so it never gets to write to
            }
            else
            {
                command[j] = token;    // put the token in place
                command[j + 1] = NULL; // always put a NULL token at the end of what was just saved
            }
        }
        if (filename != NULL) { // If we have a filename supplied
            f = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, 0666); //open the file to copy to
            dup2(f, 1); //duplicate to the file specifed
        }
        if (command[0] != NULL) // if the command is not NULL
        {
            if (strcmp(command[0], "exit") == 0) // if the command is exit, exit
                exit(0);
            if (strcmp(command[0], "cd") == 0) // if the command is CD
            {
                customstat = 1;         // CD is a custom command that cannot be execvp'ed
                if (command[1] != NULL) // If we have an arguemnt for the CD command
                {
                    int DirError = chdir(command[1]); // Attempt to change the directory
                    if (DirError == -1)               // If there is an error
                    {
                        perror("ERROR: "); // Print the error using perror (witchcraft)
                    }
                }
                else // otherwise we dont have input arguments for CD
                {
                    printf("\nNo argument given for directory!\nChanging directory to home\n");
                    chdir(getenv("HOME")); // So we set the directory to the HOME directory
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
            if (filename != NULL)       // if we have a filename
                close(f);               // close the file
            filename = NULL;            // reset the filename to NULL
            updatetime(timedata, info); // print the next prompt for user input
        }
    }

    return 0;
}
