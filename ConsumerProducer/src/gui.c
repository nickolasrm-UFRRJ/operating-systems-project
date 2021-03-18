/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.c (c) 2021
Description: Contains I/O functions for consumer/producer algorithm
Created:  2021-03-17T18:15:27.000Z
Modified: 2021-03-18T07:50:57.230Z
*/

#include "../include/gui.h"

//deprecated
extern void print_products(ConsumerProducer *data)
{
    printf("------\n");
    for(int i = 0; i < data->buffer_size; i++)
        printf("%d\n", data->products[i]);
    printf("------");
    fflush(stdout);
}

extern GUI* alloc_gui(ConsumerProducer *data)
{
    GUI *gui = (GUI*) malloc(sizeof(GUI));

    gui->consumer_producer = data;
    gui->sleep_time = 100;
    getmaxyx(stdscr, gui->height, gui->width);

    return gui;
}

extern void free_gui(GUI* data)
{
    delwin(data->progressbar);
    delwin(data->variables);
    delwin(data->constants);
    free(data);
}




extern void start_gui()
{
    initscr();
    cbreak();
    noecho();
    clear();
    curs_set(0);
    nodelay(stdscr, TRUE);
}




static void draw_box(WINDOW* win, char *label)
{
    box(win, 0, 0);
    mvwprintw(win, 0, 1, " %s ", label);
    wrefresh(win);
}

static void draw(GUI* data)
{
    attron(A_BOLD);
    mvprintw(0, 1, "Consumer Producer");
    data->progressbar = newwin(3, data->width, 2, 0);
    data->constants = newwin(4, data->width, 5,0);
    data->variables = newwin(4, data->width, 9,0);
    attron(A_NORMAL);
    mvprintw(13, 0, "Press 'k' to slow down, or 'l' to speed up, or 'q' to quit");
    
    refresh();
    
    draw_box(data->progressbar, "Buffer");
    draw_box(data->constants, "Constants");
    draw_box(data->variables, "Variables");
}




static void update_progressbar(GUI* data)
{
    WINDOW* progressbar = data->progressbar;
    ConsumerProducer* consumer_producer = data->consumer_producer;
    int real_width = data->width-2;
    float percentage = ((float) consumer_producer->counter / 
                                consumer_producer->buffer_size);
    int until_spaces = (int)((float) real_width * percentage);
    for(int i = 2; i < until_spaces; i++)
        mvwprintw(progressbar, 1, i, "#");
    for(int i = until_spaces; i < real_width; i++)
        mvwprintw(progressbar, 1, i, " ");

    mvwprintw(progressbar, 1, real_width-4, "     ");
    mvwprintw(progressbar, 1, real_width-4, " %d%%", (int)(percentage * 100));
    wrefresh(progressbar);
}

static void update_constants(GUI* data)
{
    WINDOW* win = data->constants;
    ConsumerProducer* consumer_producer = data->consumer_producer;
    mvwprintw(win, 1, 2, "Buffer Size: %d    ", consumer_producer->buffer_size);
    mvwprintw(win, 1, data->width/2, "No of items to consume: %d    ", 
        consumer_producer->number_of_items_to_consume);
    mvwprintw(win, 2, 2, "No of items to produce: %d    ", 
        consumer_producer->number_of_items_to_produce);
    wrefresh(win);
}

static void update_variables(GUI* data)
{
    WINDOW* win = data->variables;
    int tmp;
    ConsumerProducer* consumer_producer = data->consumer_producer;
    sem_getvalue(&consumer_producer->empty, &tmp);
    mvwprintw(win, 1, 2, "Empty semaphore: %d      ", tmp);
    sem_getvalue(&consumer_producer->full, &tmp);
    mvwprintw(win, 1, data->width/2, "Full semaphore: %d     ", tmp);
    mvwprintw(win, 2, 2, "Counter: %d     ", consumer_producer->counter);
    mvwprintw(win, 2, data->width/2, "Simulation speed: %f    ", 
            2.0f / ((float) data->sleep_time / (MAX_SLEEP)));
    wrefresh(win);
}

static void update(GUI* data)
{
    update_variables(data);
    update_constants(data);
    update_progressbar(data);
    refresh();
}




static void input(GUI* gui_data)
{
    ConsumerProducer* data = gui_data->consumer_producer;
    char op = '0';

    op = getch();
    switch(op)
    {
        case 'k':
            if(gui_data->sleep_time < MAX_SLEEP)
                gui_data->sleep_time += SLEEP_STEP;
            break;
        case 'l':
            if(gui_data->sleep_time > MIN_SLEEP)
                gui_data->sleep_time -= SLEEP_STEP;
            break;
        case 'q':
            data->running = false;
            break;
        default:
            break;
    }
}




static void gui(void *arg)
{
    start_gui();
    ConsumerProducer *data = (ConsumerProducer*) arg;
    GUI* gui_data = alloc_gui(data);
    draw(gui_data);
    
    sem_t *no_print = &data->no_print;
    int tmp;
    
    while(data->running)
    {
        sem_getvalue(no_print, &tmp);
        if (tmp == 0)
        {
            long start  = timestamp_ms();
            while((timestamp_ms() - start) < gui_data->sleep_time)
            {
                input(gui_data);
                update(gui_data);
                msleep(25);
            }
            sem_post(no_print);
        }
    }

    free_gui(gui_data);
    endwin();
    pthread_exit(NULL);
}




extern void create_gui(pthread_t* thread, ConsumerProducer *data)
{
    create_thread(thread, &gui, data);
}