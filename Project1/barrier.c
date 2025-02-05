#include "header.h"

void barrier_init(barrier_t *b,int n){
    pthread_cond_init(&b->complete,NULL);
    pthread_mutex_init(&b->mutex,NULL);
    b->count=n;
    b->crossing=0;
}

void barrier_cross(barrier_t *b)
{
    pthread_mutex_lock(&b->mutex);
    b->crossing++; // One more thread through
    if (b->crossing < b->count) { // If not all threads are here, wait
        pthread_cond_wait(&b->complete, &b->mutex);
    } else {
        // Signal all waiting threads to continue
        pthread_cond_broadcast(&b->complete);
        b->crossing = 0; // Reset for the next time
    }
    pthread_mutex_unlock(&b->mutex);
}

void barrier_destroy(barrier_t *b)
{
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->complete);
}
