/*
Write a program with 2 children process, where one child is created to execute command that tells you the date and time in Unix.
Use execl(...).
Note, you need to specify the full path of the file name that gives you date and time information.
Announce the successful forking of child process by displaying its PID.
And another child is created to execute a command that shows all files (including hid-den files) in a
directory with information such as permissions, owner, size, and when last modified.
Use execvp(...).
For the command to list directory contents with various options, refer the handout on Unix
filesystem sent to you in the first class.
Announce the successful forking of child process by displaying its PID.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{

	// int x = execl("/usr/bin/dte", "date", NULL);
	// printf("%d\n", x);

	int child01_id;
	int child02_id;
	int error;

	if(child01_id = fork())
	{
		if(child02_id = fork()) 
		{
			printf("Child01 : %d\n", child01_id);	
			printf("Child02 : %d\n", child02_id);
		}
		else 
		{	
			printf("Child02_id: %d\n", getpid());
			char* comm[] = {"ls", "-a", "-l", NULL};
			error = execvp("ls", comm);
			if(error == -1) strerror(errno);
		}
	}
	else
	{
		printf("Child01_id: %d\n", getpid());
		error = execl("/usr/bin/date", "date", NULL);
		if(error == -1) strerror(errno);
	}

	return 0;
}