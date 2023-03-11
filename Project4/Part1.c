/*
Part 1 Open a file called readme.txt in the child process, read the contents and pass to the parent
process using Pipe. Parent process will write to readme.txt, “Parent is writing:” and write the contents it
received from the child to the readme.txt file.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <memory.h>

int main()
{
	char* readme = "readme.txt";
	char c[50];
	int fileread, input, byte_write;
	int pipefd[2]; 		//0 - read, 1 - write
	pid_t cpid;

	input = open(readme, O_RDWR, 0666);
	if(input == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	if(pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork();
	if(cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(cpid == 0) //child writes to parent text from readme
	{
		close(pipefd[0]);

		fileread = read(input, &c, sizeof(c));
		
		if(fileread == -1)
			printf("Error while reading: [%s]\n", strerror(errno));


		printf("Child: %s\n", c);
		write(pipefd[1], &c, sizeof(c));
		
		close(pipefd[1]);
		_exit(EXIT_SUCCESS);

		// while(read(pipefd[0], &buf, 1) > 0)
		// 	write(STDOUT_FILENO, &buf, 1);
	}
	else //parent reads from child and appends the readme
	{

		close(pipefd[1]);
		wait(NULL);

		read(pipefd[0], &c, sizeof(c));

		char* c1 = "\nParent is writing: ";
		char* c2 = (char *)malloc(1 + strlen(c) + strlen(c1));
		strcpy(c2, c1);
		strcat(c2, c);

		printf("Text: %s\n", c2);

		byte_write = write(input, c2, strlen(c2));

		if(byte_write == -1)
			printf("Error while writing: [%s]\n", strerror(errno));

		close(pipefd[0]);
		exit(EXIT_SUCCESS);

	}

	return 0;
}