#include "header.h"
#include <math.h>

void Reservations_completion_check(struct flight_reservations *flights){
    int sum=find_non_empty_queues(flights);
    
    if(central_list.head->next==NULL && sum==0 && number_of_inserter_airlines==0){
        printf(ANSI_COLOR_GREEN"Reservations completion check passed\n"ANSI_COLOR_RESET);
    }else{
        printf(ANSI_COLOR_RED"Reservations completion check DID NOT pass\n"ANSI_COLOR_RESET);
        exit(1);
    }
    
}

void *control2(void * arg){

    struct flight_reservations *flights = (struct flight_reservations*)arg;
    
    stack_overflow(flights);

    total_keysum_check(flights);

    Reservations_completion_check(flights);

    barrier_cross(&controlPhase2);
    
    return NULL;
}