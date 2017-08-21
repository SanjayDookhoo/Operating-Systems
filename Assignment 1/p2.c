/*
Student ID: 814000842
Full Name: Sanjay Thistle kashmir Dookhoo
Email: sanjaydookhoo@msn.com
Course Code: COMP3100
*/

#include <stdio.h>
#include <stdlib.h>	
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SZ 100
typedef struct{
	long cz[SZ];
}shared_data;

int Golomb(int n){
	if(n==1)
		return 1;
	return 1+Golomb(n-Golomb(Golomb(n-1)));
}

void GolombGen(shared_data *shared_memory,int n){
	int i;
	for(i=0;i<n;i++){
		shared_memory->cz[i]=Golomb(i+1);
	}
}

void DisplayGolomb(shared_data * shared_memory,int n){
	int i;
	for(i=0;i<n;i++){
		printf("%ld ",shared_memory->cz[i]);
	}
}

int isPosNumber(char *num){
	int i=0;
	for(i;i<strlen(num);i++){
		if(num[i]<'0' || num[i]>'9')
			return 0;
	}

	return 1;
}

int main(int argc, char *argv[]){
	int segment_id; //Identifier for shared memory segment
	shared_data *shared_memory; //pointer to shared memory segment
	segment_id = shmget(IPC_PRIVATE, sizeof(shared_data),S_IRUSR | S_IWUSR); //Allocate a shared memory segment
	shared_memory = (shared_data *)shmat(segment_id,NULL,0); //Attach the shared memory segment

	if (argc!=2){
		printf("Argument passed is less or greater than 1");
		return -1;
	}else{
		if(!isPosNumber(argv[1])){
			printf("is not a number or is a negative number");
			return -1;
		}

	}


	int n=atoi(argv[1]);
	pid_t pid;
	pid=fork();
	if(pid<0){
		printf("Fork Failed\n");
		return 1;
	}
	if(pid==0){
		//printf("entered\n");
		GolombGen(shared_memory,n);
	}else{
		wait(NULL);
		//printf("Child Complete\n");
		DisplayGolomb(shared_memory,n);
		printf("\n");
	}

	shmdt(shared_memory); //Now detach shared memory segment
	shmctl(segment_id,IPC_RMID,NULL); //now remove shared memory segment

	return 0;		
}