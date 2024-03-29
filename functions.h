/*--------------------------------------------*/
/*This header file contains all the included libs*/
/*and the function prototypes for the functions */
/*that are used in the main.c file             */
/*
- © - Sam Byers - 2023 - sam.byers@ucdconnect.ie - 21315391
- https://github.com/sam-byers/SHELL
- VERSION 1.0 - [23 - 3 - 23]                 */
/*--------------------------------------------*/

#ifndef TIME_T_DEFINED
#define TIME_T_DEFINED
typedef long time_t; // define time_t as long
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

int CustomFunction(char *command[]);
// Process custom functions CD and exit

int ChildExe(char *command[], int customstat);
// Execute the command in a child process, choose the return value in error and custom cases

void updatetime();
// update the time and print the prompt

void signalhandle(int signum);
// handle the signal, flush the input and output, print a new line, skipsignal high

void getCWD();
// get the current working directory and print it to the terminal

#endif // FUNCTIONS.H INCLUDED