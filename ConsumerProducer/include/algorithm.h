/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.h (c) 2021
Description: Header file for algorithm.c
Created:  2021-03-17T17:09:24.588Z
Modified: 2021-03-20T18:22:07.503Z
*/

#ifndef ALGORITHM
#define ALGORITHM

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "misc.h"
#include "data.h"

void create_consumer(pthread_t* thread);
void create_producer(pthread_t* thread);

#endif