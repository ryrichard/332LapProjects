/*
Write code using message queue to simulate a chat program where one program can send and
receive data from another program in real time. For example, if you have progA.exe and
progB.exe then – you can run both program and they will behave like this:
*/


#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct msgbuf{
	long mtype;
	char mtext[50];
};

int main()
{
	key_t key;
	int msgid;
	bool state = 0; //1 = sender, 0 = receiver

	//Create/join queue
	key = ftok("Part3A.c", 9); //create key
	msgid = msgget(key, 0666 | IPC_CREAT); //IPC_CREAT - create facility if it does not exist
	printf("Room id : %d\n", msgid);
	struct msgbuf msgs;
	struct msgbuf msgr;
	msgs.mtype = 1;

	while(1)
	{
		while(state) //send message
		{
			printf("Message Sent: ");
			scanf ("%[^\n]%*c", msgs.mtext); //takes message as whole with spaces

			msgsnd(msgid, &msgs, sizeof(msgs), 0);

			if(strcmp(msgs.mtext, "Bye") == 0) //compares string to look for "Bye" - ends message if found
			{
				printf("Ending Queue\n");
				msgctl(msgid, IPC_RMID, NULL);
				exit(1);
			}

			state = 0;
		}

		while(!state) //receive message
		{
			msgrcv(msgid, &msgr, sizeof(msgr), 2, 0);

			if(msgr.mtype != msgs.mtype)
				printf("Message received: %s\n", msgr.mtext);

			if(strcmp(msgr.mtext, "Bye") == 0) //compares string to look for "Bye" - ends message if found
			{
				printf("Sender Ending Queue\n");
				msgctl(msgid, IPC_RMID, NULL);
				exit(1);
			}

			state = 1;
		}
	}

	return 0;
}