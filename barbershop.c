//
// Created by User on 12-Sep-23.
//
#include <stdio.h>
#include <stdlib.h>
#include "barbershop.h"

void barbershop_init(barbershop_t** barbershop, barber_t** barber)
{
    (*barbershop) = (barbershop_t*) malloc(sizeof(barbershop_t));

    chairs_queue_create(&(*barbershop)->chair_queue);

    (*barbershop)->barber = (*barber);

}