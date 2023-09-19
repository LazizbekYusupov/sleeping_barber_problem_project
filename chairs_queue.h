#pragma once

#include "glued_thread_queue.h"

#define CHAIR_COUNT 5

typedef enum chair_state_
{
    CHAIR_BUSY,
    CHAIR_FREE
}chair_state_t;

typedef struct chair_
{
    void* customer_on_chair;
    chair_state_t chair_state;
    size_t order_number;
    pthread_mutex_t chair_mutex;
    pthread_cond_t chair_cv;
    glthread_node_t glue_chair;
}chair_t;

typedef struct chair_queue_
{
    size_t size;
    pthread_mutex_t chair_queue_mutex;
    pthread_cond_t chair_queue_cv;
    glthread_t* gl_queue_list;
}chair_queue_t;

void chairs_queue_create(chair_queue_t** chair_queue_obj);
chair_t* get_chair_in_position(chair_queue_t* chair_queue_obj, size_t position);
chair_t* get_first_empty_chair(chair_queue_t* chair_queue);
size_t empty_chair_count(chair_queue_t* chair_queue);
