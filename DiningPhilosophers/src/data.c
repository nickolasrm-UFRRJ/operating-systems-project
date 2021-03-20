/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
data.c (c) 2021
Description: Contains functions related to DiningPhilosophers data
Created:  2021-03-18T17:52:31.475Z
Modified: 2021-03-20T19:06:16.758Z
*/

#include "../include/data.h"

extern void alloc_dining_philosophers(int philosophers_quantity_, 
                                int thinking_time_, int eating_time_)
{
    philosophers_quantity = philosophers_quantity_;
    thinking_time = thinking_time_;
    eating_time = eating_time_;
    running = true;

    philosophers = (philosopher*) malloc(sizeof(philosopher) *
                                              philosophers_quantity);
    thinking_philosophers_quantity = philosophers_quantity;
    eating_philosophers_quantity = 0;

    next_philosopher_index = philosophers_quantity-1;
    sem_init(&mutex, 0, 1);

    chopsticks = (sem_t*) malloc(sizeof(sem_t) * philosophers_quantity);
    for(int i = 0; i < philosophers_quantity; i++)
    {
        philosophers[i] = THINKING;
        sem_init(chopsticks+i, 0, 1);
    }
}

extern void free_dining_philosophers()
{
    for(int i = 0; i < philosophers_quantity; i++)
        sem_destroy(chopsticks+1);
    sem_destroy(&mutex);
}

extern void increase_eating_philosophers_quantity()
{
    eating_philosophers_quantity++;
    thinking_philosophers_quantity--;
}

extern void increase_thinking_philosophers_quantity()
{
    eating_philosophers_quantity--;
    thinking_philosophers_quantity++;
}