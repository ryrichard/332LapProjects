#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "queue.c"

int main(int argc, char* argv[])
{
	// srand(time(NULL));
	// if(argc <= 1)
	// {
	// 	printf("Invalid input\n");
	// 	exit(1);
	// }

	// int numOfProcesses = atoi(argv[1]);
	int numOfProcesses = 6;
	int quantumTime = 4;

	queue q;
	queue qEnd;
	init_queue(&q);
	init_queue(&qEnd);

	int ATArray[6] = {0, 2, 3, 4, 7, 9};
	int BTArray[6] = {7, 2, 8, 6, 3, 5};
	int array[6][6] = {{0,7}, {2,2}, {3,8}, {4,6}, {7,3}, {9,5}};

	int nodesConsidered = 0;
	int time = 0;
	node tmp;
	node * temp = malloc(sizeof(node));
	init_node(temp, nodesConsidered++, array[0][0], array[0][1], 0, 0, 0, 0);
	enqueue(&q, temp);

	while(!isEmpty(&q) || nodesConsidered < numOfProcesses)
	{
		printf("Start Time: %d\n", time);

		//pop first node
		tmp = dequeue(&q);
		printf("Popped Node%d(%d, %d)\n", tmp.id, tmp.arrivalTime, tmp.burstTime);
		
		if(tmp.closeTime == 0)
		{
			tmp.closeTime = 1;
			tmp.responseTime = time;
		}


		//consider time
		if(tmp.burstTime > quantumTime)
		{
			time += quantumTime;
		}
		else
		{
			time += tmp.burstTime;
		}

		//Add new nodes not considered
		while(nodesConsidered < numOfProcesses && time >= array[nodesConsidered][0])
		{
			node* newNode = malloc(sizeof(node));
			init_node(newNode, nodesConsidered, array[nodesConsidered][0], array[nodesConsidered][1], 0, 0, 0, 0);
			enqueue(&q, newNode);
			printf("Added new node #%d\n", nodesConsidered);
			nodesConsidered++;
			displayQueue(&q);
		}

		//Decide what to do with current popped node
		//if BT > QT, add node back to stack, increase time += QT
		if(tmp.burstTime > quantumTime)
		{
			tmp.burstTime -= quantumTime;
			printf("Returning Node%d(%d, %d)\n", tmp.id, tmp.arrivalTime, tmp.burstTime);
			node * newNode = malloc(sizeof(node));
			init_node(newNode, tmp.id, tmp.arrivalTime, tmp.burstTime, tmp.closeTime, tmp.turnAroundTime, tmp.waitingTime, tmp.responseTime);
			enqueue(&q, newNode);
		}
		else //if BT <= QT, dont add node, increate time += BT 
		{
			printf("Removing Node%d(%d, %d)\n", tmp.id, tmp.arrivalTime, tmp.burstTime);
			node * newNode = malloc(sizeof(node));
			//use tmp.id to find original bt
			init_node(newNode, tmp.id, tmp.arrivalTime, array[tmp.id][1], time, time - tmp.arrivalTime, (time - tmp.arrivalTime) - array[tmp.id][1], tmp.responseTime - tmp.arrivalTime);
			enqueue(&qEnd, newNode);
		}
		displayQueue(&q);
		printf("End Time: %d\n", time);
	}
	printf("\nEnd Queue\n");
	displayQueue(&qEnd);


	return 0;
}