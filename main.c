/*--------------------------------------------
This is a simple shell that uses execvp to execute commands, with cd functionality.
Use on a unix system in the terminal

The shell command './configure && make && make install'
should configure, build, and install this package from the .tar.gz file

It can then be run by the shell command:
./assign2_21315391

To write to a file, just do
$ (your command) > filename
where filename is the name of your desired destination file.

Doing CTRL + C will not exit the shell program
any command before or after CTRL + C will be ingnored


- © - Sam Byers - 2023 - sam.byers@ucdconnect.ie - 21315391
- https://github.com/sam-byers/SHELL
- VERSION 1.0 - [23 - 3 - 23]
-----------------------------------------------*/

#include "functions.h" //include the functions and the include path

int main()
{
    FILE *termpoint; // initalise the terminal pointer
    signal(SIGINT, signalhandle);
    char *filename = NULL; // If writing to a file, this will hold the filename
    int customstat = 0;    // custom status int, if the command being executed is 'custom', do not execvp
    int f, stdoutCPY;      // the file pointer if we are writing to a file
                           // stdoutCPY lets us revert to standard output in terminal after a filewrite
    char *command[50];
    char *dataEnt = NULL;
    char *token, *savepointer;   // the token pointer and the pointer that contains the rest of the input string
    char delim[2] = {' ', '\0'}; // the deliminator that is used when tokenising the input string
    termpoint = stdin;           // Termpoint points to the standard input
    if (termpoint == NULL)       // If termpointer did not work for whatever reason
    {
        exit(EXIT_FAILURE); // Exit with EXIT_FAILIURE
    }
    getCWD();                                                  // Print the current working directory to the terminal
    size_t len = 0;                                            // will contian the number of chars wrote
    ssize_t nread;                                             // will contain the number of chars read
    updatetime();                                              // print the prompt for user input
    while ((nread = getline(&dataEnt, &len, termpoint) != -1)) // take user input into the dataEnt array
    {
        customstat = 0;                           // customstat defaults to 0
        if (dataEnt[strlen(dataEnt) - 1] == '\n') // change the newline to a null char
            (dataEnt[strlen(dataEnt) - 1] = '\0');

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
        if (filename != NULL)
        {
            // If we have a filename supplied
            stdoutCPY = dup(1);                                                // Copy standard output to stdoutCPY
            f = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666); // open the file to copy to
            dup2(f, 1);                                                        // duplicate to the file specifed
            close(f);                                                          // close the file
        }
        if (command[0] != NULL) // if the command is not NULL, this stops a strcmp with a null string (seg. fault)
        {
            customstat = CustomFunction(command); // check if the command is custom, if it is, run the custom function
            if (customstat == 2)                  // if the command was exit, exit
                exit(0);
        }
        if (fork() == 0)
        {
            // If fork returns a zero value, child process
            return ChildExe(command,customstat);
        }
        else
        {
            // If fork returns a non zero value
            // reset the command array to NULL so that pressing enter does not run the last command again
            for (int i = 0; command[i] != NULL; i++)
                command[i] = NULL;
            wait(NULL); // voodoo witchcraft, This blocks the parent process until a child process has completed
            if (filename != NULL)
            {                       // if we have a filename
                dup2(stdoutCPY, 1); // reset the stdout to the original
                close(stdoutCPY);   // close the copy
                filename = NULL;    // reset the filename to NULL
            }
            updatetime(); // print the next prompt for user input
        }
    }
    return 0;
}