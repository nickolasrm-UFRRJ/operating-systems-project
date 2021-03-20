/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
struct.h (c) 2021
Description: Header file for struct.c
Created:  2021-03-18T17:29:04.835Z
Modified: 2021-03-19T05:03:08.026Z
*/

#ifndef STRUCT
#define STRUCT

#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>

typedef bool philosopher;
#define THINKING false
#define EATING true

struct _DiningPhilosophers
{
    int philosophers_quantity;
    int thinking_time;
    int eating_time;
    
    //stats
    philosopher* philosophers;
    int thinking_philosophers_quantity;
    int eating_philosophers_quantity;

    sem_t* chopsticks;
    sem_t mutex;
    int next_philosopher_index;

    bool running;
}; typedef struct _DiningPhilosophers DiningPhilosophers;

DiningPhilosophers* alloc_dining_philosophers(int number_of_philosophers, 
    int thinking_time, int eating_time);
void free_dining_philosophers(DiningPhilosophers* data);
void increase_eating_philosophers_quantity(DiningPhilosophers* data);
void increase_thinking_philosophers_quantity(DiningPhilosophers* data);

#endif
