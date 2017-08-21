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

int Golomb(int n){
	if(n==1)
		return 1;
	return 1+Golomb(n-Golomb(Golomb(n-1)));
}

void PrintGolomb(int n){
	int i;
	for(i=1;i<=n;i++){
		printf("%d ", Golomb(i));
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
		PrintGolomb(n);
		printf("\n");
	}else{
		wait(NULL);
		//printf("Child Complete\n");
	}
	return 0;
		
}