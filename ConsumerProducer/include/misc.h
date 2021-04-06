/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
misc.h (c) 2021
Description: Header file for misc.c
Created:  2021-03-17T05:29:12.716Z
Modified: 2021-04-06T03:23:31.204Z
*/

#ifndef MISC
#define MISC

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

#define SLEEP 1
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define is_odd(a) (a % 2 == 1 ? true : false)

pthread_t* alloc_threads(int number);
void create_thread(pthread_t*, void*, void*);
void join_thread(pthread_t*);
void join_threads(pthread_t *threads, int number);
long timestamp_ms();
void init_int_array(int *arr, int length, int value);

int msleep(long tms);

#endif