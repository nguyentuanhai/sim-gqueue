#ifndef MYQUEUE_H_INCLUDED
#define MYQUEUE_H_INCLUDED

typedef struct queue{
    int arrival;
    int element; // time in queue
    struct queue * next;
}queue;

typedef struct statistics{
    int removed;
    int served;
    int balk;
    double average_wait;
    double p_remove;
    double average_queue_length;
    double average_profit;
}statistics;

typedef struct queue_struct{
    queue * head;
    queue * tail;
    int size;
    int threshold;
    int population;
    double cost;
    double reward;
    double profit;
    statistics stats;
}queue_struct;

void insert(queue **head, queue **tail, int new_element, int state, int arrival, int * size);
int removal(queue_struct * entity, int remove, int time, int * size, int verbose);
void service(queue **head);
void state(int *MMBP, double ALPHA, double BETA);
void print_queue(queue **head);
void read_input(char ** argv, double * input);
void initialize(queue_struct * entity);


#endif // MYQUEUE_H_INCLUDED
