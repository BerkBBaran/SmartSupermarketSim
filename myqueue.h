#ifndef myqueue.h
#define myqueue.h

struct Node
{
	int age;
	int prio; // priorty young(15-40) :3 adult(40 <= age <= 60): 2 eldrly(60-100): 1
	int arrival_time;
	int service_time;
	int service_start_time;
	int ID;   //cashier ID
	int payment; //cash:0 card:1
	struct Node *next;
};
typedef struct Node *Node;

struct QueueRecord
{
	struct Node *front;
	struct Node *rear;
    int number_of_cashiers;
	int max_arrival_time;
	int number_of_customers;
	int max_service_time;
	int size;
};
typedef struct QueueRecord *Queue;

Queue CreateEmptyQueue(void);
void PrintArrivalTimes(Queue q);
void PrintAges(Queue );
void PrintPriorty(Queue );
void ArrivalTimeSort(Queue );
void PriortySort(Queue );
void switcher(Node ,Node );
void enqueue(Queue ,Node );
void dequeue(Queue );
int AvaliableCashier(int*,Queue);
int WorkCashiers(int*,Queue);
void parseInput(Queue q);
void createCustomerList(Queue q);
void newCustomer(Queue oldq,Queue newq);
void serveCustomer(Queue oldq,Queue newq,int cashiernumber,int *cashiers,int time);
void reportStatistics(Queue arrQ,Queue completedQ,int time);


#endif
