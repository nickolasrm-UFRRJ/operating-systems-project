/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.h (c) 2021
Description: Header file for gui.c
Created:  2021-03-17T18:15:30.839Z
Modified: 2021-04-07T03:03:05.067Z
*/

#ifndef GRAPHICS
#define GRAPHICS

#include "data.h"
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
#define TABLE_BLOCK_BLUE 3
#define TABLE_BLOCK_SIZE 2

struct _GUI
{
    int width;
    int height;

    int table_scroll;

    bool color_while_thinking;

    WINDOW *table;
    WINDOW *constants;
    WINDOW *variables;
}; typedef struct _GUI GUI;

void create_gui(pthread_t* thread);

#endif