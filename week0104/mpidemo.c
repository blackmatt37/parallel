// 
// Torbert, 7 October 2013
// 
// mpicc mpidemo.c
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
	int nextworker ;
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
		worker     = rand() % (size - 1) + 1 ;
		nextworker = rand() % (size - 1) + 1 ;
		//
		while( worker == nextworker )		
		{
			nextworker = rand() % (size - 1) + 1 ;
		}
		//
		printf( "send manager0 %2d %2d\n" , worker , nextworker ) ;
		//
		MPI_Send( &nextworker , 1 , MPI_INT , worker , tag , MPI_COMM_WORLD ) ;
		//
		MPI_Recv( &nextworker , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
		//
		worker = status.MPI_SOURCE ;
		//
		printf( "recv manager0 %2d %2d\n" , worker , nextworker ) ;
		//
		nextworker = -1 ; // kill signal
		//
		for( worker = 1 ; worker < size ; worker++)
		{
			MPI_Send( &nextworker , 1 , MPI_INT , worker , tag , MPI_COMM_WORLD ) ;
		}
	}
	else
	{
		while( 1 )
		{
			MPI_Recv( &nextworker , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			//
			if( nextworker < 0 ) break; // kill signal
			//
			worker = status.MPI_SOURCE ;
			//
			printf( "recv worker%02d %2d %2d\n" , rank , worker , nextworker ) ;
			//
			worker = nextworker ;
			//
			nextworker = rand() % size ;
			//
			while( worker == nextworker )		
			{
				nextworker = rand() % size ;
			}
			//
			printf( "send worker%02d %2d %2d\n" , rank , worker , nextworker ) ;
			//
			MPI_Send( &nextworker , 1 , MPI_INT , worker , tag , MPI_COMM_WORLD ) ;
		}
	}
	//
	// MPI
	//
	MPI_Finalize();
	//
	return 0;
}
// 
// end of file
// 