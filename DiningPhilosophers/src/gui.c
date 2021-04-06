/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
gui.c (c) 2021
Description: Contains I/O functions for consumer/producer algorithm
Created:  2021-03-17T18:15:27.000Z
Modified: 2021-03-20T19:38:22.458Z
*/

#include "../include/gui.h"

extern GUI* alloc_gui()
{
    GUI *gui = (GUI*) malloc(sizeof(GUI));

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
    
    return min((int) ceil(philosophers_quantity / 2.0)
                     - data->table_scroll,
                (int) floor(table_width / TABLE_BLOCK_SIZE));
}

static void update_table(GUI* data)
{
    WINDOW* table = data->table;
    
    const int scroll = data->table_scroll;
    const int odd_quantity = is_odd(philosophers_quantity);
    const int odd_spacer = odd_quantity + 1;
    
    philosopher* philosophers_ = philosophers + scroll;
    int limit = calculate_max_philosophers_per_row(data);
    for(int i = 0; i < limit; i++, philosophers_++)
        print_table_block(table, philosophers_[0], 1, i*2+1);

    //the last items of the table are displayed at the second row
    int start = limit - odd_quantity;
    //jumps the gap between rows
    philosophers_ += philosophers_quantity - (start + limit + 2*scroll);
    for(int i = start - 1; i >= 0; i--, philosophers_++)
        print_table_block(table, philosophers_[0], 2, i*2+odd_spacer);

    wrefresh(table);
}

static void update_constants(GUI* data)
{
    WINDOW* win = data->constants;
    
    mvwprintw(win, 1, 2, "Philosophers quantity: %d    ", philosophers_quantity);
    mvwprintw(win, 1, data->width/2, "Thinking time: %d    ", thinking_time);
    mvwprintw(win, 2, 2, "Eating time: %d    ", eating_time);
    
    wrefresh(win);
}

static void update_variables(GUI* data)
{
    WINDOW* win = data->variables;

    mvwprintw(win, 1, 2, "Philosophers eating: %d     ",
        eating_philosophers_quantity);
    mvwprintw(win, 1, data->width/2, "Philosophers thinking: %d     ", 
        thinking_philosophers_quantity);
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
            running = false;
            break;
        default:
            break;
    }
}




static void gui(void *arg)
{
    start_gui();
    GUI* gui_data = alloc_gui();
    draw(gui_data);
    
    int tmp;
    
    while(running)
    {
        input(gui_data);
        update(gui_data);
        msleep(25);
    }

    free_gui(gui_data);
    endwin();
    pthread_exit(NULL);
}




extern void create_gui(pthread_t* thread)
{
    create_thread(thread, &gui, NULL);
}
