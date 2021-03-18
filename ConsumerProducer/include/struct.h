/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
struct.h (c) 2021
Description: Header file for struct.c
Created:  2021-03-17T04:49:19.877Z
Modified: 2021-03-18T01:03:40.779Z
*/

#ifndef STRUCT
#define STRUCT

#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <math.h>

struct _ConsumerProducer
{
    sem_t full;
    sem_t empty;
    sem_t mutex;
    sem_t no_print;

    int* products;
    int counter;
    
    int buffer_size;
    int number_of_items_to_produce;
    int number_of_items_to_consume;
    bool running;
}; typedef struct _ConsumerProducer ConsumerProducer;

ConsumerProducer* alloc_data(int buffer_size, int number_of_items_to_consume, 
                                        int number_of_items_to_produce);
void* free_data(ConsumerProducer* data);

#endif