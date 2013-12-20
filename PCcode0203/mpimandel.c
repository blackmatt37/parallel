#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
// 
#include "mpi.h"



int main(int argc, char const *argv[])
{
	//
	// MPI
	//
	int         rank , size ;
	MPI_Status  status ;
	int         tag = 0 ;
	//
	// int worker ;
	double prob ;
	double steps;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( rank == 0 ) // manager
	{

	}
	else
	{


	}

	//
	// MPI
	//
	MPI_Finalize();
	//
	return 0;
}