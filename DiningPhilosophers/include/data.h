/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
data.h (c) 2021
Description: Header file for data.c
Created:  2021-03-18T17:29:04.835Z
Modified: 2021-04-06T23:48:02.885Z
*/

#ifndef DATA
#define DATA

#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>

typedef int philosopher;
#define THINKING 0
#define EATING 1
#define HUNGRY 2

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

void alloc_dining_philosophers(int number_of_philosophers, 
    int thinking_time, int eating_time);
void free_dining_philosophers();
void increase_eating_philosophers_quantity();
void increase_thinking_philosophers_quantity();

#endif
