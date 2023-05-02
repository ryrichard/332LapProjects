#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

#define ROWS 3
#define COLS 3

void display2d(int array[ROWS][COLS])
{
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			printf("%i ", array[i][j]);
		}
		printf("\n");
	}
}

void display1d(int* resourceAvailable)
{
	for(int i = 0; i < COLS; i++)
		printf("%d ",resourceAvailable[i]);
	printf("\n");
}

bool need(int* allocated, int* resourceNeeded, int* resourceAvailable)
{
	for(int i = 0; i < COLS; i++)
	{
		if((resourceNeeded[i] - allocated[i]) > resourceAvailable[i]) //return needed is larger than resource available
		{
			return false;
		}
	}
	return true;
}

void reallocateResource(int* allocated, int* resourceAvailable)
{
	for(int i = 0; i < COLS; i++)
	{
		resourceAvailable[i] += allocated[i];
	}
}

int main()
{
	char* allocatedResource = "allocatedResource.txt";
	char* maxResource = "maxResource.txt";

	int numOfProcess = 3;
	int numOfDiffResource = 3;
	//current state of the process
	int pState[numOfProcess];
	memset(pState, 0, numOfProcess*sizeof(int));
	//how many resource the process already has
	int pAllocation[numOfProcess][numOfProcess];
	memset(pAllocation, 0, numOfProcess*numOfProcess*sizeof(int));
	//how many resource the process needs
	int pMax[numOfProcess][numOfProcess];
	memset(pMax, 0, numOfProcess*numOfProcess*sizeof(int));
	int availResource[numOfDiffResource];
	memset(availResource, 0, numOfDiffResource*sizeof(int));
	int safeSequence[numOfProcess];
	memset(safeSequence, 0, numOfProcess*sizeof(int));

	char c = 0;
	char *token;
	char *search = " ";
	int file;

	//set the pState
	for(int i = 0; i < numOfProcess; i++)
	{
		pState[i] = false;
	}

	availResource[0] = 1;
	availResource[1] = 1;
	availResource[2] = 2;

	//read allocatedResource.txt and add their input into pAllocation
	file = open(allocatedResource, O_RDONLY);
	if(file == -1)
	{
		printf("Error: [%s]\n", strerror(errno));
		exit(1);
	}
	else
	{
		int input = 0;
		int line = 0;
		while(read(file, &c, 1) != 0)
		{
			if(c == '\n') 
			{
				line++;
			}
			if(isdigit(c))
			{
				// printf("Adding %d to [%d][%d]\n", (int)c - '0', input, line);
				pAllocation[line][input % numOfProcess] = (int)c - '0';
				input++;
			}
		}
	}
	// display2d(pAllocation);

	//read maxResource.txt and add their input into pMax
	file = open(maxResource, O_RDONLY);
	if(file == -1)
	{
		printf("Error: [%s]\n", strerror(errno));
		exit(1);
	}
	else
	{
		int input = 0;
		int line = 0;
		while(read(file, &c, 1) != 0)
		{
			if(c == '\n') 
				line++;
			if(isdigit(c))
			{
				// printf("Adding %d to [%d][%d]\n", (int)c - '0', input, line);
				pMax[line][input % numOfProcess] = (int)c - '0';
				input++;
			}
		}
	}
	// display2d(pMax);
	close(file);

	int flag = false;
	int counter = 0;
	//Check if there is a deadlock
	while(flag == 0)
	{
		for(int i = 0; i < numOfProcess; i++)
		{
			if(pState[i] == false && need(pAllocation[i], pMax[i], availResource))
			{
				printf("Resource available for %d\n", i);
				reallocateResource(pAllocation[i], availResource);
				safeSequence[counter] = i;
				// display1d(availResource);
				pState[i] = true;
				counter++;
				flag = true;
			}
		}
		if(flag == true)
		{
			flag = 0;
		}
		else if(flag == false)
		{
			break;
		}
	}

	if(counter < numOfProcess)
	{
		printf("System is not safe, deadlock can occur\n");
	}
	else
	{
		printf("System is safe\nOrder Of Sequence:\n");
		for(int *i = safeSequence; i < safeSequence + COLS; i++)
		{
			printf("[%d]",*i);
			display1d(pAllocation[*i]);
		}
	}








	return 0;
}



/*
Algorithm:
1. P[0….n-1] =[False….False]
2. SafeSequnce[n], Counter=0
3. Flag=0
4. While (flag ==0) :
For i from P[i….n-1]:
If P[i] == False & (Need(P[i]) <= Available
a. Means that, we can execute that process and after
execution all resources allocated by that process will
be free. So we can add that to the available
b. Avaiable = Available + Allocation
c. P[i] Goes to safe sequence. Means P[i]= true
d. SafeSequence[counter]=i
e. Counter = counter+1
f. Flag =1
If( Flag ==1){
Flag=0
}
Else if( Flag ==0) {
break
}
5. If counter < n: print( System not safe, deadlock can occur)
6. Else print ( System is safe) and print the safeSequence,
*/