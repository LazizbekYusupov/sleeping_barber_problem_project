//
// Created by User on 12-Sep-23.
//
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "customer.h"

void customer_init(customer_t** current_customer, size_t i)
{
    (*current_customer) = (customer_t*) malloc(sizeof(customer_t));
    (*current_customer)->id = i;
    (*current_customer)->grow_time = rand() % CUSTOMER_GROWTIME + 1;
    (*current_customer)->thread_fn_ptr = customer_lifecycle;
    (*current_customer)->thread_arg = current_customer;

    pthread_mutex_init(&(*current_customer)->customer_mutex, NULL);
    pthread_cond_init(&(*current_customer)->customer_cv, NULL);
}

void customer_create(customer_t* customer, bool is_joinable, void* (*customer_lifecycle_fn_ptr)(void*))
{
    int customer_attr;

    if (is_joinable)
    {
        customer_attr = PTHREAD_CREATE_JOINABLE;
    }
    else
    {
        customer_attr = PTHREAD_CREATE_DETACHED;
    }

    pthread_attr_init(&customer->customer_attr);
    pthread_attr_setdetachstate(&customer->customer_attr, customer_attr);

    printf("Customer_%zd creation order to be given\n", customer->id);
    pthread_create(&customer->thread_handle, &customer->customer_attr, customer_lifecycle_fn_ptr, (void*)customer);
}


































