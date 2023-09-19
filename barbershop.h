#pragma once

#include "chairs_queue.h"
#include "barber.h"

typedef struct barbershop_
{
    chair_queue_t* chair_queue;
    barber_t* barber;
}barbershop_t;

void barbershop_init(barbershop_t** barbershop, barber_t** barber);
