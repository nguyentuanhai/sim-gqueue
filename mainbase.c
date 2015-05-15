#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "rvgs.h"
#include "myqueue.h"

int main(int argc, char * argv[]){
    int verbose = 1;
    double ALPHA, BETA, LAMBDAM, LAMBDAP, REMOVE, MU, input[8];
    int THRESHOLD, SIM_LENGTH;
    int time = 0; // the clock of the discrete system
    int MMBP = 0; // the state of the MMBP
    int bool_remove = 0; // 1 if there was a removal in the cycle
    int bool_arrive = 0; // 1 if there was an arrival into an EMPTY queue
    int remove = 0; // size of batch remove
    int serve = 0; // time necessary to serve the customer
    queue_struct qs_entity; // the queue

    if(argc != 9){
        printf("Invalid input.\n");
        printf("Correct input: ALPHA BETA LAMBDAM LAMBDAP REMOVE MU THRESHOLD SIM_LENGTH\n");
        return 1;
    }

    // reading input parameters
    read_input(argv, input);
    ALPHA = input[0];
    BETA = input[1];
    LAMBDAM = input[2];
    LAMBDAP = input[3];
    REMOVE = input[4];
    MU = input[5];
    THRESHOLD = input[6];
    SIM_LENGTH = input[7];

    // initialize
    qs_entity.threshold = THRESHOLD;
    PlantSeeds(-1);

    initialize(&qs_entity);
    for(time = 0; time < SIM_LENGTH; time++){
        // SERVICE
        if(qs_entity.head != NULL){ // if there is someone in the queue
                service(&qs_entity.head);
                if(qs_entity.head->element == 0){ // if the service is finished -> remove
                    if(verbose > 1)printf("%03d: SERVED - ",time);
                    removal(&qs_entity, 1, time, &qs_entity.size, verbose);
                    // profit = profit + reward
                    qs_entity.profit += qs_entity.reward;
                    // increment #served
                    qs_entity.stats.served++;
                }
        }

        state(&MMBP,ALPHA,BETA);
        // ARRIVAL
        if(MMBP == 1){
            // POSITIVE
            if(Bernoulli(LAMBDAP)){
                if(qs_entity.size < qs_entity.threshold){ // if we have not reached the threshold level yet
                    // JOIN
                    serve = 1 + Geometric(1-MU);
                    //serve = 1;
                    if(verbose > 1)printf("%03d: ARRIVAL, %d\n",time,serve);
                    if(qs_entity.size == 0)bool_arrive = 1; // arrival into an empty queue
                    insert(&qs_entity.head, &qs_entity.tail, serve, MMBP, time, &qs_entity.size);
                }else{ // if the queue size has reached threshold
                    // BALK
                    ++qs_entity.stats.balk;
                    if(verbose > 1)printf("%03d: BALK, %d\n",time,qs_entity.stats.balk);
                }
            }
        }
        else{
            // NEGATIVE
            if(Bernoulli(LAMBDAM)){
                remove = 1 + Geometric(1-REMOVE);
                if(verbose > 1)printf("%03d: REMOVAL, %d - ",time,remove);
                qs_entity.stats.removed += removal(&qs_entity, remove, time, &qs_entity.size, verbose);
            }
        }
        qs_entity.stats.average_queue_length = ((time * qs_entity.stats.average_queue_length)+qs_entity.size)/(time+1);
        if(verbose > 2){
            printf("%03d: ",time);
            print_queue(&qs_entity.head);
            printf("\n");
        }
    }
    if(verbose > 0){
        printf("===== SIMULATION END =====\n");
        printf("Threshold: %d\n",qs_entity.threshold);
        printf("ALPHA: %.2lf, BETA: %.2lf, LAMBDAM: %.2lf, LAMBDAP: %.2lf, RHO: %.2lf, MU: %.2lf\n", ALPHA, BETA, LAMBDAM, LAMBDAP, REMOVE, MU);
        printf("Cost: %.2lf, Reward: %.2lf\n", qs_entity.cost, qs_entity.reward);
        printf("Queue at the end of simulation: "); print_queue(&qs_entity.head);
        printf("Removed: %d customer(s).\n",qs_entity.stats.removed);
        printf("Served: %d customer(s).\n",qs_entity.stats.served);
        printf("Balk: %d customer(s).\n",qs_entity.stats.balk);
        printf("Average Profit: %03lf.\n",qs_entity.profit/(qs_entity.stats.removed+qs_entity.stats.served));
        printf("Average Queue Length: %03lf.\n",qs_entity.stats.average_queue_length);
        printf("Average Wait: %03lf.\n",qs_entity.stats.average_wait/(qs_entity.stats.removed + qs_entity.stats.served));
        printf("Average Remove: %03lf.\n",(double)qs_entity.stats.removed/(qs_entity.stats.removed+qs_entity.stats.served));
    }
    if(verbose > 0)printf("==========================\n");
    if(verbose >= 0)printf("The threshold strategy is: %d.\n",qs_entity.threshold - 1);
    return 0;
}
