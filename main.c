#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <pthread.h>
#include "barbershop_universe.h"

int main(void)
{
    printf("Test changes to web repository\n");

    barbershop_simulation_start();

    pthread_cancel(0);
}































