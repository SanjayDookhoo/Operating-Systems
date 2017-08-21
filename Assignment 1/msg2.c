/*
Student ID: 814000842
Full Name: Sanjay Thistle kashmir Dookhoo
Email: sanjaydookhoo@msn.com
Course Code: COMP3100
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

//#define MAX_TEXT 512
#define SZ 1000
struct MSG {
	long int msgType; 
	char ls[SZ]; 
}; 

void DisplayLucas(char *str){
	int i;
	for(i=0;i<strlen(str);i++){
		if(i==strlen(str)-1){
			printf("%d\n",str[i]);
		}else{
			printf("%d, ",str[i]);
		}
	}
}

int main(){
	int running = 1,i;
	int msgid;
	char buffer[BUFSIZ];
	long int msg_to_receive = 0;
	struct MSG msg;
	struct MSG msg1; 
	int MAX_SIZE=sizeof(struct MSG);
	/*Initialising the Message Queue*/
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT); /*The msgget() function initializes a new message queue. It can also return the message queue ID (msqid) of the queue corresponding to the key argument. The value passed as the msgflg argument must be an octal integer with settings for the queue's permissions and control flags such as (IPC_CREAT) */

	if (msgid == -1) {
		printf("msgget failed with error\n"); //Error Occurred
		exit(-1);
	}

	while (running) {
		printf("\nEnter nonnegative: "); //enter nonnegative numbers, to stop enter a negative number
		fgets(buffer, BUFSIZ, stdin);//get the number as a string
		msg.msgType=1;
		strcpy(msg.ls,buffer);//copy the numbers to a temporary array call buffer [ ]


		int x=0;
		//x=msg.ls[0]-'0'; //convert number entered to int
		x=atoi(msg.ls); //convert number entered to int
		
		if ( x < 0 ) { //termination of program condition (enter a -ve number)
			printf("0 <= n <=9");
			running = 0;

			if ( msgsnd(msgid, (void *)&msg, MAX_SIZE, 0) == -1 ){ /*msgsnd( ) system call -creates a   message queue and sends one message to the queue.*/
				printf("msgsnd failed\n");
				exit(-1);
			}
		}else{
			if ( msgsnd(msgid, (void *)&msg, MAX_SIZE, 0) == -1 ){ /*msgsnd( ) system call -creates a   message queue and sends one message to the queue.*/
				printf("msgsnd failed\n");
				exit(-1);
			}
			if ( msgrcv(msgid, (void *)&msg1, MAX_SIZE, msg_to_receive, 0) == -1 ){ /*msgrcv( ) system call - reads the message from the queue.*/
				printf("msgrcv failed with error\n");
				exit(-1);
			}
			//DisplayLucas(msg1.ls); /*display the numbers you entered from the receiver (msg1.c)*/
			printf("%s",msg1.ls);
		}
	}
	printf("\n");

	return 0;
}
