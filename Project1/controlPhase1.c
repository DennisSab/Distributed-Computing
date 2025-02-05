#include "header.h"
#include <math.h>

void stack_overflow(struct flight_reservations *flights){
    for(int i=0;i<A;i++){
        int cap=flights[i].completed_reservations->capacity;
        int size=flights[i].completed_reservations->size;
        int tmp=0;
        struct stack_reservation *current = flights[i].completed_reservations->top;
        while(size!=0){
            tmp++;
            current=current->next;
            size--;
        }
        if(cap>=tmp){
            printf("Flight %d :stack overflow check passed(capacity:%d,found:%d)\n",i,cap,tmp);
        }else{
            printf(ANSI_COLOR_RED"Stack %d overflow did not pass because\n"ANSI_COLOR_RESET,i);
            printf(ANSI_COLOR_RED"capacity %d < found %d\n"ANSI_COLOR_RESET,cap,tmp);
            exit(1);
        } 
    }
}

void total_keysum_check(struct flight_reservations * flights){
    unsigned int keys=0;
    unsigned int sum=0;
    /*stacks*/
    for(int i=0;i<A;i++){
        int size=flights[i].completed_reservations->size;
        struct stack_reservation *current = flights[i].completed_reservations->top;
        while(size!=0){
            sum++;
            keys+=current->reservation.reservation_number;
            current=current->next;
            size--;
        }
    }
    /*queues*/
    for(int i=0;i<A;i++){
        if(flights[i].pending_reservations!=NULL){
            pthread_mutex_lock(&flights[i].pending_reservations->tail_lock);
            struct queue_reservation *cur=flights[i].pending_reservations->head;
            while(cur!=NULL){
                sum++;
                keys+=cur->reservation.reservation_number;
                cur=cur->next;
            }
            pthread_mutex_unlock(&flights[i].pending_reservations->tail_lock);
        }
    }

    /*total size check*/   
    if(sum==(unsigned int)pow(A,3)){
        printf(ANSI_COLOR_GREEN"total size check passed(expected:%d,found:%d)\n"ANSI_COLOR_RESET,(int)pow(A,3),sum);
    }else{
        printf(ANSI_COLOR_RED"total size check did not pass(expected:%d,found:%d)\n"ANSI_COLOR_RESET,(int)pow(A,3),sum);
        exit(1);
    }

    /*total keysum check*/
    unsigned int res=(((pow(A,6))+pow(A,3))/2);
    if(keys==res){
        printf(ANSI_COLOR_GREEN"total keysum check passed(expected:%d,found:%d)\n"ANSI_COLOR_RESET,res,keys);
    }else{
        printf(ANSI_COLOR_GREEN"total keysum check did not pass(expected:%d,found:%d)\n"ANSI_COLOR_RESET,res,keys);
        exit(1);
    }

}

void *control(void * arg){

    struct flight_reservations *flights = (struct flight_reservations*)arg;
    
    stack_overflow(flights);

    total_keysum_check(flights);

    barrier_cross(&controlPhase1);
    
    return NULL;
}