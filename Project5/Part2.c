/*
There are n (n>1) students enrolled in a course. The course covers x number of
chapters from a textbook (x > 1).
In each chapter y number of homeworks are assigned (y>=1). The average grade for each
homework in all the chapters need to be found out.
To solve this, write program which has the main process as Director process, which
reads a file containing grades of all homeworks of all chapters and creates x number of
Manager processes. Each Manager process will take care of solving a chapter. Each
manager process will create y number of Worker process and pass marks of n students
to each of them and they calculate and print the average.
The input file should contain the data according to the value of x and y and n. For
example, the input text file and the process tree for x = 2 and y = 2 and n= 10 will look
like the following:
*/

/*
Notes: 
1) I had trouble figuring out how to determine the number of chapters based on the given text file from quiz_grades.txt. So a default is set. it can also just be typed
2) This program only works if for every line in quiz_grades.txt, it ends with a ' '
3) Also for whatever reason, my program would randomly miss a ' ', meaning the number of workers is less than 1 from the correct amount. No idea why it happens.
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc, char* argv[]) //argv = {filename, chapter};
{
	if(argc != 3)
	{
		// printf("%d\n", argc);
		printf("Error: Please enter appropriate input:\nexe filename chapter\n");
		exit(1);
	}
	int x = strtol(argv[2], NULL, 0); //number of chapters. No idea where this info comes from or how to derive it just from quiz_grades.txt So for now, jsut set it

	char *token;
	char *search = " ";
	bool stopCount = false;
	//read file
	char* filename = argv[1];
	const int LEN = 1024;
	char c = 0;
	int fileread;
	int file;

	file = open(filename, O_RDONLY);
	int NumOfLines = 1; //for files where there is no new line after last line of info
	int NumOfSpaces = 0;  
	int LenOfOneLine = 0;

	if(file == -1)
	{	
		printf("Error: [%s]\n", strerror(errno));
		exit(1);
	}
	else
	{
        while((fileread = read(file, &c, 1)) != 0) // count new lines
		{
			// if(c == ' ')
			// {
			// 	printf("S");
			// }
			// else
			// 	printf("%c", c);

			if(!stopCount) LenOfOneLine++; //find length of one line to properly count spaces
			if(c == '\n')
			{
				NumOfLines++;
				stopCount = true;
			}
		}
		close(file);
		// printf("\nLength of One Line: %d\n", LenOfOneLine);
		file = open(filename, O_RDONLY); //return to beginning
		char line[LenOfOneLine];

		read(file, &line, LenOfOneLine);
		for(int i = 0; i < LenOfOneLine; i++) //count number of spaces
		{
			//Note: For some reason, it sometimes count a space at the end of the line and sometimes it doesnt. No idea why
			// if(c == ' ' && !stopCount); //for some reason, not counting ' ', but counting every char
			// {
			// 	NumOfSpaces++;
			// }
			if(isspace(line[i]))
			{
				// printf("S");
				if(isspace(line[i+1])) continue; //skip if there is double space
				else NumOfSpaces++;
			}
			// else
			// 	printf("%c", line[i]);
		}
		close(file);
	}
	// printf("\nSpace : %d\nNewlines : %d", NumOfSpaces, NumOfLines);

	int NumOfManagers = x; //Manger = Chapter
	int NumOfWorkers = NumOfSpaces / NumOfManagers;
	// printf("\nNumber of Workers: %d\nNumber of Managers: %d\n", NumOfWorkers, NumOfManagers);

	//Shared Memory
	key_t key[NumOfSpaces];
	int k = 9999;
	// size_t size = 100;
	int num[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int arr[NumOfSpaces];
	int *addr[NumOfSpaces]; 
	int *shm[NumOfSpaces];
	int smid[NumOfSpaces];

	for(int i = 0; i < NumOfSpaces; i++)
	{
		key[i] = k - i; //create unique keys for shared mem
		if((smid[i] = shmget(key[i], sizeof(int*), IPC_CREAT | 0666)) == -1) //create mem space and id
		{
			perror("shmget");
			exit(1);
		}
		// else
			// printf("SMID: %d\n", smid[i]);

		if((addr[i] = shmat(smid[i], NULL, 0)) == -1)//pointer to space
		{
			perror("shmat");
			exit(1);
		}

		shm[i] = addr[i]; //data to be shared
	}

	pid_t ManagerPID[NumOfManagers];
	pid_t DirectorPID = getppid();
	pid_t WorkersPID[NumOfWorkers];
	pid_t WorkerPID;
	bool newlineFlag = false;

	for(int i = 0; i < NumOfManagers; i++) //Create managers for each chapt
	{
		ManagerPID[i] = fork();
		if(ManagerPID[i] == 0)
		{
			// printf("Manager #%d here\n", i);
			
			for(int j = 0; j < NumOfWorkers; j++)
			{
				// printf("%d\n", j);
				// WorkersPID[j] == fork();
				// if(WorkersPID[j] == 0) // some conflict of iteration. var j keeps resetting to 1
				// {
				// 	printf("Manager #%d: Worker #%d here\n", i, j); 
				// 	exit(0);
				// }
				WorkerPID = fork(); //craete workers
				int target = (NumOfWorkers * i) + j + 1;
				if(WorkerPID == 0)
				{
					// printf("Manager #%d: Worker #%d here accessing %d\n", i, j, (NumOfWorkers * i) + j);
					int x = 0;
					int iter = 1;
					// int target = (NumOfWorkers * i) + j + 1;
					char line[LEN];

					file = open(filename, O_RDONLY);
					read(file, &line, LEN);
					token = strtok(line, search);
					if(iter == target)
					{
						for(int i = 0; i < strlen(token); i++)
						{
							x = (x * 10) + (char)token[i] - '0';
						}
						target += NumOfSpaces;
						// printf("Manager #%d: Worker #%d found %d\n", i, j, x);
						*shm[(NumOfWorkers * i) + j] = *shm[(NumOfWorkers * i) + j] + x;
						// printf("Manager #%d: Worker #%d found %d: total: %d\n", i, j, x, *shm[(NumOfWorkers * i) + j]);
						x = 0;
					}
						
					// printf("%s", token);
					while((token = strtok(NULL, search)) != NULL)
					{
						// printf("Worker %d found %ld\n", NumOfWorkers * i, strtol(token, NULL, 0));
						iter++;
						if(iter == target)
						{
							for(int i = 0; i < strlen(token); i++)
							{
								if((char)token[i] != '\n')
								{
									x = (x * 10) + (char)token[i] - '0';
								}
							}
							target += NumOfSpaces;
							// printf("Manager #%d: Worker #%d found %d\n", i, j, x);
							*shm[(NumOfWorkers * i) + j] = *shm[(NumOfWorkers * i) + j] + x;
							// printf("Manager #%d: Worker #%d found %d: total: %d\n", i, j, x, *shm[(NumOfWorkers * i) + j]);
							x = 0;
						}
					}
					close(file);

					// *shm[(NumOfWorkers * i) + j] /= NumOfLines;
					double avg = (double)*shm[(NumOfWorkers * i) + j] / (double)NumOfLines;
					printf("Avg of Homework%d : %.2f\n", (NumOfWorkers * i) + j + 1, avg);
					exit(0);
				}
				waitpid(WorkerPID, NULL, 0); // a bottleneck, but I cannot figure out how to use an array of workers without it going haywire
				// printf("Manager #%d: Worker #%d has died\n", i, j);
			}
			// for(int j = 0; j < NumOfWorkers; j++)
			// {
			// 	waitpid(WorkersPID[j], NULL, 0);
			// 	printf("Worker #%d has died\n", WorkersPID[j]);
			// }

			exit(0);
		}
	}

	if(DirectorPID == getppid())
	{
		for(int i = 0; i < NumOfSpaces; i++)
		{
			waitpid(ManagerPID[i], NULL, 0);
			// printf("Manager id who died: %d\n", ManagerPID[i]);
			// printf("From Manager %d: %d\n", i, *shm[i]);
		}
		
		for(int i = 0; i < NumOfSpaces; i++)
		{
			shmdt(&smid[i]);
			shmctl(smid[i], IPC_RMID, NULL);
		}
	}

	return 0;
}


/*
19 17 20 18 
9  6  10 9 
12 11 10 16 
3  7  9  10 
0  5  8  6 
15 13 15 15 
20 18 18 16 
17 19 19 18 
13 15 14 12 
10 13 18 15 
118 124 141 135		total

*/