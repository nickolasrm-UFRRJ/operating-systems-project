/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
main.c (c) 2021
Description: Main file for consumer producer algorithm
Created:  2021-03-17T17:02:27.486Z
Modified: 2021-03-20T19:09:28.532Z
*/

#include "include/data.h"
#include "include/algorithm.h"
#include "include/gui.h"
#include <ncurses.h>

int main(int argc, char const *argv[])
{
    if(argc > 3)
    {
        const int philosophers_quantity_ = atoi(argv[1]);
        const int threads_quantity = philosophers_quantity_+1;
        if(philosophers_quantity_ > 1)
        {
            const int thinking_time_ = atoi(argv[2]);
            const int eating_time_ = atoi(argv[3]);
            if (thinking_time_ > -1 && eating_time_ > -1)
            {
                alloc_dining_philosophers(philosophers_quantity_, 
                                    thinking_time_, eating_time_);
                pthread_t threads[threads_quantity];
                
                for(int i = 0; i < philosophers_quantity; i++)
                    create_philosopher(threads+i);
                create_gui(threads+philosophers_quantity);

                join_threads(threads, threads_quantity);
                
                free_dining_philosophers();
                return 0;
            }
            else
                printf("Eating or thinking times must be equal or greater than 0");
        }
        else
            printf("Less than 1 philosopher is not allowed\n");
    }
    else
        printf("Please, add more arguments for this software\n");
        
    return 1;
}
