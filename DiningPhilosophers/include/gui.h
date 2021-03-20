/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.h (c) 2021
Description: Header file for gui.c
Created:  2021-03-17T18:15:30.839Z
Modified: 2021-03-19T05:39:46.613Z
*/

#ifndef GRAPHICS
#define GRAPHICS

#include "struct.h"
#include "misc.h"
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#define TABLE_BLOCK_GREEN 1
#define TABLE_BLOCK_RED 2
#define TABLE_BLOCK_SIZE 2

struct _GUI
{
    DiningPhilosophers *dinner;

    int width;
    int height;

    int table_scroll;

    WINDOW *table;
    WINDOW *constants;
    WINDOW *variables;
}; typedef struct _GUI GUI;

void create_gui(pthread_t* thread, DiningPhilosophers *data);

#endif