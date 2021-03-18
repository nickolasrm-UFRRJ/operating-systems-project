/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
main.c (c) 2021
Description: Main file for consumer producer algorithm
Created:  2021-03-17T17:02:27.486Z
Modified: 2021-03-18T06:49:07.001Z
*/

#include "include/struct.h"
#include "include/algorithm.h"
#include "include/gui.h"
#include <ncurses.h>

int main(int argc, char const *argv[])
{
    if(argc > 3)
    {
        ConsumerProducer *data = alloc_data(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

        pthread_t threads[3];
        create_gui(threads, data);
        create_consumer(threads+1, data);
        create_producer(threads+2, data);

        join_threads(threads, 3);
        
        free_data(data);
        return 0;
    }
    else
    {
        printf("Please, add more arguments for this software\n");
        return 1;
    }
}
