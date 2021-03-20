/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.h (c) 2021
Description: Header file for algorithm.c
Created:  2021-03-18T17:28:51.723Z
Modified: 2021-03-20T19:04:04.508Z
*/

#ifndef ALGORITHM
#define ALGORITHM

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "data.h"
#include "misc.h"

void create_philosopher(pthread_t* thread);

#endif
