#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
// 
#include "mpi.h"

#define WIDTH 800
#define HEIGHT 600


int tmax = 100;
// int *gpu_t;
int cpu_t[WIDTH*HEIGHT];
int worker_t[WIDTH];
double center_x;
double center_y;
double center_dy;
double center_dx;
//
// MPI
//
int         rank , size ;
MPI_Status  status ;
int         tag = 0 ;
int worker;
//
// int worker ;
double prob ;
double steps;
int start;

void translate(int x, int y, double* a, double* b)
{
	*a = x*2.0*center_dx/WIDTH + (center_x - center_dx);
	*b = y*2.0*center_dy/HEIGHT + (center_y - center_dy);
}

double translatex(int x, double center_x, double center_dx)
{
	return x*2.0*center_dx/WIDTH + (center_x - center_dx);

}
double translatey(int y, double center_y, double center_dy)
{
	return y*2.0*center_dy/HEIGHT + (center_y - center_dy);

}
int mandel(int tmax, double x, double y, double center_x, double center_y, double center_dx, double center_dy)
{
	x = translatex(x, center_x, center_dx);
	y = translatey(y, center_y, center_dy);
	double a = 0.0;
	double b = 0.0;
	int i;
	for(i=0; i< tmax; i++)
	{
		double anew = a*a - b*b + x;
		double bnew = 2*a*b + y;
		a = anew;
		b = bnew;
		if(a*a + b*b > 4.0)
			break;
	}
	return i+1;
}


