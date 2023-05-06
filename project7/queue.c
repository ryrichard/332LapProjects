#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct node
{
	int id;
	int arrivalTime;
	int burstTime;
	int closeTime;
	int turnAroundTime;
	int waitingTime;
	int responseTime;
	struct node* next;
}node;

typedef struct
{
	node *head;
	int size;
}queue;


void init_queue(queue *q)
{
	q->head = NULL;
	q->size = 0;
}

void init_node(node *n, int id, int at, int bt, int ct, int tt, int wt, int rt)
{
	n->id = id;
	n->arrivalTime = at;
	n->burstTime = bt;
	n->closeTime = ct;
	n->turnAroundTime = tt;
	n->waitingTime = wt;
	n->responseTime = rt;
	n->next = NULL;
}

// bool enqueue(queue *q, int id, int at, int bt)
// {
// 	//create new node
// 	// printf("New Node%d(%d, %d)\n", id, at, bt);
// 	node * newNode = malloc(sizeof(node));
// 	if(newNode == NULL) return false;
// 	q->size++;
// 	newNode->id = id;
// 	newNode->arrivalTime = at;
// 	newNode->burstTime = bt;
// 	newNode->next = NULL;

//  	//if queue is empty, newnode is head
//  	if(q->head == NULL)
//  	{
//  		q->head = newNode;
//  	}
//  	else
//  	{
//  		//attach new node to queue
// 		node * tmp  = q->head;
// 		while(tmp->next != NULL)
// 			tmp = tmp->next;

//  		tmp->next = newNode; 
//  	}
//  	return true;
// }

bool enqueue(queue *q, node* n)
{
	//create new node
	// printf("New Node%d(%d, %d)\n", id, at, bt);
	q->size++;
	node * newNode = n;
 	//if queue is empty, newnode is head
 	if(q->head == NULL)
 	{
 		q->head = n;
 	}
 	else
 	{
 		//attach new node to queue
		node * tmp  = q->head;
		while(tmp->next != NULL)
			tmp = tmp->next;

 		tmp->next = newNode; 
 	}
 	return true;
}


int isEmpty(queue *q)
{
	return q->head == NULL ? true : false;
}

// int* dequeue(queue *q)
// {
// 	//create temp node and store values from head
// 	q->size--;
// 	node* tmp = q->head;
// 	int *temp = malloc(sizeof(int)*3);
// 	temp[0] = tmp->id;
// 	temp[1] = tmp->arrivalTime;
// 	temp[2] = tmp->burstTime;
	
// 	//new head is assigned
// 	q->head = q->head->next;
// 	free(tmp);

// 	return temp;
// }

node dequeue(queue *q)
{
	//create temp node and store values from head
	q->size--;
	node* tmp = q->head;

	//new head is assigned
	q->head = q->head->next;

	return *tmp;
}

int getSize(queue *q)
{
	return q->size;
}

void displayQueue(queue *q)
{
	if(isEmpty(q))
		printf("No nodes in queue\n");
	else
	{
		printf("Size = %d\n", getSize(q));
		node *n = q->head;
		while(n != NULL)
		{
			printf("Node%d(%d, %d, %d, %d, %d, %d)\n", n->id, n->arrivalTime, n->burstTime, n->closeTime, n->turnAroundTime, n->waitingTime, n->responseTime);
			n = n->next;
		}
		printf("\n");
	}
}
