/*
Create two files namely, destination1.txt and destination2.txt with read,
write permissions.
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main()
{

	int fd;

	fd = open("destination1.txt", O_CREAT | O_RDWR, 0666);
	if(fd == -1)
	{
		perror("open");
		return 0;
	}

	fd = open("destination2.txt", O_CREAT | O_RDWR, 0666);
	if(fd == -1)
	{
		perror("open");
		return 0;
	}

	char* comm[] = {"ls", "-a", "-l", NULL};
	execvp("ls", comm);

	return 0;
}