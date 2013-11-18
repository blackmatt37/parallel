#include <stdio.h>
#include <GL/glut.h>
#include <cuda.h>
#include <cuda_runtime.h>

int HEIGHT = 300;
int WIDTH = 400;

int tmax = 100;



// int mandel(double, double, int);
void translate(int, int, double*, double*);
__device__ double translatex(int x)
{
	return x*1.0/100.0 - 2.0;

}
__device__ double translatey(int y)
{
	return y*1.0/100.0 - 1.5;

}

__global__ void mandel(int* gpu_t)
{
	int tmax = 100;
	double x = translatex(blockIdx.x);
	double y = translatey(blockIdx.y);
	double a = 0.0;
	double b = 0.0;
	int i;
	for(i=0; i<tmax; i++)
	{
		double anew = a*a - b*b + x;
		double bnew = 2*a*b + y;
		a = anew;
		b = bnew;
		if(a*a + b*b > 4.0)
			break;
	}
	*(gpu_t + 400*blockIdx.x + blockIdx.y) = i+1;
}
void idlefunc(void)
{
	// DO SOME STUFF
}

void displayfunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	int x;
	int y;

	int *gpu_t;
	cudaMalloc((void**)&gpu_t, HEIGHT*WIDTH*sizeof(int));
	printf("Declared array\n");
	int cpu_t[HEIGHT][WIDTH];
	dim3 grid(WIDTH, HEIGHT);
	mandel<<<grid, 1>>>(gpu_t);
	printf("Called the kernel\n");
	cudaMemcpy(cpu_t, gpu_t, HEIGHT*WIDTH*sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(gpu_t);
	printf("Copied the data back\n");
	for(x=0;x<WIDTH;x++)
	{
		for(y=0;y<HEIGHT;y++)
		{
			glColor3f( 0 , cpu_t[x][y]*1.0/tmax , 0 ) ; // brown
         	//
         	glBegin(GL_POINTS);
         	glVertex2f(x,y);
         	glEnd();
		}
		glutSwapBuffers();
	}
	printf("Displayed data\n");

}

void reshapefunc(int wscr,int hscr)
{
	HEIGHT = hscr;
	WIDTH = wscr;
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
      printf("Left (%d, %d)\n",xscr,HEIGHT-yscr);
   }
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
   {
      printf("Right (%d, %d)\n",xscr,HEIGHT-yscr);
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
	}
}
void specialfunc(int key,int xscr,int yscr)
{
   // ARROW KEYS
}
void closefunc(void)
{
   printf("Window closed.\n");
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100,50);
	glutCreateWindow("Mandelbrot Set");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glutIdleFunc(idlefunc);
	glutDisplayFunc(displayfunc);
	glutReshapeFunc(reshapefunc);
	glutMouseFunc(mousefunc);
	glutMotionFunc(motionfunc);
	glutKeyboardFunc(keyfunc);
	glutSpecialFunc(specialfunc);

	glutMainLoop();

	return 0;
}


// int mandel(double x, double y, int tmax)
// {
// 	double a = 0.0;
// 	double b = 0.0;
// 	int i;
// 	for(i=0; i<tmax; i++)
// 	{
// 		double anew = a*a - b*b + x;
// 		double bnew = 2*a*b + y;
// 		a = anew;
// 		b = bnew;
// 		if(a*a + b*b > 4.0)
// 			break;
// 	}
// 	return (i+1);
// }

void translate(int x, int y, double* a, double* b)
{
	*a = x/100.0 - 2.0;
	*b = y/100.0 - 1.5;
}
