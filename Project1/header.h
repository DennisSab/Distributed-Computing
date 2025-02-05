#ifndef HEADER_H
#define HEADER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Define ANSI color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 * Structure defining a barrier
*/
typedef struct barrier{
    pthread_cond_t complete;
    pthread_mutex_t mutex;
    int count;
    int crossing;
}barrier_t;


/**
 * Structure defining each available flight 
*/
struct flight_reservations {
    struct stack *completed_reservations;
    struct queue *pending_reservations;
};


/**
 *Struct defining a stack which holds the flights that are completed 
*/
struct stack {
    struct stack_reservation *top;
    pthread_mutex_t top_lock;
    int size;
    int capacity;
};


/**
 * Structure defining each reservation
*/
struct Reservation {
    int agency_id;
    int reservation_number;
};


/**
 * Structure defining each node for the stack
*/
struct stack_reservation {
    struct Reservation reservation;
    struct stack_reservation *next;
};


/**
 * Structure defining a queue which holds the the flights that pending
*/ 
struct queue {
    struct queue_reservation *head;
    struct queue_reservation *tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;
};


/**
 * Structure definig each node for the queue
*/
struct queue_reservation {
    struct Reservation reservation;
    struct queue_reservation *next;
};


/**
 * Structure defining the arguments to pass ,entering functions with threads
*/
struct ThreadsCreateArgs{
    int index;
    struct stack*s;
    struct queue*q;
};


/**
 * Structure defining the aguments to pass ,entering functions with threads
*/
struct args1 {
    int practoryID; 
    struct flight_reservations flight;
};

/**
 * Structure defining a list
*/
struct list{
    struct list_reservation *head;
    struct list_reservation *tail;
};

/**
 * Structure defining each node of a list
*/
struct list_reservation{
    struct Reservation reservation;
    int marked;
    pthread_mutex_t lock;
    struct list_reservation *next;
};

extern int A;                           /* The size of flights*/
extern struct list central_list;        /* Shared central list*/
extern int number_of_inserter_airlines; /* Shared value of number of insester airlines in the central list*/
extern barrier_t phase1_barrier;        /* Barrier for the first phase*/
extern barrier_t controlPhase1;         /* Barrier for the first phase control*/
extern barrier_t phase2_barrier;        /* Barrier for the second phase*/
extern barrier_t controlPhase2;         /* Barrier for the second phase control*/

/**
 * @brief Push the reservation in the stack of this flight
 * 
 * @param flight The flight the reservation gets pushed
 * @param res The reservation
 */
void *push(struct flight_reservations flight,struct Reservation res);

/**
 * @brief Initialization of flights(stacks,queues) ,sentinel node,barriers
 * 
 * @param fligths Pointer to the array of the flights 
 */
void initialize_flight_reservations(struct flight_reservations *flights);

/**
 * @brief phase 1:pushes flights in the stack or enqueues in the queue
 * 
 * @param arg The flight ID and the flight itself  
 */
void *make_reservations(void*arg);

/**
 * @brief Print the queue
 * 
 * @param q the queue 
 */
void print_queue(struct queue *q);

/**
 * @brief Print the stack
 * 
 * @param s the stack 
 */
void print_stack(struct stack *s);

/**
 * @brief print the flights(completed and pending reservations)
 * 
 * @param flights Pointer to the array of flights 
 */
void print_flights(struct flight_reservations *flights);

/**
 * @brief phase1 control
 * 
 * @param arg A pointer to the array of the flights 
 */
void *control(void * arg);

/**
 * @brief phase2 control 
 * 
 * @param arg A pointer to the array of the flights
 */
void *control2(void * arg);

/**
 * @brief Phase2: add or delete from the central list
 * 
 * @param arg A pointer to the array of the flights
 */
void *phase2(void *arg);

/**
 * @brief Finds the non empty queues
 * 
 * @param arg A pointer to the array of the flights 
 * 
 * @return the sum of the non empty queues
 */
int find_non_empty_queues(struct flight_reservations *flights);

/**
 * @brief Function to print a list
 * 
 * @param central_list A pointer to the shared central list 
 */
void print_list(struct list* central_list);

/**
 * @brief Check for each stack overflow
 * 
 * @param flights A pointer to the array of the flights 
 */
void stack_overflow(struct flight_reservations *flights);

/**
 * @brief Check the total keysum
 * 
 * @param flights A pointer to the array of the flights 
 */
void total_keysum_check(struct flight_reservations * flights);

/**
 * @brief Initialize a barrier
 * 
 * @param b The barrier
 * @param n Size of the waiting proccess 
 */
void barrier_init(barrier_t *b,int n);

/**
 * @brief A thread reaches this cross and awaits
 * 
 * @param b The barrier
 */
void barrier_cross(barrier_t *b);

/**
 * @brief Destroy and free the barrier
 * 
 * @param b The barrier 
 */
void barrier_destroy(barrier_t *b);

#endif