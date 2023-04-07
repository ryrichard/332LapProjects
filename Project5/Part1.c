/*
Write a special simple command interpreter that takes a command and its arguments.
This interpreter is a program where the main process creates a child process to execute
the command using exec() family functions. After executing the command, it asks for
a new command input ( parent waits for child). The interpreter program will get
terminated when the user enters exit.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

int main()
{
	bool pipeline;
	int NumOfInputs, NumOfInputs2;
	size_t pid1, pid2;
	char *token;
	char *search = " ";
	char input[100];
	char* cmd[10];
	char* cmd2[10];
	int fd[2];

	printf("Command: ");
	scanf("%[^\n]%*c", input);

	if(pipe(fd)==-1)
	{
		perror("Pipe Error");
		exit(1);
	}

	while(strcmp(input, "exit") != 0)
	{
		pipeline = false;

		//turn input into string list
		token = strtok(input, search);
		// printf("token: %s\n", token);
		cmd[0] = (char*)malloc(strlen(token)+1);
		strcpy(cmd[0], token);
		NumOfInputs = 1;
		while((token = strtok(NULL, search)) != NULL && strcmp(token, "|") != 0)
		{
			// printf("token: %s\n", token);
			cmd[NumOfInputs] = (char*)malloc(strlen(token)+1);
			strcpy(cmd[NumOfInputs], token);
			NumOfInputs++;
		}
		cmd[NumOfInputs] = NULL; 
	
		NumOfInputs2 = 1;
		//if "|" is found, a new command list needs to be made
		if(token != NULL && strcmp(token, "|") == 0)
		{
			pipeline = true;
			while((token = strtok(NULL, search)) != NULL)
			{
				// printf("token2: %s\n", token);
				cmd2[NumOfInputs2] = (char*)malloc(strlen(token)+1);
				strcpy(cmd2[NumOfInputs2], token);
				NumOfInputs2++;
			}
		cmd2[NumOfInputs2] = NULL;
		}


		//outputs cmd list
		// for(int i = 0; i < NumOfInputs; i++)
        // 	printf("%s\n", cmd[i]);

		pid1 = fork(); //child 1 created
		if(pid1 < 0)
		{
			perror("fork");
			exit(1);
		}
		if(pid1 == 0)
		{
			// printf("Child 1 Process\n");
			if(pipeline)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmd[0], cmd);
			perror("execvp failure");
			exit(1);
		}

		if(pipeline)
		{
			pid2 = fork(); //child 2 created
			if(pid2 < 0)
			{
				perror("fork");
				exit(1);
			}

			if(pid2 == 0)
			{
				// printf("Child 2 Process\n");
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(cmd2[0], cmd2);
				perror("execvp failure");
				exit(1);
			}
		}

		close(fd[0]);
		close(fd[1]);

		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);

		memset(input, 0, sizeof(input)); //empty char array

		printf("Command: ");
		scanf("%[^\n]%*c", input);
	}




	return 0;
}