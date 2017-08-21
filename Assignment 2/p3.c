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

//used to store the row and col index for the gridThreads
typedef struct el{
	int row;
	int col;
}El;

int isSudoku=1; //a flag, if 1 then sudoku is valid else it is invalid
int sudoku[9][9]=
	{{6,2,4,5,3,9,1,8,7},
	{5,1,9,7,2,8,6,3,4},
	{8,3,7,6,1,4,2,9,5},
	{1,4,3,8,6,5,7,2,9},
	{9,5,8,2,4,7,3,6,1},
	{7,6,2,3,9,1,4,5,8},
	{3,7,1,9,5,6,8,4,2},
	{4,9,6,1,8,2,5,7,3},
	{2,8,5,4,7,3,9,1,6}};

//array initialized to all be -1, when a -1 is found digit 1-9 can be entered
void initArr(int arr[]){
	int i=0;
	for(i=0;i<9;i++){
		arr[i]=-1;
	}
}

//adds to array if the digit from 1-9 is not already in the arr, otherwise the sudoku is invalid. Used for checking in row, grid and col
//returns 0 if digit entered without any problems, returns 1 if sudoku is invalid because digit repeats
int addToArr(int arr[],int x){
	int i;
	for(i=0;i<9;i++){
		if(arr[i]==x){
			return 1;
		}else if(arr[i]==-1){
			arr[i]=x;
			return 0;
		}
	}	
}

//checks column passed as param if its 9 digits are unique
void * colCheck(void *param){
	int col=*(int*)param;
	int i,state;
	int arr[9];

	//init arr
	initArr(arr);
	for(i=0;i<9;i++){
		state=addToArr(arr,sudoku[i][col]);
		if(state==1){
			isSudoku=0;
			break;
		}
	}	
}

//checks row passed as param if its 9 digits are unique
void * rowCheck(void *param){
	int row=*(int*)param;
	int i,state;
	int arr[9];

	//init arr
	initArr(arr);
	for(i=0;i<9;i++){
		state=addToArr(arr,sudoku[row][i]);
		if(state==1){
			isSudoku=0;
			break;
		}
	}	
}

//checks row passed as param if its 9 digits are unique
void * gridCheck(void *param){
	El gridEl=*(El*)param;
	int row,col,temp,state;
	int arr[9];
	
	//init arr
	initArr(arr);
	for(row=gridEl.row;row<gridEl.row+3;row++){
		for(col=gridEl.col;col<gridEl.col+3;col++){
			state=addToArr(arr,sudoku[row][col]);
			if(state==1){
				isSudoku=0;
				break;
			}
		}
	}	
}

//prints sudoku that is to be validated
void printSudoku(){
	int i,j;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			printf("%d ",sudoku[i][j]);
		}
		printf("\n");
	}
}

int main(){
	int i;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_t tid[27]; // this is for the 27 threads that is created for checking each row,col and grid
	int x=0; //the next tid to be inserted
	El gridEl; //first grid element

	printSudoku();

	//rows
	for(i=0;i<9;i++){
		pthread_create(&tid[x++],&attr,rowCheck,&i);
	}

	//cols
	for(i=0;i<9;i++){
		pthread_create(&tid[x++],&attr,colCheck,&i);	
	}

	//grids
	for(i=0;i<9;i++){
		//determines row and col of first el in each 3x3 grid in the sudoku
		gridEl.row=i/3*3;
		gridEl.col=i%3*3;

		pthread_create(&tid[x++],&attr,gridCheck,&gridEl);	
	}

	for(i=0;i<27;i++){
		pthread_join(tid[i],NULL);
	}
	
	if(isSudoku==1){
		printf("The solution to the 9x9 sudoku puzzle is valid");
	}else{
		printf("The solution to the 9x9 sudoku puzzle is invalid");
	}

	printf("\n\n");
	return 0;
}