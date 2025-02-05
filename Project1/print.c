#include "header.h"

void print_flights(struct flight_reservations *flights) {
    for (int i = 0; i < A; i++) {
        printf("Flight %d - Completed Reservations:\n", i+1);
        print_stack(flights[i].completed_reservations);

        printf("Flight %d - Pending Reservations:\n", i+1);
        print_queue(flights[i].pending_reservations);

        printf("--------------------------------------------------\n");
    }
}

void print_stack(struct stack *s) {
    printf("    Stack Contents:\n");
    int tmp=s->size;
    struct stack_reservation *current = s->top;
    while (tmp != 0) {
        printf("      Reservation ID: %d\n", current->reservation.reservation_number);
        current = current->next;
        tmp--;
    }
}

void print_queue(struct queue *q) {
    pthread_mutex_lock(&q->tail_lock);  /*Lock the queue to ensure consistent printing*/
    printf("    Queue Contents:\n");
    struct queue_reservation *current = q->head;
    while (current != NULL) {
        printf("      Reservation ID: %d\n", current->reservation.reservation_number);
        current = current->next;
    }
    pthread_mutex_unlock(&q->tail_lock);  /*Unlock the queue after printing*/
}


void print_list(struct list* central_list){
    struct list_reservation *curr=central_list->head;
    printf("\nCentral List:\n");
    if(curr->reservation.reservation_number==0 && curr->next==NULL){
        printf("Central List is Empty");
        curr=NULL;
    }
    while(curr!=NULL){
        printf("%d->",curr->reservation.reservation_number);
        curr=curr->next;
    }
    printf("\n");
}
