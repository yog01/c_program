/*
 * hamming.c
 *
 *  Created on: 17-Aug-2024
 *      Author: yo
 */
#include <stdio.h>
#include <string.h>

#define N 5
#define K 3

int G[K][N]={
			{1,0,0,1,1},
			{0,1,0,1,0},
			{0,0,1,0,1}
	};

int H[N-K][N]={
		{1,1,0,1,0},
		{1,0,1,0,1}
	};

//codeword=mXG
void encode(int *message,int *codeword){
	int i,j;
	for(i=0;i<N;++i){
		codeword[i]=0;
		for(j=0;j<K;++j){
			codeword[i]^=message[j]&G[j][i];
		}
	}
}

//s=rXHT
void coumpute_syndrome(int *recived,int *syndrome){
	int i=0,j=0;

	for(i=0;i<N-K;++i){
		syndrome[i]=0;
		for(j=0;j<N;++j)
		{
			syndrome[i]^=recived[j]&H[i][j];
		}
	}

}

void correct_error(int *recived,int *syndrome){

	if(syndrome[0]==0 && syndrome[1]== 1)
	{
		recived[2]^=1;//3rd
	}else if(syndrome[0]==1 && syndrome[1]==0)
	{
		recived[1]^=1;//2rd
	}else if(syndrome[0]==1 && syndrome[1]==1){
		recived[0]^=1;//1st
	}else{

	}
}

int main(){

	int message[K]={1,0,1} ,codeword[N], recived[N], syndrome[N-K],i=0;

	encode(message,codeword);
	printf("Encoded Codeword: ");
	for(i=0;i<N;++i)
	{
		printf("%d ",codeword[i]);
	}
	printf("\n");

	memcpy(recived,codeword,N*sizeof(int));
	recived[2]^=1;
	printf("Received Codeword with 3rd bit Error: ");
	for(i=0;i<N;++i)
		printf("%d ",recived[i]);
	printf("\n");

	coumpute_syndrome(recived,syndrome);
	printf("Syndrome: ");
	for(i=0;i<N-K;++i)
	{
		printf("%d ",syndrome[i]);
	}
	printf("\n");

	correct_error(recived,syndrome);
	printf("Corrected code: ");
	for(i=0;i<N;++i)
	{
		printf("%d ",recived[i]);
	}
	printf("\n");

	return 0;
}
