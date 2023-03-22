#ifndef TIME_T_DEFINED
#define TIME_T_DEFINED
typedef long time_t; //define time_t as long
#endif


#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define _GNU_SOURCE
#define promptchar '#'

int skipcheck(); // Checks if a skip needs to occur when a signal is encountered, returns 1 if yes

void updatetime(); // update the time and print the prompt

void signalhandle(int signum); // handle the signal, flush the input and output, print a new line, skipsignal high

void getCWD(); // get the current working directory and print it to the terminal

#endif //FUNCTIONS.H INCLUDED