#include "header.h"

pthread_mutex_t number_of_inserter_airlines_lock = PTHREAD_MUTEX_INITIALIZER;
struct list central_list;

int validate(struct list_reservation *pred,struct list_reservation *curr){
    if(pred->marked==0 && curr->marked==0 && pred->next==curr){
        return 1;
    }else{
        return 0;
    }
}

void add_to_list(struct Reservation res) {
    struct list_reservation *new_node = (struct list_reservation*)malloc(sizeof(struct list_reservation));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->reservation = res;
    new_node->marked = 0;
    pthread_mutex_init(&new_node->lock, NULL);
    new_node->next = NULL;

    pthread_mutex_lock(&new_node->lock);
    
    /*Insert in the list*/
    if (central_list.head == NULL) {
        /*If list is empty, new node becomes both head and tail*/
        central_list.head = new_node;
        central_list.tail = new_node;
        pthread_mutex_unlock(&new_node->lock);
    } else {
        struct list_reservation *pred = NULL;
        struct list_reservation *curr = central_list.head;

        /*Traverse the list to find the correct position for insertion*/
        while (curr != NULL && curr->reservation.reservation_number < new_node->reservation.reservation_number) {
            pred = curr;
            curr = curr->next;
        }

        /*Lock the nodes for insertion*/
        if (pred != NULL) {
            pthread_mutex_lock(&pred->lock);
        }
        
        if (pred == NULL) { /*Insert at the beginning*/
            new_node->next = central_list.head;
            central_list.head = new_node;

            if(central_list.tail==NULL){
                central_list.tail=new_node;
            }

        } else { 
            /*Insert in the middle or at the end*/
            new_node->next = curr;
            pred->next = new_node;
            if (curr == NULL) {
                central_list.tail = new_node; /*Update tail if inserting at the end*/
            }
        }

        /*Unlock the nodes*/
        if (pred != NULL) {
            pthread_mutex_unlock(&pred->lock);
        }
        pthread_mutex_unlock(&new_node->lock);
    }

    /*print_list(&central_list);*/
}

void *removeF(struct flight_reservations *flight) {
    struct stack *s = flight->completed_reservations;


    while (s->size < s->capacity && (central_list.head->next != NULL || number_of_inserter_airlines!=0)){
        struct list_reservation *prev = central_list.head;
        pthread_mutex_lock(&prev->lock);

        struct list_reservation *elem = prev->next;
        if (elem != NULL) {
            pthread_mutex_lock(&elem->lock);

            prev->next = elem->next; /*Update the prev's next pointer*/

            pthread_mutex_unlock(&elem->lock);
            pthread_mutex_unlock(&prev->lock);

            /*Push the reservation onto flight's completed_reservations*/
            push(*flight, elem->reservation);

            /*Update elem to the next node*/
            elem = prev->next;
        } else {
            /*No more nodes after sentinel, unlock prev and exit loop*/
            pthread_mutex_unlock(&prev->lock);
            break;
        }
    }
    
    return NULL;
}

void *phase2(void *arg){

    struct flight_reservations *flight = (struct flight_reservations *)arg;
    struct queue_reservation *q=flight->pending_reservations->head;
  
    /*Case 1: Queue not empty, add to central list*/
    if (q != NULL) {
        pthread_mutex_lock(&number_of_inserter_airlines_lock);
        while (q != NULL && number_of_inserter_airlines != 0) {
            add_to_list(q->reservation);
            q = q->next;
            flight->pending_reservations->head = flight->pending_reservations->head->next;
        }
        number_of_inserter_airlines--;
        pthread_mutex_unlock(&number_of_inserter_airlines_lock);
    }else {            
         /*Wait until all inserters finish*/
        while (1) {
            pthread_mutex_lock(&number_of_inserter_airlines_lock);
            if (number_of_inserter_airlines == 0) {
                pthread_mutex_unlock(&number_of_inserter_airlines_lock);
                break;
            }
            pthread_mutex_unlock(&number_of_inserter_airlines_lock);
            usleep(100); /*Sleep for a short period before checking again*/
        }

        /*All adding threads have finished, now remove*/
        removeF(flight);   
    }

    barrier_cross(&phase2_barrier);
    return NULL;
}