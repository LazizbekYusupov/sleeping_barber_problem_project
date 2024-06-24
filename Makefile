# -*- Makefile -*-

all: main.o barber.o barbershop.o barbershop_universe.o chairs_queue.o customer.o glue_threads.o glued_thread_queue.o
	gcc main.o barber.o barbershop.o barbershop_universe.o chairs_queue.o customer.o glue_threads.o glued_thread_queue.o -o sleeping_barber_vsc -lpthread

main.o: main.c barbershop_universe.h
	gcc -Wall -g -c -O0 main.c

barbershop_universe.o: barbershop_universe.c barbershop_universe.h customer.h barbershop.h
	gcc -Wall -g -c -O0 barbershop_universe.c

barbershop.o: barbershop.c barbershop.h
	gcc -Wall -g -c -O0 barbershop.c

barber.o: barber.c barber.h
	gcc -Wall -g -c -O0 barber.c

customer.o: customer.c customer.h
	gcc -Wall -g -c -O0 customer.c

chairs_queue.o: chairs_queue.c chairs_queue.h glued_thread_queue.h
	gcc -Wall -g -c -O0 chairs_queue.c

glued_thread_queue.o: glued_thread_queue.c glued_thread_queue.h
	gcc -Wall -g -c -O0 glued_thread_queue.c

clean:
	rm *.o sleeping_barber_vsc