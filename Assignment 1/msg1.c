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

int Lucas(int n){
	if(n==0) return 2;
	if(n==1) return 1;
	return Lucas(n-1) + Lucas(n-2);
}

void genLucas(struct MSG *msg1,int n){
	msg1->msgType=1;
	strcpy(msg1->ls,"");
	int i,num,len;
	char str[6];
	for(i=0;i<=n;i++){
		num=Lucas(i);
		//itoa(num,str);
		sprintf(str,"%d",num);
		if(i!=n){
			strcat(msg1->ls,str);
			strcat(msg1->ls,", ");
		}else{
			strcat(msg1->ls,str);
		}
	}
}

int main(){
	struct MSG msg; 
	struct MSG msg1; 
	int running = 1;
	int msgid;
	long int msg_to_receive = 0;
	char buffer[100];
	int num;
	int MAX_SIZE=sizeof(struct MSG);
	int n;  
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  /*The msgget() function initializes a new message queue. It can also return the message queue ID (msqid) of the queue corresponding to the key argument. The value passed as the msgflg argument must be an octal integer with settings for the queue's permissions and control flags such as (IPC_CREAT) */

	if (msgid == -1) {
		printf("msgget failed with error\n");//Error Occurred
		exit(-1);
	}

	while (running) {
		if ( msgrcv(msgid, (void *)&msg, BUFSIZ, msg_to_receive, 0) == -1 ) { /*msgrcv( ) system call - reads the message from the queue.*/
			printf("msgrcv failed with error\n");
			exit(-1);
		}
		n=atoi(msg.ls);
		if ( n < 0) {
			running = 0;
			exit(0);
		}else{
			printf("Received n= %s", msg.ls); //display the numbers from the sender (msg2.c)
			num = atoi(msg.ls);
			msg.ls[0]=num;

			genLucas(&msg1,num);
			if ( msgsnd(msgid, (void *)&msg1, MAX_SIZE, 0) == -1 ) { /*msgsnd( ) system call -creates a message queue and sends one message to the queue.*/
				printf("msgsnd failed\n");
				exit(-1);
			}
		}
	}

	return 0;
}