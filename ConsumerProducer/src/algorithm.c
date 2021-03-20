/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.c (c) 2021
Description: Contains consumer/producer algorithm
Created:  2021-03-17T04:49:09.252Z
Modified: 2021-03-20T18:35:08.796Z
*/
#include "../include/algorithm.h"

static inline int get_real_items_to_consume()
{
    return min(buffer_size, number_of_items_to_consume);
}

static inline int get_real_items_to_produce()
{
    return min(buffer_size, number_of_items_to_produce);
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

static inline void finish_thread(sem_t* semaphore_to_unlock)
{
    sem_post(&no_print);
    sem_post(semaphore_to_unlock);
    pthread_exit(NULL);
}

static inline void print_while_running()
{
    while(running)
        sem_wait(&no_print);
}




static inline int produce_item()
{
    return rand() % 1000;
}

static inline int calculate_items_to_produce(int max_items_to_produce)
{
    int remaining = buffer_size - counter;
    return min(max_items_to_produce, remaining);
}

static inline void produce_items(int items_to_produce)
{
    //stats
    int last_produced_index = 0;
    for(int i = counter; 
            i < counter + items_to_produce; i++, last_produced_index++)
    {
        products[i] = produce_item();
        last_produced[last_produced_index] = products[i];
    }
    counter += items_to_produce;
}




static void produce(void* arg)
{
    const int max_items_to_produce = get_real_items_to_produce();
    const int max_items_to_consume = get_real_items_to_consume();
    int items_to_produce = 0;
    int produced_counter = 0;
    
    if(max_items_to_produce > 0)
        if(max_items_to_consume < 1)
        {
            while(counter < buffer_size && running)
            {
                sem_wait(&empty);
                items_to_produce = calculate_items_to_produce(max_items_to_produce);
                produce_items(items_to_produce);
                sem_wait(&no_print);
            }
            print_while_running();
        }
        else
        {
            while(running)
            {
                sem_wait(&empty);
                sem_wait(&mutex);
                items_to_produce = calculate_items_to_produce(max_items_to_produce);
                produce_items(items_to_produce);
                sem_post(&mutex);
                produced_counter = process_semaphore(produced_counter, items_to_produce, 
                                    max_items_to_consume, &full);
                sem_wait(&no_print);
            }
            finish_thread(&full);
        }
    else
        print_while_running();
        
    pthread_exit(NULL);
}




static inline void consume_item(int* item)
{
    item[0] = 0;
}

static inline int calculate_items_to_consume(int max_items_to_consume)
{
    return min(counter, max_items_to_consume);
}

static inline void consume_items(int items_to_consume)
{
    //stats
    int last_consumed_index = 0;
    for(int i = (counter - items_to_consume); 
            i < counter; i++, last_consumed_index++)
    {
        last_consumed[last_consumed_index] = products[i];
        products[i] = 0;
    }
    counter -= items_to_consume;
}

static void consume(void* arg)
{
    const int max_items_to_consume = get_real_items_to_consume();
    const int max_items_to_produce = get_real_items_to_produce();

    if(max_items_to_consume > 0)
        if(max_items_to_produce > 0)
        {
            int items_to_consume = 0;
            int consumed_counter = 0;
            
            while(running)
            {
                sem_wait(&full);
                sem_wait(&mutex);
                items_to_consume = calculate_items_to_consume(max_items_to_consume);
                consume_items(items_to_consume);
                sem_post(&mutex);
                consumed_counter = process_semaphore(consumed_counter, items_to_consume, 
                                        max_items_to_produce, &empty);
                sem_wait(&no_print);
            }
            finish_thread(&empty);
        }
    
    pthread_exit(NULL);
}

extern inline void create_consumer(pthread_t* thread)
{
    return create_thread(thread, &consume, NULL);
}

extern inline void create_producer(pthread_t* thread)
{
    return create_thread(thread, &produce, NULL);
}