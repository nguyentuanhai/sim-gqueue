#include <stdio.h>
#include <stdlib.h>
#include "myqueue.h"

void insert(queue **head, queue **tail, int new_element, int state, int arrival, int * size){
    queue * now;
    if((*head) == NULL){
        (*head) = (queue *)malloc(sizeof(queue));
        (*head)->element = new_element;
        (*head)->arrival = arrival;
        (*head)->next = NULL;
        (*tail) = (*head);
    }else{
        now = (queue *)malloc(sizeof(queue));
        now->element = new_element;
        now->arrival = arrival;
        now->next = NULL;
        (*tail)->next = now;
        (*tail) = (*tail)->next;
    }
    (*size)++;
}

int removal(queue_struct * entity, int remove, int time, int * size, int verbose){
    queue *now;
    queue ** head = &(*entity).head;
    int removed = 0, i = 0;
    if((*head) != NULL){
        if(remove > *size){
            removed = *size;
        }else{
            removed = remove;
        }
        if((removed > 0)&&(verbose > 1))printf("WAIT ");
        for(i = 0; i < removed; i++){
            now = *head;
            if(verbose > 1)printf("%d, ",(time - now->arrival));
            (*entity).stats.average_wait += time - now->arrival;
            (*entity).population++;
            (*entity).profit -= (time - now->arrival) * (*entity).cost;
            *head = now->next;
            free(now);
        }
        *size -= removed;
    }
    if(verbose > 1)printf("\n");
    return removed;
}

void service(queue **head){
    if((*head) != NULL){
        if((*head)->element > 0){
            (*head)->element--;
        }
    }
}

void state(int *MMBP, double ALPHA, double BETA){
    if(*MMBP == 1){
        if(!Bernoulli(ALPHA))
            *MMBP = 0;
            //printf("Change state to 0: \n");
    }else{
        if(!Bernoulli(BETA))
            *MMBP = 1;
            //printf("Change state to 1: \n");
    }
}

void print_queue(queue **head){
    queue *now;
    if((*head) != NULL){
        for(now = *head; now->next != NULL; now = now->next)printf("%d, ",now->element);
        printf("%d\n",now->element);
    }else{
        printf("The queue is empty.\n");
    }
}

void read_input(char ** argv, double * input){
    int i;
    for(i = 0; i < 7; i++){
        if(!(input[i] = atof(argv[i+1]))){
            printf("%d. parameter is invalid.\n",i);
            exit(2);
        }
    }
    if(!(input[7] = atoi(argv[8]))){
        printf("7. parameter is invalid.\n");
        exit(2);
    }
}

void initialize(queue_struct * entity){
    (*entity).head = NULL;
    (*entity).tail = NULL;
    (*entity).profit = 0.0;
    (*entity).size = 0;
    (*entity).population = 0;
    (*entity).stats.average_queue_length = 0;
    (*entity).stats.average_wait = 0;
    (*entity).stats.p_remove = 0;
    (*entity).stats.served = 0;
    (*entity).stats.removed = 0;
    (*entity).stats.balk = 0;
    (*entity).stats.average_profit = 0;
    // parameters
    (*entity).cost = 3.0;
    (*entity).reward = 10.0;
    //(*entity).threshold = 3;
}
