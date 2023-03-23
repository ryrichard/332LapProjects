/*
Write a program where a 3 process are attached to a shared memory.
Process A, writes 5 characters {A,A,A,A,A} to the shared memory. Then waits till the 1 st
character is changed to B by Process B , print out the new contents of the shared memory and
then terminates.
Process B changes the first character to B => {B,A,A,A,A} and Then waits till the 1st Character is
changed to C by Process C, print out the new contents and then terminates.
Process C changes the first character to C => {C,A,A,A,A} and Then detaches the shared memory
and terminates.
There is no parent-child relationship between the processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h>

int main()
{
	key_t key = 9999;
	size_t size = 100;
	char *addr, *shm;
	int smid;

	if((smid = shmget(key, size, 0666)) == -1) //dont create, just find shared memory
	{
		perror("shmget");
		exit(1);
	}	
	else
		printf("SMID: %d\n", smid);

	if((addr = shmat(smid, NULL, 0)) == (char *)-1)
	{
		perror("shmat");
		exit(1);
	}	

	shm = addr; //make shm point to first char
	*shm = 'B';

	while(shm[0] != 'C')
		sleep(1);

	printf("Addr: %s\n", addr);

	exit(0);

	return 0;
}