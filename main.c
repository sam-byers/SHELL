#include "functions.h" //include the functions

int main()
{
    FILE *termpoint; // initalise the terminal pointer
    signal(SIGINT, signalhandle);
    char *filename = NULL;        // If writing to a file, this will hold the filename
    int customstat = 0;           // custom status int, if the command being executed is 'custom', do not execvp
    int status = 0, f, stdoutCPY; // the status of the execvp command and the file pointer if we are writing to a file
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
bump:                                                          // Goto if we have a SIGINT
    updatetime();                                              // print the prompt for user input
    while ((nread = getline(&dataEnt, &len, termpoint) != -1)) // take user input into the dataEnt array
    {
        if (skipcheck() == 1)
            goto bump;                            // If SIGINT then bump
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
                        fprintf(stderr, "cd: %s: ", command[1]); //print to stderr
                        perror(""); // Print the error using perror (witchcraft)
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