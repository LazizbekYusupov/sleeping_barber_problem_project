//
// Created by User on 12-Sep-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "barber.h"

void barber_init(barber_t** barber)
{
    *barber = (barber_t*) malloc(sizeof(barber_t));

    (*barber)->barber_state = BARBER_WORKING;
    (*barber)->thread_fn_ptr = barber_lifecycle;

    pthread_cond_init(&(*barber)->barber_cv, NULL);
    pthread_mutex_init(&(*barber)->barber_mutex, NULL);
}

void barber_create(barber_t* barber, bool is_joinable, void* (*barber_lifecycle_fn_ptr)(void*), void* barbershop_thread_arg)
{
    int barber_attr_state;

    if (is_joinable)
    {
        barber_attr_state = PTHREAD_CREATE_JOINABLE;
    }
    else
    {
        barber_attr_state = PTHREAD_CREATE_DETACHED;
    }

    pthread_attr_init(&barber->barber_attr);
    pthread_attr_setdetachstate(&barber->barber_attr, barber_attr_state);

    pthread_create(&barber->thread_handle, &barber->barber_attr, barber_lifecycle_fn_ptr, (void *) barbershop_thread_arg);
    printf("Barber creation order given\n");
}





