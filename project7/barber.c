#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

//wait room chair:	 	n
//barber char:	 		1
//barber cuts hair:		3secs
//program should run atleast 1min

//---cases---
//no customers = barber sleeps
//if customer comes in and all waiting chair is occupied, customer leaves
//if barber is busy, but waiting chair is available, then customer sits
//if barber is asleep, then customer wakes barber

#define WAITING_CHAIR 5
int availableChair = WAITING_CHAIR;
pthread_mutex_t buffer_mutex;
sem_t barberSem;
sem_t customerSem;
sem_t customerChairSem;
int customerWaitingFlag = 0;
int closingTime = 0;

void* barberAction()
{
	while(closingTime == 0 || availableChair < WAITING_CHAIR)
	{
		sem_wait(&barberSem);
		pthread_mutex_lock(&buffer_mutex);
		printf("Barber cutting a customer's hair\n");
		sleep(3);
		pthread_mutex_unlock(&buffer_mutex);
		sem_post(&customerSem);		
	}
}

void* customerWaitingRoom()
{
	while(1)
	{
		sleep(1);
		sem_wait(&customerChairSem);
		if(availableChair > 0 && closingTime == 0)
		{
			printf("Customer arriving and sitting down in waiting chair %d\n", availableChair);
			availableChair--;
		}	
		else if(availableChair == WAITING_CHAIR)
		{
			printf("Customer arriving and immediatly leaving\n");
		}
		else if(closingTime == 1)
			break;
	}
	printf("Waiting room is closed\n");
}


int main()
{
	pthread_t barberThread;
	pthread_t customerThread;

	pthread_mutex_init(&buffer_mutex, NULL);
	sem_init(&barberSem, 0, 0);
	sem_init(&customerSem, 0, 1);
	sem_init(&customerChairSem, 0, WAITING_CHAIR);

	pthread_create(&barberThread, NULL, &barberAction, NULL);
	pthread_create(&customerThread, NULL, &customerWaitingRoom, NULL);


	time_t startTime;
	time_t now;
	float elapsedTime;
	float setTime = 10; //units in seconds
	time(&startTime);

	while(elapsedTime < setTime || availableChair < WAITING_CHAIR)
	{
		//customer is waiting
		if(availableChair < WAITING_CHAIR)
		{
			sem_wait(&customerSem);
			sem_post(&barberSem);

			//if there is time and space, allow more customers to sit
			if(elapsedTime < setTime)
			{
				sem_post(&customerChairSem);
			}	
			else if(closingTime == 0)
			{
				closingTime = 1;
				sem_post(&customerChairSem);
			}

			availableChair++;
		}
		// printf("Available Chairs: %d\n", availableChair);

		now = time(NULL);
		elapsedTime = difftime(now, startTime);	
		// printf("Time: %f\n", elapsedTime);

	}

	if(pthread_join(barberThread, NULL) != 0)
		perror("Failed to join Barber Thread\n");
	if(pthread_join(customerThread, NULL) != 0)
		perror("Failed to join Customer Thread\n");
		
	printf("End of day, no more customers arriving and\nBarber going home\n");

	return 0;
}