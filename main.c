#include <stdlib.h>
#include <stdio.h>  /* for printf */
#include <string.h> /* for strlen */
#include <unistd.h> /*for read and write and pipe*/
#include <sys/types.h>

#define _GNU_SOURCE
#define promptchar '#'

int main()
{
    FILE *termpoint;

    int end = 0;
    int pid;
    char *dataEnt = NULL;
    char *token, *savepointer;
    char *delim = {' ','\0'};
    termpoint = stdin;

    if (termpoint == NULL)
    {
        exit(EXIT_FAILURE);
    }

    size_t len = 0;
    ssize_t nread;
    printf("%c", promptchar);
    while (nread = getline(&dataEnt, &len, termpoint) != -1)
    {
        printf("%c", promptchar);
        for (int i = 0; i < strlen(dataEnt); i++)
        {
            if (dataEnt[i] == '\n')
                (dataEnt[i] = '\0');
        }
        // user will press enter at the end of the string
        token = __strtok_r(dataEnt, delim, &savepointer);
        pid = fork();
        if (pid == 0)
        {
            printf("%s", token);
        }
        else
        {
            while (token != NULL)
            {
                token = __strtok_r(NULL,delim, &savepointer);
            }
        }
    }

    return 0;
}
