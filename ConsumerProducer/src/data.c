/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
data.c (c) 2021
Description: Contains consumer/producer struct and its management functions
Created:  2021-03-17T04:49:09.252Z
Modified: 2021-04-06T03:27:31.535Z
*/
#include "../include/data.h"
#include "../include/misc.h"

extern void alloc_consumer_producer(int buffer_size_, int number_of_items_to_consume_, 
                                        int number_of_items_to_produce_)
{
    buffer_size = buffer_size_;
    number_of_items_to_consume = number_of_items_to_consume_;
    number_of_items_to_produce = number_of_items_to_produce_;

    products = (int*) malloc(sizeof(int) * buffer_size);

    int last_produced_length = min(buffer_size, number_of_items_to_produce);
    last_produced = (int*) malloc(sizeof(int) * last_produced_length);
    init_int_array(last_produced, last_produced_length, NULL_VALUE);

    int last_consumed_length = min(buffer_size, number_of_items_to_consume);
    last_consumed = (int*) malloc(sizeof(int) * last_consumed_length);
    init_int_array(last_consumed, last_consumed_length, NULL_VALUE);

    counter = 0;

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, (int) ceil((double) buffer_size / number_of_items_to_produce));
    sem_init(&mutex, 0, 1);
    sem_init(&no_print, 0, 0);

    running = true;
}

extern void free_consumer_producer()
{
    free(products);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);
    sem_destroy(&no_print);
}
