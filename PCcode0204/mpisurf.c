#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpi.h"

double sigmoid(double i)
{
	return 1.0/(1.0+exp(-i));
}
int         rank , size ;
MPI_Status  status ;
int         tag = 0 ;
int worker;
//
// int worker ;
double prob ;
double steps;
int start;



int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( rank == 0 ) // manager
	{
		double x;
		x = -40;
		int q;
		for(q=1;q<size;q++)
		{
			MPI_Send(&x, 1 , MPI_DOUBLE , q , tag , MPI_COMM_WORLD );
			x+=(50+40)/41;
		}
		while(x <= 50)
		{
			double temp;
			double error[42];
			MPI_Recv( &temp , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			worker = status.MPI_SOURCE ;
			MPI_Recv( &error , 42 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD , &status ) ;

			double y;
			int p = 0;
			for(y=-1490;y<=1000;y+=(1000+1490)/41)
			{
				printf("%f %f %f\n", temp,y, error[p]);
				p++;
			}
			printf("\n");
			MPI_Send(&x, 1 , MPI_DOUBLE , worker , tag , MPI_COMM_WORLD );
			x+=(50+40)/41;
		}
	}
	else
	{
		while(1)
		{
			double worker_x;
			MPI_Recv( &worker_x, 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status );
			int p = 0;
			double error[42];
			double y;
			for(y=-1490;y<=1000;y+=(1000+1490)/41)
			{
				int trial;
				error[p] = 0.0;
				for(trial = 0; trial<10000;trial++)
				{
					double prob = ((double)trial)/10000.0;
					// printf("%f\n", prob);
					int major = 0;
					double total = 0.0;
					int i;
					for(i = 0;i<149;i++)
					{
						if( (double)random()/RAND_MAX > prob)
						{
							total += worker_x;
							major += 1;
						}
						else
						{
							major -= 1;
						}
					}
					total += y;
					if(major > 1)
						major = 1;
					else
						major = 0;
					error[p] += (major-sigmoid(total))*(major-sigmoid(total));
				}
				error[p] /= 10000.0;
				p++;
				// printf("%f %f %f\n", x,y, error);
			}
			MPI_Send(&worker_x , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
			MPI_Send(error, 42, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

		}
	}

	//
	// MPI
	//
	MPI_Finalize();
	return 0;
}