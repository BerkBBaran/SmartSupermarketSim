#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "myqueue.h"

int main(){
    int time,*cashiers,cashiernumber,flag;
    Queue arrival_queue,inside_queue,completed_queue;
    arrival_queue= CreateEmptyQueue();
    inside_queue= CreateEmptyQueue();
    completed_queue=CreateEmptyQueue();
    printf("             Welcome to smart market simulator.\n\n Please initiliaze datas:       \n");
    parseInput(arrival_queue);
    createCustomerList(arrival_queue);
    time=0;
    cashiers = (int*) calloc(arrival_queue->number_of_cashiers, sizeof(int));

    while(inside_queue->size != 0 || arrival_queue->size != 0 || flag!=0){

        printf("\n inside:%d  ,  arrival: %d  ",inside_queue->size,arrival_queue->size);

        while(arrival_queue->front->arrival_time == time && arrival_queue->front!=NULL){ //when arrival_queue->size becomes NULL program crashes

            newCustomer(arrival_queue,inside_queue);

        }


        PriortySort(inside_queue);

        if(inside_queue->size > 0){

            cashiernumber=AvaliableCashier(cashiers,arrival_queue);

            if(cashiernumber != -1){

                serveCustomer(inside_queue,completed_queue,cashiernumber,cashiers,time);

            }

        }
        printf("\n |");
        PrintArrivalTimes(arrival_queue);
        printf("|");
        PrintArrivalTimes(inside_queue);
        printf("|");
        PrintArrivalTimes(completed_queue);
        flag=WorkCashiers(cashiers,arrival_queue);
        time++;
    }
    reportStatistics(arrival_queue,completed_queue,time);




return 0;
}
//Main Functions
void parseInput(Queue q){

    printf("\n Enter amount of customers: ");
    scanf("%d",&q->number_of_customers);
    printf("\n Enter max arrival time: ");
    scanf("%d",&q->max_arrival_time);
    printf("\n Enter max service time : ");
    scanf("%d",&q->max_service_time);
    printf("\n Enter number of cashiers: ");
    scanf("%d",&q->number_of_cashiers);
}
void createCustomerList(Queue q){

    int i;
    time_t t;
    Node temp;
    srand((unsigned) time(&t));

    for(i=0;i<q->number_of_customers;i++){

        temp=(Node) malloc(sizeof(struct Node));
        temp->age = rand()%85 + 15;
        if(temp->age < 40)
            temp->prio=3;
        else if(temp->age <= 60)
            temp->prio=2;
        else
            temp->prio=1;
        temp->ID=0;
        temp->arrival_time=rand()%q->max_arrival_time;
        temp->service_time=rand()%q->max_service_time;
        temp->payment=rand()%1;
        enqueue(q,temp);
    }
    ArrivalTimeSort(q);


}
void newCustomer(Queue oldq,Queue newq ){

    Node temp;
    temp=(Node) malloc(sizeof(struct Node));
    temp=oldq->front;
    dequeue(oldq);
    enqueue(newq,temp);

}
void serveCustomer(Queue oldq,Queue newq,int cashiernumber,int *cashiers,int time){

    Node temp;
    temp=(Node) malloc(sizeof(struct Node));
    temp=oldq->front;
    temp->ID=cashiernumber;
    temp->service_start_time=time;
    cashiers[cashiernumber]=temp->service_time;
    dequeue(oldq);
    enqueue(newq,temp);
}
void reportStatistics(Queue arrQ,Queue completedQ,int time){
    Node temp;
    int i,young,adult,elder,avgtime,maxwaiting,flag,cash,credit;
    int *cashiers;
    cashiers = (int*) calloc(arrQ->number_of_cashiers, sizeof(int));
    temp=completedQ->front;
    young=0;adult=0;elder=0;
    avgtime=0;maxwaiting=-1;
    credit=0;cash=0;
    while (temp!= NULL) {
//age groups
        if(temp->prio == 1)
            ++elder;
        else if(temp->prio ==2)
            ++adult;
        else{
        ++ young;
        }
//cashier number
        cashiers[temp->ID]++ ;


        temp->ID;


        temp = temp->next;
    }


    printf("\n The number of Cashiers: %d",arrQ->number_of_cashiers);
    printf("\n The number of Customers: %d",arrQ->number_of_customers);
    printf("\n Customer age groups -> Young:%d ,  Adult:%d  , Elder:%d",young,adult,elder);
    printf("\n Customers per cashier: ");
    for(i=0;i<arrQ->number_of_cashiers;i++){
        printf("Cashier%d: %d  ",i,cashiers[i]);
    }
    printf("\n The completion time: %d ",time);
    temp=completedQ->front;
    for(i=0;i<arrQ->number_of_customers;i++){
        avgtime+= (temp->arrival_time) - (temp->service_start_time)+ (temp->service_time) ;
        temp=temp->next;
    }
    printf("\n Average time spent in the queue: %d ",avgtime / (arrQ->number_of_customers) );
    temp=completedQ->front;
    for(i=0;i<arrQ->number_of_customers;i++){
        flag= (temp->arrival_time) - (temp->service_start_time)+ (temp->service_time) ;
        if(maxwaiting < flag)
            maxwaiting=flag;
        temp=temp->next;
    }
    printf("\n Maximum waiting time: : %d ",maxwaiting);
    temp=completedQ->front;
    for(i=0;i<arrQ->number_of_customers;i++){

        if(temp->payment== 0)
            cash++;
        else
            credit++;

        temp=temp->next;
    }

    if(cash>credit)
        printf("\n Most popular payment type is cash ");
    else
        printf("\n Most popular payment type is credit card ");
}
//Helper Functions

