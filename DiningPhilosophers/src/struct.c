/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
struct.c (c) 2021
Description: Contains functions related to DiningPhilosophers data structure
Created:  2021-03-18T17:52:31.475Z
Modified: 2021-03-19T05:03:18.327Z
*/

#include "../include/struct.h"

extern DiningPhilosophers* alloc_dining_philosophers(int philosophers_quantity, 
                                int thinking_time, int eating_time)
{
    DiningPhilosophers* data = (DiningPhilosophers*) malloc(sizeof(DiningPhilosophers));
    
    data->philosophers_quantity = philosophers_quantity;
    data->thinking_time = thinking_time;
    data->eating_time = eating_time;
    data->running = true;

    data->philosophers = (philosopher*) malloc(sizeof(philosopher) *
                                                    philosophers_quantity);
    data->thinking_philosophers_quantity = philosophers_quantity;
    data->eating_philosophers_quantity = 0;

    data->next_philosopher_index = philosophers_quantity-1;
    sem_init(&data->mutex, 0, 1);

    data->chopsticks = (sem_t*) malloc(sizeof(sem_t) * philosophers_quantity);
    for(int i = 0; i < philosophers_quantity; i++)
    {
        data->philosophers[i] = THINKING;
        sem_init(data->chopsticks+i, 0, 1);
    }

    return data;
}

extern void free_dining_philosophers(DiningPhilosophers* data)
{
    for(int i = 0; i < data->philosophers_quantity; i++)
        sem_destroy(data->chopsticks+1);
    sem_destroy(&data->mutex);
    free(data);
}

extern void increase_eating_philosophers_quantity(DiningPhilosophers* data)
{
    data->eating_philosophers_quantity++;
    data->thinking_philosophers_quantity--;
}

extern void increase_thinking_philosophers_quantity(DiningPhilosophers* data)
{
    data->eating_philosophers_quantity--;
    data->thinking_philosophers_quantity++;
}