#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "barbershop_universe.h"
#include "customer.h"
#include "barbershop.h"

static barber_t * barber_global = NULL;
static barbershop_t* barbershop_global = NULL;

void barbershop_simulation_start()
{
    barber_init(&barber_global);
    barbershop_init(&barbershop_global, &barber_global);

    barber_create(barber_global, false, barber_lifecycle, barbershop_global);

    create_all_customers();

    pthread_exit(0);
}

void create_all_customers()
{
    time_t t;
    customer_t* customer_obj_var = NULL;

    srand(time(&t));

    for (size_t i = 0; i < CUSTOMER_COUNT; i++)
    {
        customer_init(&customer_obj_var, i);
        customer_create(customer_obj_var, false, customer_lifecycle);
    }
}

void* customer_lifecycle(void* arg)
{
    customer_t* customer = (customer_t*)arg;
    customer->customer_state = CUSTOMER_JUST_CREATED;

    chair_queue_t* chair_queue = barbershop_global->chair_queue;
    chair_t* chair = NULL;

    printf("Customer_%zd entered function, growtime = %zd\n", customer->id, customer->grow_time);

    while (1)
    {
        pthread_mutex_lock(&customer->customer_mutex);

        sleep(customer->grow_time);
        customer->customer_state = CUSTOMER_HAIRY;
        printf("Customer_%zd entered loop, becoming hairy after growtime = %zd\n", customer->id, customer->grow_time);

        pthread_mutex_lock(&chair_queue->chair_queue_mutex);
        printf("Customer_%zd gt = %zd locked chair_queue_mutex\n", customer->id, customer->grow_time);

        chair = get_first_empty_chair(chair_queue);

        if(chair != NULL)
        {
            printf("Customer_%zd gt = %zd seen chair %zd to be empty and got it\n", customer->id, customer->grow_time, chair->order_number);
        }
        else
        {
            printf("Customer_%zd gt = %zd got not empty chair\n", customer->id, customer->grow_time);
        }

        pthread_mutex_unlock(&chair_queue->chair_queue_mutex);

        if(chair != NULL)
        {
            pthread_mutex_lock(&chair->chair_mutex);
            printf("within if Customer_%zd gt = %zd locked chair %zd mutex\n", customer->id, customer->grow_time, chair->order_number);
            chair->customer_on_chair = (void*)customer;
            chair->chair_state = CHAIR_BUSY;

            pthread_mutex_lock(&barber_global->barber_mutex);

            if(barber_global->barber_state == BARBER_SLEEPING)
            {
                printf("Customer_%zd gt = %zd seen barber sleeping and waking it up\n", customer->id, customer->grow_time);
                pthread_cond_signal(&barber_global->barber_cv);
                barber_global->barber_state = BARBER_WORKING;
            }

            pthread_mutex_unlock(&barber_global->barber_mutex);

            printf("Customer_%zd gt = %zd getting chair %zd is going to sleep\n", customer->id, customer->grow_time, chair->order_number);
            customer->customer_state = CUSTOMER_WAITING;
            pthread_cond_wait(&customer->customer_cv, &customer->customer_mutex);

            pthread_mutex_unlock(&chair->chair_mutex);
        }
        else
        {
            printf("Customer_%zd gt = %zd seen all chairs full and leaving barbershop\n", customer->id, customer->grow_time);
            pthread_mutex_unlock(&chair_queue->chair_queue_mutex);
            pthread_mutex_unlock(&customer->customer_mutex);
            continue;
        }

        pthread_mutex_unlock(&customer->customer_mutex);
    }

    return NULL; //Just for in case. Considered to be good programming practice
}

void* barber_lifecycle(void* arg)
{
    chair_queue_t* chair_queue = barbershop_global->chair_queue;
    chair_t* current_chair = NULL;
    customer_t* current_customer = NULL;

    printf("Barbershop started!\n");

    while (1)
    {
            printf("Start over chair loop from the beginning\n");
            if(barber_global->barber_state == BARBER_GOING_TO_SLEEP)
            {
                printf("Barber encountered all empty chairs to sleep\n");
                pthread_mutex_lock(&barber_global->barber_mutex);

                barber_global->barber_state = BARBER_SLEEPING;
                pthread_cond_wait(&barber_global->barber_cv, &barber_global->barber_mutex);
                printf("Barber woke up here and to unlock its mutex\n");

                pthread_mutex_unlock(&barber_global->barber_mutex);
            }

        barber_global->barber_state = BARBER_GOING_TO_SLEEP;

        for (int i = 0; i < CHAIR_COUNT; i++)
        {
            pthread_mutex_lock(&chair_queue->chair_queue_mutex);
            current_chair = get_chair_in_position(chair_queue, i);
            printf("Barber got chair_%zd here\n", current_chair->order_number);

            if(current_chair->chair_state == CHAIR_BUSY)
            {
                barber_global->barber_state = BARBER_NOT_GOING_TO_SLEEP;

                printf("Chair %zd found to be busy, now taking hair for 2 seconds\n", current_chair->order_number);
                sleep(2);

                current_customer = (customer_t*)current_chair->customer_on_chair;

                pthread_mutex_lock(&current_customer->customer_mutex);
                printf("Barber locked Customer_%zd to change state\n", current_customer->id);

                pthread_cond_signal(&current_customer->customer_cv);
                current_customer->customer_state = CUSTOMER_CLEAN;
                printf("Barber unlocked Customer_%zd outside\n", current_customer->id);

                pthread_mutex_unlock(&current_customer->customer_mutex);
                pthread_mutex_unlock(&chair_queue->chair_queue_mutex);
            }
            else
            {
                printf("Barber seen chair %zd to be empty\n", current_chair->order_number);
                pthread_mutex_unlock(&chair_queue->chair_queue_mutex);
            }
        }
    }

    return NULL; //Just for in case. Considered to be good programming practice
}



