Queue CreateEmptyQueue(void){
    Queue q;
	q = (Queue)malloc(sizeof(struct QueueRecord));
	if(q==NULL){
        printf("Error at - create new queue -");
        exit(0);
    }

    q->front = (Node) malloc(sizeof(struct Node));
	q->front->next = NULL;
	q->rear = q->front;
	q->size = 0;
	return q;
}
void PrintArrivalTimes(Queue q){
    Node temp;
    temp=q->front;
    while (temp!= NULL) {
        printf(" %d ", temp->arrival_time);
        temp = temp->next;
    }

}
void PrintAges(Queue q){
    Node temp;
    temp=q->front;
    while (temp!= NULL) {
        printf(" %d ", temp->age);
        temp = temp->next;
    }
}
void PrintPriorty(Queue q){
    Node temp;
    temp=q->front;
    while (temp!= NULL) {
        printf(" %d ", temp->prio);
        temp = temp->next;
    }
}

void ArrivalTimeSort(Queue q){

    Node min,current,flag,temp;
    temp = q->front;

     while (temp) {

        min = temp;
        flag = temp->next;

        while (flag) {
            if (min->arrival_time > flag->arrival_time)
                min = flag;

            flag = flag->next;
    }
    switcher(min,temp);
    temp=temp->next;


    }
}
void PriortySort(Queue q){
    if(q->size < 2)
        return;

    Node min,current,flag,temp;
    temp = q->front;

     while (temp) {

        min = temp;
        flag = temp->next;

        while (flag) {
            if (min->prio > flag->prio)
                min = flag;

            flag = flag->next;
    }
    switcher(min,temp);
    temp=temp->next;


    }
}
void switcher(Node a,Node b){
    int age;
	int prio;
	int arrival_time;
	int service_time;
	int service_start_time;
	int ID;
	int payment;

    age=a->age;
    prio=a->prio;
    arrival_time=a->arrival_time;
    ID=a->ID;
    payment=a->payment;
    service_start_time=a->service_start_time;


    a->age=b->age;
    a->prio=b->prio;
    a->arrival_time=b->arrival_time;
    a->ID=b->ID;
    a->payment=b->payment;
    a->service_start_time=b->service_start_time;

    b->age=age;
    b->prio=prio;
    b->arrival_time=arrival_time;
    b->ID=ID;
    b->payment=payment;
    b->service_start_time=service_start_time;


}
void enqueue(Queue q,Node c){

    if(q->size==0){
        q->front=c;
        q->rear= c;
        c->next=NULL;
        (q->size)++;
    }
    else{
    q->rear->next= c;
    c->next=NULL;
    q->rear= c;
    q->size++;
    }
}
void dequeue(Queue q){
    if (q->size == 0){
        printf("\n Error ! Queue is empty already ");
    }
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;
    (q->size)--;

}
int AvaliableCashier(int* cashiers,Queue q){
    int i;
    for(i=0; i<q->number_of_cashiers ;i++)
        if(cashiers[i]==0)
            return i;

    return -1;

}
int WorkCashiers(int* cashiers,Queue q){
    int i,flag;
    flag=0;
    for(i=0; i<q->number_of_cashiers ;i++){
        if(cashiers[i]>0){
            (cashiers[i])-- ;
            flag=1;
        }
    }
    return flag;
}

