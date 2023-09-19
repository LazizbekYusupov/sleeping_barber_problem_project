cmake_minimum_required(VERSION 3.25)
project(sleeping_barber_problem_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(sleeping_barber_problem_project main.c glue_threads.c glue_threads.h glued_thread_queue.c glued_thread_queue.h barbershop_universe.c barbershop_universe.h chairs_queue.c chairs_queue.h customer.h barbershop.h barbershop.c customer.c barber.c barber.h)
