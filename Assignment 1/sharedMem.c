
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

int main(){
	int segment_id; //Identifier for shared memory segment
	shared_data *shared_memory; //pointer to shared memory segment
	segment_id = shmget(IPC_PRIVATE, sizeof(shared_data),S_IRUSR | S_IWUSR); //Allocate a shared memory segment
	shared_memory = (shared_data *)shmat(segment_id,NULL,0); //Attach the shared memory segment

	shared_memory->cz[0]=5; //Write a message to the shared memory segment
	printf("*%ld\n",shared_memory->cz[0]); // Now print out the string from shared ememory

	shmdt(shared_memory); //Now detach shared memory segment
	shmctl(segment_id,IPC_RMID,NULL); //now remove shared memory segment

	return 0;
}