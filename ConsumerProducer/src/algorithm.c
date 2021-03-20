/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.c (c) 2021
Description: Contains consumer/producer algorithm
Created:  2021-03-17T04:49:09.252Z
Modified: 2021-03-20T15:42:36.748Z
*/
#include "../include/algorithm.h"

static inline int get_real_items_to_consume(ConsumerProducer* data)
{
    return min(data->buffer_size, data->number_of_items_to_consume);
}

static inline int get_real_items_to_produce(ConsumerProducer* data)
{
    return min(data->buffer_size, data->number_of_items_to_produce);
}

static inline int process_semaphore(int produced_counter,
                                    int produced_items,
                                    int max_items_to_consume,
                                    sem_t* full)
{
    produced_counter += produced_items;
    if(produced_counter >= max_items_to_consume)
        for (; produced_counter > 0; 
                produced_counter -= max_items_to_consume)
            sem_post(full);
            
    return produced_counter;
}

static inline void finish_thread(ConsumerProducer* data, 
        sem_t* semaphore_to_unlock)
{
    sem_post(&data->no_print);
    sem_post(semaphore_to_unlock);
    pthread_exit(NULL);
}

static inline void print_while_running(ConsumerProducer *data)
{
    sem_t *no_print = &data->no_print;
    while(data->running)
        sem_wait(no_print);
}




static inline int produce_item()
{
    return rand() % 1000;
}

static inline int calculate_items_to_produce(ConsumerProducer* data, 
                                            int max_items_to_produce)
{
    int remaining = data->buffer_size - data->counter;
    return min(max_items_to_produce, remaining);
}

static inline void produce_items(ConsumerProducer* data, int items_to_produce)
{
    int counter = data->counter;
    int *products = data->products;
    //stats
    int *last_produced = data->last_produced;
    int last_produced_index = 0;
    for(int i = counter; 
            i < counter + items_to_produce; i++, last_produced_index++)
    {
        products[i] = produce_item();
        last_produced[last_produced_index] = products[i];
    }
    data->counter += items_to_produce;
}




static void produce(void* arg)
{
    ConsumerProducer *data = (ConsumerProducer*) arg;
    const int max_items_to_produce = get_real_items_to_produce(data);
    const int max_items_to_consume = get_real_items_to_consume(data);
    sem_t *mutex = &data->mutex;
    sem_t *empty = &data->empty;
    sem_t *full = &data->full;
    sem_t *no_print = &data->no_print;
    int items_to_produce = 0;
    int produced_counter = 0;
    
    if(max_items_to_produce > 0)
        if(max_items_to_consume < 1)
        {
            while(data->counter < data->buffer_size && data->running)
            {
                sem_wait(empty);
                items_to_produce = calculate_items_to_produce(data, max_items_to_produce);
                produce_items(data, items_to_produce);
                sem_wait(no_print);
            }
            print_while_running(data);
        }
        else
        {
            while(data->running)
            {
                sem_wait(empty);
                sem_wait(mutex);
                items_to_produce = calculate_items_to_produce(data, max_items_to_produce);
                produce_items(data, items_to_produce);
                sem_post(mutex);
                produced_counter = process_semaphore(produced_counter, items_to_produce, 
                                    max_items_to_consume, full);
                sem_wait(no_print);
            }
            finish_thread(data, full);
        }
    else
        print_while_running(data);
        
    pthread_exit(NULL);
}




static inline void consume_item(int* item)
{
    item[0] = 0;
}

static inline int calculate_items_to_consume(ConsumerProducer* data, 
                                            int max_items_to_consume)
{
    return min(data->counter, max_items_to_consume);
}

static inline void consume_items(ConsumerProducer* data, int items_to_consume)
{
    int counter = data->counter;
    int *products = data->products;
    //stats
    int *last_consumed = data->last_consumed;
    int last_consumed_index = 0;
    for(int i = (counter - items_to_consume); 
            i < counter; i++, last_consumed_index++)
    {
        last_consumed[last_consumed_index] = products[i];
        products[i] = 0;
    }
    data->counter -= items_to_consume;
}

static void consume(void* arg)
{
    ConsumerProducer *data = (ConsumerProducer*) arg;
    const int max_items_to_consume = get_real_items_to_consume(data);
    const int max_items_to_produce = get_real_items_to_produce(data);
    sem_t *no_print = &data->no_print;

    if(max_items_to_consume > 0)
        if(max_items_to_produce > 0)
        {
            sem_t *mutex = &data->mutex;
            sem_t *empty = &data->empty;
            sem_t *full = &data->full;
            int *products = data->products;
            int counter = data->counter;
            int items_to_consume = 0;
            int consumed_counter = 0;
            
            while(data->running)
            {
                sem_wait(full);
                sem_wait(mutex);
                items_to_consume = calculate_items_to_consume(data, max_items_to_consume);
                consume_items(data, items_to_consume);
                sem_post(mutex);
                consumed_counter = process_semaphore(consumed_counter, items_to_consume, 
                                        max_items_to_produce, empty);
                sem_wait(no_print);
            }
            finish_thread(data, empty);
        }
    
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