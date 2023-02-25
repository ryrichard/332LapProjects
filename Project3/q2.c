/*
Consider a series, S1 = 2 +4 +6+,..+10 (sum of even positive numbers upto 10) and another series
S2=1+3+5+..+9 (sum of odd positive numbers upto 10) and
Another series S3 = 1+2+3+…+10. (sum of all positive number upto 10)
We know that S1+S2 = S3.
Now write a program, where a parent process creates 2 child process and computes S1 and S2. And Parent
process computes S3. The input argument for program will be the end of series number for S3.
For example, if the execution file name is series.exe then, the argument will be
./series.exe 10
Child 1 will compute the series from 1 to upto 10 with difference 2. So, it would be 1+3+5+7+9
Child 2 will compute the series from 2 to upto 10 with difference 2. So, it would be 2+4+6+8+10
Parent will compute the series from 1 to 10 with difference 1. So, it would be,
1+2+3+4+5+6+7+8+9+10
Make sure your program can compute S1, S2 and S3 for any given number.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// printf("SelfId: %d, ParentId: %d\n", getpid(), getppid());
	int input = atoi(argv[1]);

	if(fork())
	{
		if(fork())
		{	
			wait(1);
			int sum1 = 0;
			for(input; input > 0; input--)
				sum1 += input;
			printf("-- Self --\n%d\n", sum1);
		}
		else //even
		{
			int sumE = 0;
			if(input%2 != 0) 
				input -= 1;
			for(input; input > 0; input-=2)
				sumE += input;
			printf("-- Child02 --\n%d\n", sumE);
		}
	}
	else //odd
	{
		int sumO = 0;
		if(input%2 != 1)
			input -= 1;
		for(input; input > 0; input-=2)
			sumO += input;
		printf("-- Child01 --\n%d\n", sumO);
	}

	return 0;
}