void displayfunc(void)
{
	if(tmax == 0)
		tmax = 1;
	glClear(GL_COLOR_BUFFER_BIT);
	int x;
	// int y;
	int i;
	int colcount = 0;
	int jobs = 0;
	// for (i = 0; i < WIDTH*HEIGHT; ++i)
	// {
	// 	cpu_t[i] = mandel(tmax, i%WIDTH, i/WIDTH, center_x, center_y, center_dx, center_dy);
	// }
	for(i=1; i<size; i++)
	{
		MPI_Send( &colcount , 1 , MPI_INT , i , tag , MPI_COMM_WORLD ) ;
		MPI_Send( &tmax , 1 , MPI_INT , i , tag , MPI_COMM_WORLD ) ;
		MPI_Send( &center_x , 1 , MPI_DOUBLE , i , tag , MPI_COMM_WORLD ) ;
		MPI_Send( &center_y , 1 , MPI_DOUBLE , i , tag , MPI_COMM_WORLD ) ;
		MPI_Send( &center_dx , 1 , MPI_DOUBLE , i , tag , MPI_COMM_WORLD ) ;
		MPI_Send( &center_dy , 1 , MPI_DOUBLE , i , tag , MPI_COMM_WORLD ) ;
		jobs++;
		colcount += WIDTH;
	}
	// printf("FINISHED SENDING INITIAL\n");
	while(jobs > 0)
	{
		MPI_Recv( &start , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
		MPI_Recv( &worker_t, WIDTH, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status) ;
		jobs--;
		for(i=start;i<start+WIDTH;i++)
		{
			cpu_t[i] = worker_t[i-start];
		}
		if(colcount < WIDTH*HEIGHT)
		{
			MPI_Send( &colcount , 1 , MPI_INT , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &tmax , 1 , MPI_INT , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &center_x , 1 , MPI_DOUBLE , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &center_y , 1 , MPI_DOUBLE , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &center_dx , 1 , MPI_DOUBLE , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &center_dy , 1 , MPI_DOUBLE , status.MPI_SOURCE  , tag , MPI_COMM_WORLD ) ;
			jobs++;
			colcount += WIDTH;
		}
	}
	for(x=0;x<WIDTH*HEIGHT;x++)
	{
		glColor3f( 0 , cpu_t[x]*1.0/tmax , 0 ) ; // brown
     	//
     	glBegin(GL_POINTS);
     	glVertex2f(x%WIDTH, x/WIDTH);
     	glEnd();
     	// if(x%WIDTH == 0 ) glutSwapBuffers();	
	}
	
	glutSwapBuffers();
	// exit(0);
	// printf("Displayed data\n");

}

void reshapefunc(int wscr,int hscr)
{
	// HEIGHT = hscr;
	// WIDTH = wscr;
	glViewport(0,0,(GLsizei)WIDTH,(GLsizei)HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,1.0*WIDTH,0.0,1.0*HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
   {
      // printf("Left (%d, %d)\n",xscr,HEIGHT-yscr);
      double xnew = 0;
      double ynew = 0;
      translate(xscr, HEIGHT-yscr, &xnew, &ynew);
      center_x = xnew;
      center_y = ynew;
      center_dx*= 0.7;
      center_dy*= 0.7;
      printf("(%0.16f, %0.16f. %0.16f, %0.16f)\n", center_x, center_y, center_dx, center_dy);
      glutPostRedisplay();
   }
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
   {
      
      double xnew = 0;
      double ynew = 0;
      translate(xscr, HEIGHT-yscr, &xnew, &ynew);
      center_x = xnew;
      center_y = ynew;
      center_dx/= 0.7;
      center_dy/= 0.7;
      printf("(%0.16f, %0.16f. %0.16f, %0.16f)\n", center_x, center_y, center_dx, center_dy);
      glutPostRedisplay();
   }
}
void motionfunc(int xscr,int yscr)
{
   // SHOW MOUSE MOTION
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
	if(key == 32)
	{
		tmax *= 2;
		printf("%d\n", tmax);
		glutPostRedisplay();
	}
}
void specialfunc(int key,int xscr,int yscr)
{
	if(key == GLUT_KEY_DOWN)
	{
   		tmax /= 2;
		printf("%d\n", tmax);
		glutPostRedisplay();
	}
	if(key == GLUT_KEY_UP)
	{
   		tmax *= 2;
   		printf("%d\n", tmax);
		glutPostRedisplay();
	}
}
void closefunc(void)
{
   printf("Window closed.\n");
   printf("MEM CLEARED\n");
}



int main(int argc, char *argv[])
{
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( rank == 0 ) // manager
	{
		center_x = 0.0;
		center_y = 0.0;
		center_dx = 2;
		center_dy = 1.5;
		tmax = 400;
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutInitWindowPosition(100,50);
		glutCreateWindow("Mandelbrot Set");
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glShadeModel(GL_SMOOTH);

		// glutIdleFunc(idlefunc);
		glutDisplayFunc(displayfunc);
		glutReshapeFunc(reshapefunc);
		glutMouseFunc(mousefunc);
		glutMotionFunc(motionfunc);
		glutKeyboardFunc(keyfunc);
		glutSpecialFunc(specialfunc);
		
		glutMainLoop();

		// return 0;
	}
	else
	{
		while(1)
		{
			MPI_Recv( &start , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			printf("%d\n", start);
			if( start < 0 ) break;
			MPI_Recv( &tmax , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			// printf("PAST TMAX\n");
			MPI_Recv( &center_x , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			printf("%f\n", center_x);
			MPI_Recv( &center_y , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			MPI_Recv( &center_dx , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			MPI_Recv( &center_dy , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			 // kill signal
			//
			worker = status.MPI_SOURCE ;
			int i;
			for(i = start;i<start+WIDTH;i++)
			{
				worker_t[i] = 0;//mandel(tmax, i%WIDTH, i/WIDTH, center_x, center_y, center_dx, center_dy);
			}
			// printf("%f\n", steps);
			//
			MPI_Send( &start , 1 , MPI_INT , worker , tag , MPI_COMM_WORLD ) ;
			MPI_Send( &worker_t, WIDTH, MPI_INT, worker, tag, MPI_COMM_WORLD);
		}
	}

	//
	// MPI
	//
	MPI_Finalize();
	//
	return 0;
}