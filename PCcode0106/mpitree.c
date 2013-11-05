// 
// Matthew Kramer
// 
// mpicc mpitree.c
// mpirun -np 4 a.out
// 
// mpirun -np 4 -machinefile hostlist.txt a.out
// 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include "mpi.h"
// 

#ifndef HEIGHT
#define HEIGHT 100
#endif
#ifndef WIDTH
#define WIDTH 100
#endif
#ifndef TRIALS
#define TRIALS 1000
#endif

int burnStep(char[][WIDTH]);
int inBound(int, int);
int burn(double);

int main(int argc, char* argv[])
{
	//
	// MPI
	//
	int         rank , size ;
	MPI_Status  status ;
	int         tag = 0 ;
	//
	int worker ;
	double prob ;
	double steps;
	// result double[10];
	//
	// MPI
	//
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	//
	srand( time(NULL) + rank ) ;
	//
	if( rank == 0 ) // manager
	{
		for (int i = 1; i < size; ++i)
		{
			prob = (0.0);
			MPI_Send(&prob, 1 , MPI_DOUBLE , i , tag , MPI_COMM_WORLD );
		}
		for (int i = 0; i <= 10; ++i)
		{
			double total = 0.0;
			for(int trial = 0;trial< TRIALS;trial++)
			{
				MPI_Recv( &prob , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
				printf("PROB %f\n", prob);
				worker = status.MPI_SOURCE ;
				MPI_Recv( &steps , 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD , &status ) ;
				printf("STEPS %f\n", steps);
				total += steps;
				prob = ((double)i/10.0);
				MPI_Send( &prob , 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD ) ;
				
			}
		}
		for (int i = 1; i < size; ++i)
		{
			MPI_Recv( &prob , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			// printf("PROB %f\n", prob);
			worker = status.MPI_SOURCE ;
			MPI_Recv( &steps , 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD , &status ) ;
			// printf("STEPS %f\n", steps);
			prob = -1;
			MPI_Send( &prob , 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD ) ;
		}
		printf("DONE\n");
		
	}
	else
	{
		while( 1 )
		{
			MPI_Recv( &prob , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			//
			if( prob < 0.0 ) break; // kill signal
			//
			worker = status.MPI_SOURCE ;
			steps = burn(prob)/(double)WIDTH;
			// printf("%f\n", steps);
			//
			MPI_Send( &prob , 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &steps, 1, MPI_DOUBLE, worker, tag, MPI_COMM_WORLD);
		}
	}
	//
	// MPI
	//
	MPI_Finalize();
	//
	return 0;
}


int burnStep(char f[][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == '*') // if the tree is on fire
			{
				f[i][j] = 0; //remove the currently burning tree
				if(inBound(i+1, j) && f[i+1][j] == 'T')
					f[i+1][j] = '?';
				if(inBound(i-1, j) && f[i-1][j] == 'T')
					f[i-1][j] = '?';
				if(inBound(i, j+1) && f[i][j+1] == 'T')
					f[i][j+1] = '?';
				if(inBound(i, j-1) && f[i][j-1] == 'T')
					f[i][j-1] = '?';
			}
		}
	}
	int q = 0;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == '?') //change the intermediate to a fire
			{	
				f[i][j] = '*';
				q++;
			}
		}
	}
	return q;
}
int inBound(int h, int w)
{
	return (h < HEIGHT && h >= 0 && w >=0 && w < WIDTH);
}


int burn(double prob)
{
	char forest[HEIGHT][WIDTH];
	bzero(forest, sizeof(char)*HEIGHT*WIDTH);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			float p = 1.0*random()/RAND_MAX;
			if(p < prob)
				forest[i][j] = 'T';
		}
	}
	for (int i = 0; i < HEIGHT; ++i)
	{
		if(forest[i][0] == 'T')
			forest[i][0] = '*';
	}
	int stepCount = 0;
	while (1)
	{
		if(!burnStep(forest))
			break;
		stepCount++;
		// display(forest);
		// printf("\n");
	}
	return stepCount+1;
}
// 
// end of file
// 