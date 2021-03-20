/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.c (c) 2021
Description: Contains I/O functions for consumer/producer algorithm
Created:  2021-03-17T18:15:27.000Z
Modified: 2021-03-20T15:29:00.398Z
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
    gui->sleep_time = BASE_SLEEP;
    gui->produced_list_scroll = 0;
    gui->consumed_list_scroll = 0;
    gui->max_produced_list_items = min(MAX_LIST_ITEMS, 
                                        data->number_of_items_to_produce);
    gui->max_consumed_list_items = min(MAX_LIST_ITEMS, 
                                        data->number_of_items_to_consume);

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
    data->last_produced = newwin(2+MAX_LIST_ITEMS, data->width / 2, 13, 0);
    data->last_consumed = newwin(2+MAX_LIST_ITEMS, data->width / 2 + 1, 13,
                                                 data->width/2);
    attroff(A_BOLD);
    mvprintw(21, 0, "Press 'k' to slow down, or 'l' to speed up, or 'q' to quit");
    mvprintw(22, 0, "Press 'x' to scroll produced list down, or 'c' to scroll produced list up");
    mvprintw(23, 0, "Press 'd' to scroll consumed list down, or 'f' to scroll consumed list up");
    
    refresh();
    
    draw_box(data->progressbar, "Buffer");
    draw_box(data->constants, "Constants");
    draw_box(data->variables, "Variables");
    draw_box(data->last_produced, "Produced");
    draw_box(data->last_consumed, "Consumed");
}




static void update_term_size(GUI* data)
{
    int new_width, new_height;
    getmaxyx(stdscr, new_height, new_width);
    if (new_width != data->width || new_height != data->height)
    {
        data->width = new_width;
        data->height = new_height;
        clear();
        draw(data);
    }
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

static void update_last_items(WINDOW* win, int *last, int scroll, int max_items)
{
    last += scroll;

    for(int row = 1; 
            row <= max_items; row++, last++)
        mvwprintw(win, row, 1, "%d", last[0]);
    wrefresh(win);
}

static void update_last_produced(GUI* data)
{
    update_last_items(data->last_produced, 
        data->consumer_producer->last_produced, 
        data->produced_list_scroll, data->max_produced_list_items);
}

static void update_last_consumed(GUI* data)
{
    update_last_items(data->last_consumed, 
        data->consumer_producer->last_consumed, 
        data->consumed_list_scroll, data->max_consumed_list_items);
}

static void update(GUI* data)
{
    update_term_size(data);
    update_constants(data);
    if(data->consumer_producer->number_of_items_to_produce > 0)
    {
        update_variables(data);
        update_progressbar(data);
        update_last_produced(data);
        update_last_consumed(data);
    }
    refresh();
}




static void scroll_down_input(int *scroll, int max_items_to_process)
{
    if(scroll[0] + MAX_LIST_ITEMS < max_items_to_process)
        scroll[0]++;
}

static void scroll_up_input(int *scroll)
{
    if(scroll[0] > 0)
        scroll[0]--;
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
        case 'x':
            scroll_down_input(&gui_data->produced_list_scroll, 
                            data->number_of_items_to_produce);
            break;
        case 'c':
            scroll_up_input(&gui_data->produced_list_scroll);
            break;
        case 'd':
            scroll_down_input(&gui_data->consumed_list_scroll, 
                            data->number_of_items_to_consume);
            break;
        case 'f':
            scroll_up_input(&gui_data->consumed_list_scroll);
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
    int sleep_counter = 0;
    
    while(data->running)
    {
        sem_getvalue(no_print, &tmp);
        if (tmp == 0)
        {
            while(sleep_counter < gui_data->sleep_time)
            {
                input(gui_data);
                update(gui_data);
                msleep(SLEEP_STEP);
                sleep_counter += SLEEP_STEP;
            }
            sleep_counter = 0;
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