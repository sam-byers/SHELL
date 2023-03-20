#include <stdlib.h>
#include <stdio.h>  /* for printf */
#include <string.h> /* for strlen */
#include <unistd.h> /*for read and write and pipe*/
#include <sys/types.h>
#include <limits.h>

#define _GNU_SOURCE
#define promptchar '#'

int main()
{
    FILE *termpoint;
    char cwd[100 * sizeof(char)];
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
    char *token, *savepointer;
    char delim[2] = {' ', '\0'};
    char * synthargv[100];
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
        for (int j = 0;; j++, dataEnt = NULL)
        {
            token = __strtok_r(dataEnt, delim, &savepointer);
            if (token == NULL)
            {
                printf("\n");
                printf("%c", promptchar);
                break;
            }
            printf("\n%d:%s", j, token);
            strcpy(synthargv[j],token);
            printf("\n %s",synthargv[j]);
        }
        execvp(cwd, synthargv);
    }
    return 0;
}