/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
struct.c (c) 2021
Description: Contains consumer/producer struct and its management functions
Created:  2021-03-17T04:49:09.252Z
Modified: 2021-03-19T07:47:20.775Z
*/
#include "../include/struct.h"
#include "../include/misc.h"

extern ConsumerProducer* alloc_consumer_producer(int buffer_size, int number_of_items_to_consume, 
                                        int number_of_items_to_produce)
{
    
    ConsumerProducer *data = (ConsumerProducer*) malloc(sizeof(ConsumerProducer));

    data->buffer_size = buffer_size;
    data->number_of_items_to_consume = number_of_items_to_consume;
    data->number_of_items_to_produce = number_of_items_to_produce;

    data->products = (int*) malloc(sizeof(int) * buffer_size);
    data->last_produced = (int*) malloc(sizeof(int) * 
        min(buffer_size, number_of_items_to_produce));
    data->last_consumed = (int*) malloc(sizeof(int) * 
        min(buffer_size, number_of_items_to_consume));
    data->counter = 0;

    sem_init(&data->full, 0, 0);
    sem_init(&data->empty, 0, (int) ceil((double) buffer_size / number_of_items_to_produce));
    sem_init(&data->mutex, 0, 1);
    sem_init(&data->no_print, 0, 0);

    data->running = true;

    return data;
}

extern void* free_consumer_producer(ConsumerProducer* data)
{
    free(data->products);
    sem_destroy(&data->full);
    sem_destroy(&data->empty);
    sem_destroy(&data->mutex);
    sem_destroy(&data->no_print);
    
    free(data);
    
    return NULL;
}
