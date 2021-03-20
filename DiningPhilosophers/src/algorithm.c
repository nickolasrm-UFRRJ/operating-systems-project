/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
algorithm.c (c) 2021
Description: Contains the dining philosophers algorithm.
Created:  2021-03-18T17:28:18.591Z
Modified: 2021-03-20T19:11:46.840Z
*/

#include "../include/algorithm.h"

static inline int next_chopstick(int chopstick, int chopsticks_quantity)
{
    int next = chopstick + 1;
    if (next == chopsticks_quantity)
        return 0;
    else
        return next;
}

static inline int previous_chopstick(int chopstick, int chopsticks_quantity)
{
    if (chopstick == 0)
        return chopsticks_quantity - 1;
    else
        return chopstick - 1;
}

static void dine(void *arg)
{
    int philosopher_index;

    sem_wait(&mutex);
    philosopher_index = next_philosopher_index;
    next_philosopher_index--;
    sem_post(&mutex);

    int first_chopstick, second_chopstick;
    bool odd = is_odd(philosopher_index);
    if(is_odd(philosopher_index))
    {
        first_chopstick = philosopher_index;
        second_chopstick = next_chopstick(philosopher_index, philosophers_quantity); 
    }
    else
    {
        first_chopstick = previous_chopstick(philosopher_index, philosophers_quantity);
        second_chopstick = philosopher_index;
    }

    while(running)
    {

        sem_wait(chopsticks+first_chopstick);
        sem_wait(chopsticks+second_chopstick);
        
        //eating
        //stats
        philosophers[philosopher_index] = EATING;
        sem_wait(&mutex);
        increase_eating_philosophers_quantity();
        sem_post(&mutex);
        //doing
        sleep(eating_time);

        sem_post(chopsticks+first_chopstick);
        sem_post(chopsticks+second_chopstick);

        //thinking
        //stats
        philosophers[philosopher_index] = THINKING;
        sem_wait(&mutex);
        increase_thinking_philosophers_quantity();
        sem_post(&mutex);
        //doing
        sleep(thinking_time);
    }

    sem_post(chopsticks+philosopher_index);
    pthread_exit(NULL);
}




extern inline void create_philosopher(pthread_t* thread)
{
    return create_thread(thread, &dine, NULL);
}