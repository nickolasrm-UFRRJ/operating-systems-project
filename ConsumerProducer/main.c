/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
main.c (c) 2021
Description: Main file for consumer producer algorithm
Created:  2021-03-17T17:02:27.486Z
Modified: 2021-03-19T04:47:28.928Z
*/

#include "include/struct.h"
#include "include/algorithm.h"
#include "include/gui.h"
#include <ncurses.h>

int main(int argc, char const *argv[])
{
    if(argc > 3)
    {
        int buffer_size = atoi(argv[1]);
        if(buffer_size > 0)
        {
            int items_to_consume = atoi(argv[2]);
            int items_to_produce = atoi(argv[3]);
            if(items_to_consume < 0)
            { items_to_produce -= items_to_consume; items_to_consume = 0;}
            if(items_to_produce < 0)
            { items_to_consume -= items_to_produce; items_to_produce = 0;}
            
            ConsumerProducer *data = alloc_consumer_producer(atoi(argv[1]), 
                items_to_consume, items_to_produce);

            pthread_t threads[3];
            create_gui(threads, data);
            create_consumer(threads+1, data);
            create_producer(threads+2, data);

            join_threads(threads, 3);
            
            free_consumer_producer(data);
            return 0;
        }
        else
            printf("Not allowed buffer size less than 1\n");
    }
    else
        printf("Please, add more arguments for this software\n");
    return 1;
}
