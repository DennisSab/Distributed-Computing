#include "header.h"

int number_of_inserter_airlines;
int A;
barrier_t phase1_barrier;
barrier_t controlPhase1;
barrier_t phase2_barrier;
barrier_t controlPhase2;


int main(int argc,char* argv[]){

    A=atoi(argv[argc-1]);
    struct flight_reservations *flights=(struct flight_reservations*)malloc(A*sizeof(struct flight_reservations));
    pthread_t practories[A*A];
    pthread_t controller;
    pthread_t AirComp[A]; 
    
    initialize_flight_reservations(flights);


    struct args1 * argument=(struct args1*)malloc(A*A*sizeof(struct args1));
    for(int i=0;i<(A*A);i++){
        argument[i].practoryID=i;
        argument[i].flight=flights[i%A];
        if(pthread_create(&practories[i],NULL,make_reservations,(void *)&argument[i])){
            fprintf(stderr,"Error creating thread %d\n",i);
            return 1;
        }
    }

    for(int i = 0; i < (A * A); i++) {
        pthread_join(practories[i], NULL);
    }
        

    /*print_flights(flights);*/


    
    /*---------------------------------*/
    /*control phaseA*/ 

    if(pthread_create(&controller,NULL,control,(void *) flights)){
        fprintf(stderr,"Error creating thread Controller\n");
        return 1;
    }

    number_of_inserter_airlines=find_non_empty_queues(flights);

    pthread_join(controller,NULL);

    /*----------------------------------*/
    /*phase B*/

    printf("\n PHASE B ----------------------------\n\n");


    for(int i=0;i<(A);i++){
        if(pthread_create(&AirComp[i],NULL,phase2,(void *) &flights[i])){
            fprintf(stderr,"Error creating thread %d\n",i);
            return 1;
        } 
    }

    /*Wait for phase B completion*/
    for(int i = 0; i < A; i++) {
        pthread_join(AirComp[i], NULL);
    }

    /*print_flights(flights);*/

    /*--------------------------*/
    /*phase B check*/



    if(pthread_create(&controller,NULL,control2,(void *) flights)){
        fprintf(stderr,"Error creating thread Controller\n");
        return 1;
    }

    pthread_join(controller,NULL);

    /*print the central list*/
    /*print_list(&central_list);*/

    barrier_destroy(&phase1_barrier);
    barrier_destroy(&phase2_barrier);
    barrier_destroy(&controlPhase1);
    barrier_destroy(&controlPhase2);

    return 0;
}