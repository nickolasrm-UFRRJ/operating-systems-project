/*
Author: Nickolas da Rocha Machado , Natalia Zambe da Silva 
Emails: nickolas123full@gmail.com , nataliazambe@gmail.com
misc.c (c) 2021
Description: General purpose functions
Created:  2021-03-17T03:54:23.303Z
Modified: 2021-03-18T07:35:55.278Z
*/
#include "../include/misc.h"

extern pthread_t* alloc_threads(int number)
{
    return (pthread_t*) malloc(sizeof(pthread_t) * number);
}

extern void create_thread(pthread_t *thread, void *function, void *data)
{
    if (pthread_create(thread, NULL, function, data) != 0)
        perror("Failed to create the thread");
}

extern void join_thread(pthread_t *thread)
{
    if (pthread_join(*thread, NULL) != 0)
        perror("Failed to join the thread");
}

extern void join_threads(pthread_t *threads, int number)
{
    for(int i = 0; i < number; i++)
        join_thread(threads+i);
}

extern int msleep(long tms)
{
    struct timespec ts;
    int ret;

    if (tms < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);

    return ret;
}

extern long timestamp_ms() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    return te.tv_sec*1000LL + te.tv_usec/1000;
}