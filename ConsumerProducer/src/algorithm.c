/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.c (c) 2021
Description: Contains consumer/producer algorithm
Created:  2021-03-17T04:49:09.252Z
Modified: 2021-03-18T05:56:51.335Z
*/
#include "../include/algorithm.h"

static void produce(void* arg)
{
    ConsumerProducer *data = (ConsumerProducer*) arg;
    const int max_items_to_produce = 
        min(data->number_of_items_to_produce, data->buffer_size);
    const int number_of_items_to_consume = 
        min(data->number_of_items_to_consume, data->buffer_size);
    const int buffer_size = data->buffer_size;
    sem_t *mutex = &data->mutex;
    sem_t *empty = &data->empty;
    sem_t *full = &data->full;
    sem_t *no_print = &data->no_print;
    int *products = data->products;
    int counter = data->counter;
    int items_to_produce;
    int remaining;
    int produced_counter = 0;
    
    while(data->running)
    {
        //waits for an empty slot to produce
        sem_wait(empty);
        //locks producing and consuming
        //sem_wait(mutex);
        //sets counter to the maximum production capacity
        items_to_produce = max_items_to_produce;
        //checks wether the production capacity is higher than the number
        //of empty spaces. If true, it redefines counter
        counter = data->counter;
        remaining = buffer_size - counter;
        if (max_items_to_produce > remaining)
            items_to_produce = remaining;

        if(items_to_produce > 0)
        {
            sem_wait(mutex);
            //produces an item and increases the full semaphore
            for(int i = counter; i < counter + items_to_produce; i++)
                products[i] = rand() % 1000;
            data->counter += items_to_produce;
            sem_post(mutex);
            
            produced_counter += items_to_produce;
            if(produced_counter >= number_of_items_to_consume)
            {
                for (; produced_counter > 0; 
                        produced_counter -= number_of_items_to_consume)
                    sem_post(full);
                produced_counter = 0;
            }
            sem_wait(no_print);
        }
        //unlocks producing and consuming
        //sem_post(mutex);
    }
    
    data->counter = buffer_size;
    sem_post(full);
    sem_post(mutex);
    sem_post(no_print);
    pthread_exit(NULL);
}

static void consume(void* arg)
{
    ConsumerProducer *data = (ConsumerProducer*) arg;
    const int max_items_to_consume = 
        min(data->number_of_items_to_consume, data->buffer_size);
    const int number_of_items_to_produce = 
        min(data->number_of_items_to_produce, data->buffer_size);
    const int buffer_size = data->buffer_size;
    sem_t *mutex = &data->mutex;
    sem_t *empty = &data->empty;
    sem_t *full = &data->full;
    sem_t *no_print = &data->no_print;
    int *products = data->products;
    int counter = data->counter;
    int items_to_consume;
    int consumed_counter = 0;
    
    while(data->running)
    {
        //waits for an item
        sem_wait(full);
        //locks producing and consuming
        //sem_wait(mutex);
        //sets counter to the maximum consumption capacity
        items_to_consume = max_items_to_consume;
        //checks wether the consumption capacity is higher than the number
        //of available items. If true, it redefines counter
        counter = data->counter;
        if (counter < max_items_to_consume)
            items_to_consume = counter;

        if(items_to_consume > 0)
        {
            sem_wait(mutex);
            //consumes an item and increases the empty semaphore
            for(int i = (counter - items_to_consume); 
                    i < counter; i++)
                products[i] = 0;
            data->counter -= items_to_consume;
            sem_post(mutex);

            consumed_counter += items_to_consume;
            if(consumed_counter >= number_of_items_to_produce)
            {
                for (; consumed_counter > 0; 
                        consumed_counter -= number_of_items_to_produce)
                    sem_post(empty);
                consumed_counter = 0;
            }
            sem_wait(no_print);
        }
        //unlocks producing and consuming
        //sem_post(mutex);
    }

    data->counter = 0;
    sem_post(empty);
    sem_post(mutex);
    sem_post(no_print);
    pthread_exit(NULL);
}

extern inline void create_consumer(pthread_t* thread, ConsumerProducer *data)
{
    return create_thread(thread, &consume, data);
}

extern inline void create_producer(pthread_t* thread, ConsumerProducer *data)
{
    return create_thread(thread, &produce, data);
}