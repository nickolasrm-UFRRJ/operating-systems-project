/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
data.h (c) 2021
Description: Header file for struct.c
Created:  2021-03-17T04:49:19.877Z
Modified: 2021-04-06T04:04:59.516Z
*/

#ifndef DATA
#define DATA

#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <math.h>

#define NULL_VALUE -10

sem_t full;
sem_t empty;
sem_t mutex;
sem_t no_print;

int* products;
int* last_consumed;
int* last_produced;

int counter;

int buffer_size;
int number_of_items_to_produce;
int number_of_items_to_consume;
bool running;

void alloc_consumer_producer(int buffer_size, int number_of_items_to_consume, 
                                        int number_of_items_to_produce);
void free_consumer_producer();

#endif