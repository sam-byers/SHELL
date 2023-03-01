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
    char *token, *savepointer, *str;
    char delim[2] = {' ', '\0'};
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
            str[i] = dataEnt[i];
        }
        for (int j = 1;; str = NULL)
        {
            token = __strtok_r(dataEnt, delim, &savepointer);
            if(token == NULL) break;
            printf("\n%d:%s",j,token);

        }
    }

    return 0;
}
