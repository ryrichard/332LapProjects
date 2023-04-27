#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

#define THREAD_NUM 3
#define Deal 10
int attempt = 10;
sem_t matchSmoker;
sem_t paperSmoker;
sem_t tobSmoker;
sem_t agent;
int matchSmoke = 0;
int tobSmoke = 0;
int paperSmoke = 0;
int *num;

pthread_mutex_t buffer_mutex;

void* matchSmokeSeq (void* args)
{
	while(1 && attempt > 0)
	{
		sem_wait(&matchSmoker);
		if(attempt > 0)
		{
			pthread_mutex_lock(&buffer_mutex);
			matchSmoke++;
			printf("Attempt #%d - Match smoked: %d\n", attempt, matchSmoke);
			attempt--;
			pthread_mutex_unlock(&buffer_mutex);
		}
		sem_post(&agent);
		// free(args);
	}
}

void* paperSmokeSeq (void* args)
{
	while(1 && attempt > 0)
	{
		sem_wait(&paperSmoker);
		if(attempt > 0)
		{
			pthread_mutex_lock(&buffer_mutex);
			paperSmoke++;
			printf("Attempt #%d - Paper smoked: %d\n", attempt, paperSmoke);
			attempt--;
			pthread_mutex_unlock(&buffer_mutex);
		}
		sem_post(&agent);
		// free(args);
	}
}

void* tobSmokeSeq (void* args)
{
	while(1 && attempt > 0)
	{
		sem_wait(&tobSmoker);
		if(attempt > 0)
		{	
			pthread_mutex_lock(&buffer_mutex);
			tobSmoke++;
			printf("Attempt #%d - Tob smoked: %d\n", attempt, tobSmoke);
			attempt--;
			pthread_mutex_unlock(&buffer_mutex);
			// free(args);
		}
		sem_post(&agent);
	}
}



int main()
{
	srand(time(NULL));
	pthread_t matchThread;
	pthread_t paperThread;
	pthread_t tobThread;

	pthread_mutex_init(&buffer_mutex, NULL);
	sem_init(&matchSmoker, 0, 0);
	sem_init(&paperSmoker, 0, 0);
	sem_init(&tobSmoker, 0, 0);

	sem_init(&agent, 0 ,0);
	int randNum;
	num = malloc(sizeof(int));
	//CRAETE THE 3 Smokers
	pthread_create(&matchThread, NULL, &matchSmokeSeq, NULL);
	pthread_create(&paperThread, NULL, &paperSmokeSeq, NULL);
	pthread_create(&tobThread, NULL, &tobSmokeSeq, NULL);

	while(attempt > 0)
	{
		randNum = rand() % 3 + 1;
		*num = randNum;
		printf("Agent picked: %d\n", *num);
		if(*num == 1)
			sem_post(&matchSmoker);
		else if(*num == 2)
			sem_post(&paperSmoker);
		else
			sem_post(&tobSmoker);
		sem_wait(&agent);
	}

	printf("Agent is finished\n");
	sem_post(&matchSmoker);
	sem_post(&paperSmoker);
	sem_post(&tobSmoker);

	if(pthread_join(matchThread, NULL) != 0)
		perror("Failed to join Match Thread");
	if(pthread_join(paperThread, NULL) != 0)
		perror("Failed to join Match Thread");	
	if(pthread_join(tobThread, NULL) != 0)
		perror("Failed to join Match Thread");

	printf("Match smoked : %d\nPaper smoked : %d\nTob smoked : %d\n", matchSmoke, paperSmoke, tobSmoke);

	return 0;
}