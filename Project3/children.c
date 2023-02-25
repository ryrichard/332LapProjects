/*
Write a program children.c, and let the parent process produce two child processes. One prints
out "I am child one, my pid is: " PID, and the other prints out "I am child two, my pid is: " PID.
Guarantee that the parent terminates after the children terminate (Note, you need to wait for two
child processes here). Use the getpid() function to retrieve the PID
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	// pid_t self_id = getpid();
	// pid_t parent_id = getppid();

	int child01_id = fork(); //create first child process

	if(child01_id == 0) //child01
	{
		printf("-- Child01 --\nId: %d \nParentID: %d\n", 
				getpid(), getppid());
	}
	else 
	{
		int child02_id = fork(); //create second child process

		if(child02_id == 0) //child02
		{
			printf("-- Child02 --\nId: %d \nParentID: %d\n", 
					getpid(), getppid());
		}
		else //self
		{
			wait(1); //to allow the children to finish first
			printf("-- Self --\nId: %d \nParentID: %d\nChild01ID: %d\nChild02Id: %d\n", 
							getpid(), getppid(), child01_id, child02_id);
		}
		
	}

	return 0;
}