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

typedef struct matrix{
	int rows;
	int cols;
	int ** mat; //will dynamically assign a 2D dimensional aray, due to different matrix having different sizes.  this is done to avoid setting a fixed size to mat since it is a struct and its size cannot change
}Matrix;

//data passed to thread where row relates to the row in mat A that is beign multiplied and col relates tot eh col in mat B that is being multiplied
typedef struct threadData{
	int row;
	int col;
}ThreadData;

//Global matrix
Matrix A,B,C;

void deallocateMem(int ** mat,int rows){
	int i;
	//deallocate second dimension
	for ( i=0; i<rows; i++ )
		free(mat[i]);

	//deallocate first dimension
	free(mat);
}

//function used for dynamically allocaing memory to a struct
int ** allocateMem(int rows,int cols){
	int ** mat,i;

	//allocate first dimension
	mat = malloc((rows)*sizeof(int*));
	if ( mat == NULL ){
	    /* we have a problem */
	    printf("Error: out of memory.\n");
	    return 0;
	}

	//allocate second dimension
	for ( i=0; i<rows; i++ ){
	    mat[i] = malloc((cols)*sizeof(int));
	    if ( mat[i] == NULL ){
	        /* problem */
	        break;
	    }
	}

	if ( i != rows ){
	    /* it didn't allocate */
	}else{
		//printf("All memory allocated successfully\n\n");
	}

	return mat;
}

//print matrix passed
void printMat(Matrix * matPtr){
	int row,col;

	for(row=0;row<matPtr->rows;row++){
		for (col=0;col<matPtr->cols;col++){
			printf("%d\t",matPtr->mat[row][col]);
		}
		printf("\n");
	}

	printf("\n\n");
}

//function that multiples each row and col of the two matrix A and B
void *runner(void *param){
	int x,sum=0;
	ThreadData n=*(ThreadData*)param;
	for(x=0;x<A.cols;x++) {
		sum+=A.mat[n.row][x] * B.mat[x][n.col];
	}
	C.mat[n.row][n.col]=sum;
	pthread_exit(0);
}

//dynamically allocates memory for mat C
//determines which rows and cols are to be multipled in threads
void multiplyMat(){
	pthread_t workers[A.rows * B.cols];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//initialize matrix C
	C.rows=A.rows;
	C.cols=B.cols;
	C.mat=allocateMem(C.rows,C.cols);

	int i,j,x=0;
	for(i=0;i<A.rows;i++){ //row (each el in the row)
		for(j=0;j<B.cols;j++){ // by col (each el in the col)
			ThreadData * data= (ThreadData*) malloc(sizeof(ThreadData));
			data->row=i;
			data->col=j;

			pthread_create(&workers[x++],&attr,runner,data);
			pthread_join(workers[x-1],NULL);	
		}
	}
}

int main(){
	int row,col,el;

	//get rows and cols of matrix
	printf("FIRST MATRIX\n\n");
	printf("Enter the number of rows: ");
	scanf("%d",&row);
	printf("Enter the number of cols: ");
	scanf("%d",&col);
	
	//set first matrix
	A.rows=row;
	A.cols=col;
	A.mat=allocateMem(row,col);

	//get elements of first matrix
	for(row=0;row<A.rows;row++){
		for (col=0;col<A.cols;col++){
			printf("Element [%d][%d] = ",row,col);
			scanf("%d",&el);
			A.mat[row][col]=el;
		}
	}

	// //set matrix hardcoded (to use: comment out from above "get rows and cols of matrix" to here)
	// A.rows=3;
	// A.cols=2;
	// A.mat=allocateMem(A.rows,A.cols);
	// A.mat[0][0]=1;
	// A.mat[0][1]=4;
	// A.mat[1][0]=2;
	// A.mat[1][1]=5;
	// A.mat[2][0]=3;
	// A.mat[2][1]=6;

	//print mat
	printMat(&A);

	//get rows and cols of matrix
	printf("SECOND MATRIX\n\n");
	printf("Enter the number of rows: ");
	scanf("%d",&row);
	printf("Enter the number of cols: ");
	scanf("%d",&col);
	
	//set first matrix
	B.rows=row;
	B.cols=col;
	B.mat=allocateMem(row,col);

	//get elements of first matrix
	for(row=0;row<B.rows;row++){
		for (col=0;col<B.cols;col++){
			printf("Element [%d][%d] = ",row,col);
			scanf("%d",&el);
			B.mat[row][col]=el;
		}
	}

	// //set matrix hardcoded (to use: comment out from above "get rows and cols of matrix" to here)
	// B.rows=2;
	// B.cols=3;
	// B.mat=allocateMem(B.rows,B.cols);
	// B.mat[0][0]=8;
	// B.mat[0][1]=7;
	// B.mat[0][2]=6;
	// B.mat[1][0]=5;
	// B.mat[1][1]=4;
	// B.mat[1][2]=3;

	//print mat
	printMat(&B);

	multiplyMat();

	printf("Result for matrix multiplication\n");
	printMat(&C);

	deallocateMem(A.mat,A.rows);
	deallocateMem(B.mat,B.rows);
	deallocateMem(C.mat,C.rows);

	printf("\n");
	return 0;
}