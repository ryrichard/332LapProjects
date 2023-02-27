/*
Once you’re done with successful creation of executables for the above two steps do the following.
Write a C program and call it Parent_Prcs.c. Execute the files as per the following procedure
using execv system call.
[Step 3] Using fork create a child process, say Child 1 that executes Prcs_P1. This will create
two destination files according to Step 1.
[Step 4] After Child 1 finishes its execution, use fork to create another child process, say Child
2 and execute
Prcs_P2 that accomplishes the procedure described in Step 2.
Note that: source.txt is already provided in the previous lab. Use that for task 4.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main()
{
	pid_t child01_id;
	pid_t child02_id;
	int wstatus;
	char* comm[] = {NULL};

	child01_id = fork();

	if(child01_id == 0) 
	{
		printf("Child01 [%d] running\n", getpid());
		if(execv("./Prcs_P1", comm) == -1)
		{
			printf("Error in Child01 : [%s]\n", strerror(errno));
			return -1;
		}
	}
	else
	{
		waitpid(child01_id, &wstatus, 0);
		child02_id = fork();

		if(child02_id == 0)
		{
			printf("Child02 [%d] running\n", getpid());
			if(execv("./Prcs_P2", comm) == -1)
			{
				printf("Error in Child02 : [%s]\n", strerror(errno));
				return -1;
			}

		}
		else
		{
			waitpid(child02_id, &wstatus, 0);
			printf("Child01_id : %d\nChild02_id: %d\n",child01_id, child02_id);
		}
	}
	

	return 0;
}