/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.h (c) 2021
Description: Header file for gui.c
Created:  2021-03-17T18:15:30.839Z
Modified: 2021-04-06T03:22:06.624Z
*/

#ifndef GRAPHICS
#define GRAPHICS

#include "data.h"
#include "misc.h"
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#define MAX_LIST_ITEMS 6
#define BASE_SLEEP 500
#define SLEEP_STEP 25
#define MIN_SLEEP (SLEEP_STEP)
#define MAX_SLEEP (SLEEP_STEP*20)

struct _GUI
{
    int width;
    int height;

    WINDOW *progressbar;
    WINDOW *constants;
    WINDOW *variables;

    WINDOW *last_produced;
    WINDOW *last_consumed;

    int max_produced_list_items;
    int max_consumed_list_items;
    int produced_list_scroll;
    int consumed_list_scroll;

    long sleep_time;
}; typedef struct _GUI GUI;

void create_gui(pthread_t* thread);

#endif