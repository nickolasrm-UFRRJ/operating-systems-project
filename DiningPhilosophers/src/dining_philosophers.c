/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
dining_philosophers.c (c) 2021
Description: Contains dining philosophers implementation source using pthreads
Created:  2021-03-17T03:50:28.157Z
Modified: 2021-03-17T04:52:46.958Z
*/

struct _DiningPhilosophers
{
    bool *semaphores;
    int philosophers_number;
    int thinking_time;
    int eating_time;
}; typedef struct _DiningPhilosophers DiningPhilosophers;

DiningPhilosophers *dining_allocate(int philosophers_number, int thinking_time, int eating_time)
{
    DiningPhilosophers *data = (DiningPhilosophers*) malloc(sizeof(DiningPhilosophers));

    data->philosophers_number = philosophers_number;
    data->eating_time = eating_time;
    data->thinking_time = thinking_time;

    data->semaphores = (bool*) malloc(sizeof(bool) * philosophers_number);

    return data;
}

void dining_algorithm(DiningPhilosophers *data)
{
    for(int i = 0; i < data->philosophers_number; i++)
    {
        
    }
}

void dining_philosophers(int philosophers_number, int thinking_time, int eating_time)
{
    bool quit = false;
    DiningPhilosophers *data = dining_allocate(philosophers_number, thinking_time, eating_time);
    while(!quit)
    {
        dining_algorithm();
        //dining_update_graphics();
        //quit = dining_quit_input();
    }
}