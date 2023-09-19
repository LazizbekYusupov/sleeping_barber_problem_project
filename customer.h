#pragma once

#define CUSTOMER_COUNT 10
#define CUSTOMER_GROWTIME 5

typedef enum customer_state_
{
    CUSTOMER_JUST_CREATED,
    CUSTOMER_HAIRY,
    CUSTOMER_CLEAN,
    CUSTOMER_WAITING
}customer_state_t;

typedef struct customer_
{
    size_t id;
    size_t grow_time;
    pthread_t thread_handle;
    pthread_mutex_t customer_mutex;
    pthread_cond_t customer_cv;
    customer_state_t customer_state;
    pthread_attr_t customer_attr;
    void* (*thread_fn_ptr)(void*);
    void* thread_arg;
}customer_t;

void customer_create(customer_t* customer, bool is_joinable, void* (*customer_lifecycle_fn_ptr)(void*));
void customer_init(customer_t** current_customer, size_t i);
void* customer_lifecycle(void* arg);
