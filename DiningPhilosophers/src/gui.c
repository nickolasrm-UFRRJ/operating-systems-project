/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.c (c) 2021
Description: Contains I/O functions for consumer/producer algorithm
Created:  2021-03-17T18:15:27.000Z
Modified: 2021-03-19T07:05:37.959Z
*/

#include "../include/gui.h"

extern GUI* alloc_gui(DiningPhilosophers *data)
{
    GUI *gui = (GUI*) malloc(sizeof(GUI));

    gui->dinner = data;
    gui->table_scroll = 0;
    getmaxyx(stdscr, gui->height, gui->width);

    return gui;
}

extern void free_gui(GUI* data)
{
    delwin(data->table);
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
    start_color();
    use_default_colors();

    init_pair(TABLE_BLOCK_GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(TABLE_BLOCK_RED, COLOR_RED, COLOR_RED);
}




static void draw_box(WINDOW* win, char *label)
{
    box(win, 0, 0);
    mvwprintw(win, 0, 1, " %s ", label);
    wrefresh(win);
}

static void draw(GUI* data)
{
    DiningPhilosophers *dinner = data->dinner;

    attron(A_BOLD);
    mvprintw(0, 1, "Dining Philosophers");
    attroff(A_BOLD);
    
    data->table = newwin(4, data->width, 2, 0);
    data->constants = newwin(4, data->width, 6,0);
    data->variables = newwin(4, data->width, 10,0);
    
    mvprintw(14, 0, "Press 'k' to scroll table to the left, or 'l'"\
        "to scroll table to the right, or 'q' to quit."\
        "\n- Green = Eating\n- Red = Thinking");
    
    refresh();
    
    draw_box(data->table, "Table");
    draw_box(data->constants, "Constants");
    draw_box(data->variables, "Variables");
}




static void clear_draw(GUI* data)
{
    clear();
    draw(data);
}

static void update_term_size(GUI* data)
{
    int new_width, new_height;
    getmaxyx(stdscr, new_height, new_width);
    if (new_width != data->width || new_height != data->height)
    {
        data->width = new_width;
        data->height = new_height;
        clear_draw(data);
    }
}

static void print_table_block(WINDOW* table, philosopher phil, int y, int x)
{
    int color;
    int tmp;

    if (phil == EATING)
        color = TABLE_BLOCK_GREEN;
    else
        color = TABLE_BLOCK_RED;

    wattron(table, COLOR_PAIR(color));
    for(int j = 0; j < TABLE_BLOCK_SIZE; j++)
        mvwprintw(table, y, x+j, "X");
    wattroff(table, COLOR_PAIR(color));
}

static int calculate_max_philosophers_per_row(GUI* data)
{
    int table_width, table_height; getmaxyx(data->table, table_height, table_width);
    table_width -= 2; table_height -= 2;
    return min((int) ceil(data->dinner->philosophers_quantity / 2.0)
                     - data->table_scroll,
                (int) floor(table_width / TABLE_BLOCK_SIZE));
}

static void update_table(GUI* data)
{
    WINDOW* table = data->table;
    DiningPhilosophers* dinner = data->dinner;
    
    const int philosophers_quantity = dinner->philosophers_quantity;
    const int scroll = data->table_scroll;
    const int odd_quantity = is_odd(philosophers_quantity);
    const int odd_spacer = odd_quantity + 1;
    
    philosopher* philosophers = dinner->philosophers + scroll;
    int limit = calculate_max_philosophers_per_row(data);
    for(int i = 0; i < limit; i++, philosophers++)
        print_table_block(table, philosophers[0], 1, i*2+1);

    philosophers += scroll;
    limit = limit - odd_quantity;
    for(int i = 0; i < limit; i++, philosophers++)
        print_table_block(table, philosophers[0], 2, i*2+odd_spacer);

    wrefresh(table);
}

static void update_constants(GUI* data)
{
    WINDOW* win = data->constants;
    DiningPhilosophers* dinner = data->dinner;
    
    mvwprintw(win, 1, 2, "Philosophers quantity: %d    ", dinner->philosophers_quantity);
    mvwprintw(win, 1, data->width/2, "Thinking time: %d    ", dinner->thinking_time);
    mvwprintw(win, 2, 2, "Eating time: %d    ", dinner->eating_time);
    
    wrefresh(win);
}

static void update_variables(GUI* data)
{
    WINDOW* win = data->variables;
    DiningPhilosophers* dinner = data->dinner;

    mvwprintw(win, 1, 2, "Eating philosophers: %d     ",
        dinner->eating_philosophers_quantity);
    mvwprintw(win, 1, data->width/2, "Thinking philosophers: %d     ", 
        dinner->thinking_philosophers_quantity);
    mvwprintw(win, 2, 2, "Table scroll: %d     ",
        data->table_scroll);
        
    wrefresh(win);
}

static void update(GUI* data)
{
    update_term_size(data);
    update_variables(data);
    update_constants(data);
    update_table(data);
    refresh();
}




static void increment_scroll(GUI* data, int increment)
{
    data->table_scroll += increment;
    if(data->table_scroll < 0) data->table_scroll = 0;
    if(calculate_max_philosophers_per_row(data) < 1) 
        data->table_scroll -= increment;
    clear_draw(data);
}

static void input(GUI* gui_data)
{
    DiningPhilosophers* data = gui_data->dinner;
    char op = '0';

    op = getch();
    switch(op)
    {
        case 'k':
            increment_scroll(gui_data, -1);
            break;
        case 'l':
            increment_scroll(gui_data, 1);
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
    DiningPhilosophers *data = (DiningPhilosophers*) arg;
    GUI* gui_data = alloc_gui(data);
    draw(gui_data);
    
    int tmp;
    
    while(data->running)
    {
        input(gui_data);
        update(gui_data);
        msleep(25);
    }

    free_gui(gui_data);
    endwin();
    pthread_exit(NULL);
}




extern void create_gui(pthread_t* thread, DiningPhilosophers *data)
{
    create_thread(thread, &gui, data);
}