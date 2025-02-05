#include "header.h"

void initialize_flight_reservations(struct flight_reservations *flights){

    for(int i=0;i<A;i++){
        flights[i].completed_reservations=NULL;
        flights[i].pending_reservations=NULL;
    }

    /*initialize sentinel node of central list*/
    struct list_reservation * sentinel=malloc(sizeof(struct list_reservation));
    pthread_mutex_init(&sentinel->lock,NULL);
    sentinel->marked=0;
    sentinel->next=NULL;
    sentinel->reservation.agency_id=0;
    sentinel->reservation.reservation_number=0;
    central_list.head=sentinel;


    /*initialize each stack*/
    for(int i=0;i<A;i++){
        flights[i].completed_reservations=malloc(sizeof(struct stack));
        pthread_mutex_init(&flights[i].completed_reservations->top_lock,NULL);
        flights[i].completed_reservations->top = NULL; // Initialize top to NULL or whatever is appropriate
        flights[i].completed_reservations->size = 0;   // Initialize size to 0
        flights[i].completed_reservations->capacity = ((3.0 / 2.0) * (A * A) - (A - (i + 1)) * A); // Example capacity, change as needed
    }


    /*initilize each queue*/
    for(int i=0;i<A;i++){
        flights[i].pending_reservations=malloc(sizeof(struct queue));
        pthread_mutex_init(&flights[i].pending_reservations->head_lock,NULL);
        pthread_mutex_init(&flights[i].pending_reservations->tail_lock,NULL);
        flights[i].pending_reservations->head=NULL;
        flights[i].pending_reservations->tail=NULL;
    }

    /*initialize barriers*/
    barrier_init(&phase1_barrier,A*A);
    barrier_init(&controlPhase1,1);
    barrier_init(&phase2_barrier,A);
    barrier_init(&controlPhase2,1);

    return;
}

int find_non_empty_queues(struct flight_reservations *flights){
    int sum=0;
    for(int i=0;i<A;i++){
        pthread_mutex_lock(&flights[i].pending_reservations->tail_lock);
        struct queue_reservation *cur=flights[i].pending_reservations->head;
        if(cur!=NULL){
            sum++;
        }
        pthread_mutex_unlock(&flights[i].pending_reservations->tail_lock);
    }
    return sum;
}

