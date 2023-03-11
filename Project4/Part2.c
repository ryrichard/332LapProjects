/*
Part 2 Write a program where a Parent process sends 2 signals to the child process, Child
process catches two signals and prints out that it received the signal. And then child process
terminates.
*/

#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int received1 = 0;
static int received2 = 0;

void sigusr1_handler(int sig)
{
	if(sig == SIGUSR1)
	{
		received1 = 1;
		printf("Sending Signal 1: %d\n", sig);
	}
}

void sigusr2_handler(int sig)
{
	if(sig == SIGUSR2)
	{
		received2 = 1;
		printf("Sending Signal 2: %d\n", sig);
	}
}

int main()
{

	signal(SIGUSR1, sigusr1_handler);
	signal(SIGUSR2, sigusr2_handler);

	pid_t cpid;
	cpid = fork();
	if(cpid == 0) 	//child
	{
		printf("Process 2, pid = %d\n", getpid());
		while(!received1);
		printf("Signal 1 received\n");
		while(!received2);
		printf("Signal 2 received\n");
		printf("Process 2 ending\n");
		_exit(EXIT_SUCCESS);
	}
	else			//parent
	{
		printf("Process 1, pid = %d\n", getpid());
		printf("Sending 2 signals\n");
		kill(cpid, SIGUSR1);
		kill(cpid, SIGUSR2);
		waitpid(cpid, NULL, 0);
		printf("Process 1 ending\n");
		exit(EXIT_SUCCESS);
	}



	return 0;
}