/*
Student ID: 814000842
Full Name: Sanjay Dookhoo
Email: sanjaydookhoo@msn.com
Course Code: COMP3100
*/

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define n 10 //number of elements in array

int unsorted[n]={7,12,19,3,18,4,2,6,15,8};
int sorted[n];

//data passed to thread to know which part of the array to manipulate, the var int mid will only be used by the mergeThread
typedef struct indices{
	int start;
	int end;
	int mid; 
}Indices;

//selection sord that utilizes the start and end components of the struct passed
void * sortingThread(void *param){
	int x,y,smallest,temp;
	Indices ind=*(Indices*)param;

	for(x=ind.start;x<=ind.end-1;x++){
		smallest=x;
		for(y=x+1;y<=ind.end;y++){
			if(unsorted[y]<unsorted[smallest]){
				smallest=y;
			}
		}
		if(smallest!=x){
			temp=unsorted[x];
			unsorted[x]=unsorted[smallest];
			unsorted[smallest]=temp;
		}
	}
}

//merging that utilizes the start, mid and end components of the struct passed
void * mergeThread(void *param){
	Indices ind=*(Indices*)param;
	int i=ind.start,j=ind.mid,k=0,smallest,temp;

	while(i<ind.mid && j<=ind.end){
		if(unsorted[i]<unsorted[j]){
			sorted[k++]=unsorted[i++];
		}else{
			sorted[k++]=unsorted[j++];
		}
	}

	if(i<ind.mid){
		for(i;i<ind.mid;i++){
			sorted[k++]=unsorted[i];	
		}
	}else{
		for(j;j<=ind.end;j++){
			sorted[k++]=unsorted[j];	
		}
	}
}

int main(){
	int i,mid;

	printf("The given array is\n");
	for(i=0;i<n;i++){
		printf("%d ",unsorted[i]);
	}
	printf("\n");

	mid=n/2; //splitting the array

	printf("Before selection sort of the first half\n");
	for(i=0;i<mid;i++){
		printf("%d ",unsorted[i]);
	}
	printf("\n");

	printf("Before selection sort of the second half\n");
	for(i=mid;i<n;i++){
		printf("%d ",unsorted[i]);
	}
	printf("\n");

	//set indices
	Indices ind1;
	Indices ind2;
	ind1.start=0;
	ind1.end=mid-1;
	ind2.start=mid;
	ind2.end=n-1;

	//create sorting threads
	pthread_t sort1;
	pthread_t sort2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&sort1,&attr,sortingThread,&ind1);
	pthread_create(&sort2,&attr,sortingThread,&ind2);
	//wait for both sorting threads to finish before continuing
	pthread_join(sort1,NULL);
	pthread_join(sort2,NULL);

	printf("After selection sort of the first half\n");
	for(i=0;i<mid;i++){
		printf("%d ",unsorted[i]);
	}
	printf("\n");

	printf("After selection sort of the second half\n");
	for(i=mid;i<n;i++){
		printf("%d ",unsorted[i]);
	}
	printf("\n");

	//create mergeThread
	Indices ind3;
	ind3.start=0;
	ind3.mid=mid;
	ind3.end=n-1;

	pthread_t merge;
	pthread_create(&merge,&attr,mergeThread,&ind3);
	pthread_join(merge,NULL);

	printf("After being merged, the sorted array is\n");
	for(i=0;i<n;i++){
		printf("%d ",sorted[i]);
	}
	printf("\n");

	printf("\n\n");
	return 0;
}