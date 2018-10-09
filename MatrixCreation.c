#include<stdio.h>

#define R 3

#define C 3



void creatematrix(int m[][C],int x,int z)
 
{ 	int i,j;
	
for(i=0;i<x;i++)
	    
for(j=0;j<z;j++)
	      
scanf("%d",(*(m+i)+j));
    

   
  }



void printmatrix(int (*m)[C],int x,int z)
	
{  int i,j;
	
for(i=0;i<x;i++)
	  
for(j=0;j<z;j++)
	     
printf("%d",m[i][j]);
	
}


int main()
 
 {
	int m[R][C];

	creatematrix(m,R,C);

	printmatrix(m,R,C);

	return 0;
  }
