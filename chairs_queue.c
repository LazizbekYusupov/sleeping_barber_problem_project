//
// Created by User on 06-Sep-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "chairs_queue.h"

void chair_queue_init(chair_queue_t* chair_queue_obj)
{
    chair_queue_obj->size = CHAIR_COUNT;
    chair_queue_obj->gl_queue_list = (glthread_t*)malloc(sizeof(glthread_t));

    pthread_mutex_init(&chair_queue_obj->chair_queue_mutex, NULL);
    pthread_cond_init(&chair_queue_obj->chair_queue_cv, NULL);
    init_glthread(chair_queue_obj->gl_queue_list, offsetof(chair_t, glue_chair));
}

void chair_init(chair_t* chair_obj, size_t i)
{
    chair_obj->chair_state = CHAIR_FREE;
    chair_obj->order_number = i;

    pthread_mutex_init(&chair_obj->chair_mutex, NULL);
    pthread_cond_init(&chair_obj->chair_cv, NULL);
}

void chairs_queue_create(chair_queue_t** chair_queue_obj)
{
    if(!chair_queue_obj)
    {
        return;
    }

    chair_t* chair_obj = NULL;

    (*chair_queue_obj) = (chair_queue_t*) malloc(sizeof(chair_queue_t));
    chair_queue_init((*chair_queue_obj));

    for (int i = 0; i < CHAIR_COUNT; i++)
    {
        chair_obj = (chair_t*) malloc(sizeof(chair_t));
        chair_init(chair_obj, i);

        glthread_add_last((*chair_queue_obj)->gl_queue_list, &chair_obj->glue_chair);
    }
}

chair_t* get_chair_in_position(chair_queue_t* chair_queue_obj, size_t position)
{
    chair_t* current_chair = NULL;
    glthread_t* glued_queue = chair_queue_obj->gl_queue_list;

    glthread_node_t* next_glnode = NULL;

    if(!glued_queue->head){
        return NULL;
    }

    for (next_glnode = glued_queue->head; ; next_glnode = next_glnode->right)
    {
        current_chair = (chair_t*)((char*)next_glnode - glued_queue->offset);

        if(current_chair->order_number == position)
        {
            break;
        }
    }

    return current_chair;
}

chair_t* get_first_empty_chair(chair_queue_t* chair_queue)
{
    chair_t* first_empty_chair = NULL;

    for (int i = 0; i < CHAIR_COUNT; ++i)
    {
        first_empty_chair = get_chair_in_position(chair_queue, i);

        if(first_empty_chair->chair_state == CHAIR_FREE)
        {
            return first_empty_chair;
        }
    }

    return NULL;
}

size_t empty_chair_count(chair_queue_t* chair_queue)
{
    chair_t* empty_chair_obj = NULL;
    size_t empty_chair_count = 0;

    for (int i = 0; i < CHAIR_COUNT; ++i)
    {
        empty_chair_obj = get_chair_in_position(chair_queue, i);

        if(empty_chair_obj->chair_state == CHAIR_FREE)
        {
            empty_chair_count++;
        }
    }

    return empty_chair_count;
}





























































