#include "header.h"

void *push(struct flight_reservations flight,struct Reservation res){
    struct stack_reservation *new_node=malloc(sizeof(struct stack_reservation));
    if(new_node==NULL){
        fprintf(stderr,"Memory allocation failed.\n");
        return NULL;
    }

    new_node->reservation=res;

    /*lock the stack before modification*/
    pthread_mutex_lock(&flight.completed_reservations->top_lock);

    new_node->next=flight.completed_reservations->top;
    flight.completed_reservations->top=new_node;
    flight.completed_reservations->size++;

    pthread_mutex_unlock(&flight.completed_reservations->top_lock);
    
    return NULL;

}


void *enqueue(struct flight_reservations flight,struct Reservation res){
    struct queue_reservation *new_node=malloc(sizeof(struct queue_reservation));
    if(new_node==NULL){
        fprintf(stderr,"Memory allocation failed.\n");
        return NULL;
    }

    new_node->reservation=res;
    new_node->next=NULL;

    /*lock the tail before modifying it*/
    pthread_mutex_lock(&flight.pending_reservations->tail_lock);

    if(flight.pending_reservations->head==NULL){
        flight.pending_reservations->head=new_node;
        flight.pending_reservations->tail=new_node;
    }else{
        flight.pending_reservations->tail->next=new_node;
        flight.pending_reservations->tail=new_node;
    }

    /*unlock the tail*/
    pthread_mutex_unlock(&flight.pending_reservations->tail_lock);

    return NULL;
}


void *make_reservations(void *arg){
    struct args1 * args=(struct args1 *) arg;
    int index=args->practoryID;
    struct flight_reservations  flight=args->flight;

    //create A reservations for every practory
    for(int i=0;i<A;i++){
        struct Reservation *res=malloc(sizeof(struct Reservation));
        res->reservation_number=((i)*(A*A)+index)+1;
        res->agency_id=index+1;     
        if((flight.completed_reservations->capacity)<=(flight.completed_reservations->size)){
            enqueue(flight,*res);
        }else{
            push(flight,*res);
        }
    }
    barrier_cross(&phase1_barrier);
    return NULL;

}