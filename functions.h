#ifndef TIME_T_DEFINED
#define TIME_T_DEFINED
typedef long time_t; //define time_t as long
#endif
/*include the libs!*/
#include <stdlib.h>    /* for standard library functions*/
#include <stdio.h>     /* for printf */
#include <string.h>    /* for strlen */
#include <unistd.h>    /*for read and write and pipe*/
#include <sys/types.h> /*For unix system data types*/
#include <sys/wait.h>  /*for the wait command */
#include <limits.h>    /*determines propreties of var types*/
#include <time.h>      /*for printing the time in the terminal*/
#include <signal.h>    /*for catching sigint*/
#include <fcntl.h>     /*for writing the output to a file*/

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define _GNU_SOURCE
#define promptchar '#'

int skipcheck(); // Checks if a skip needs to occur when a signal is encountered, returns 1 if yes

void updatetime(); // update the time and print the prompt

void signalhandle(int signum); // handle the signal, flush the input and output, print a new line, skipsignal high

void getCWD(); // get the current working directory and print it to the terminal

#endif //FUNCTIONS.H INCLUDED