#pragma once

#include <stdbool.h>

typedef enum barber_state_
{
    BARBER_WORKING,
    BARBER_SLEEPING,
    BARBER_NOT_GOING_TO_SLEEP,
    BARBER_GOING_TO_SLEEP
}barber_state_t;

typedef struct barber_
{
    pthread_t thread_handle;
    pthread_mutex_t barber_mutex;
    pthread_cond_t barber_cv;
    barber_state_t barber_state;
    pthread_attr_t barber_attr;
    void* (*thread_fn_ptr)(void*);
    void* arg;
}barber_t;

void barber_init(barber_t** barber);
void barber_create(barber_t* barber, bool is_joinable, void* (*barber_lifecycle_fn_ptr)(void*), void* barbershop_thread_arg);
void* barber_lifecycle(void* arg);
