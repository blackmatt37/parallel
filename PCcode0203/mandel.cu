#include <stdio.h>
#include <GL/glut.h>
#include <cuda.h>
#include <cuda_runtime.h>


#define WIDTH 800
#define HEIGHT 600


int tmax = 100;
int *gpu_t;
int cpu_t[WIDTH*HEIGHT];
double center_x;
double center_y;
double center_dy;
double center_dx;

// int mandel(float, float, int);
void translate(int, int, double*, double*);
__device__ double translatex(int x, float center_x, float center_dx)
{
	return x*2.0*center_dx/WIDTH + (center_x - center_dx);

}
__device__ double translatey(int y, float center_y, float center_dy)
{
	return y*2.0*center_dy/HEIGHT + (center_y - center_dy);

}

__global__ void mandel(int* gpu_t, int tmax, float center_x, float center_y, float center_dx, float center_dy)
{
	double x = translatex(blockIdx.x, center_x, center_dx);
	double y = translatey(blockIdx.y, center_y, center_dy);
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
	*(gpu_t + blockIdx.x + WIDTH*blockIdx.y) = i+1;
}

void idlefunc(void)
{
	// DO SOME STUFF
}

void displayfunc(void)
{
	if(tmax == 0)
		tmax = 1;
	glClear(GL_COLOR_BUFFER_BIT);
	int x;
	// int y;
	cudaMalloc((void**)&gpu_t, HEIGHT*WIDTH*sizeof(int));
	// printf("Declared array\n");
	dim3 grid(WIDTH, HEIGHT);
	mandel<<<grid, 1>>>(gpu_t, tmax, center_x, center_y, center_dx, center_dy);
	// printf("Called the kernel\n");
	cudaMemcpy(cpu_t, gpu_t, HEIGHT*WIDTH*sizeof(int), cudaMemcpyDeviceToHost);
	// printf("Copied the data back\n");
	for(x=0;x<WIDTH*HEIGHT;x++)
	{
		glColor3f( 0 , cpu_t[x]*1.0/tmax , 0 ) ; // brown
     	//
     	glBegin(GL_POINTS);
     	glVertex2f(x%WIDTH, x/WIDTH);
     	glEnd();
     	// if(x%WIDTH == 0 ) glutSwapBuffers();	
	}
	
	cudaFree(gpu_t);
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
	center_x = 0.4099716787079002;
	center_y = -0.1482886524476352;
	center_dx = 0.0002682137239328;
	center_dy = 0.0002011602929496;
	tmax = 400;
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


// int mandel(float x, float y, int tmax)
// {
// 	float a = 0.0;
// 	float b = 0.0;
// 	int i;
// 	for(i=0; i<tmax; i++)
// 	{
// 		float anew = a*a - b*b + x;
// 		float bnew = 2*a*b + y;
// 		a = anew;
// 		b = bnew;
// 		if(a*a + b*b > 4.0)
// 			break;
// 	}
// 	return (i+1);
// }

void translate(int x, int y, double* a, double* b)
{
	*a = x*2.0*center_dx/WIDTH + (center_x - center_dx);
	*b = y*2.0*center_dy/HEIGHT + (center_y - center_dy);
}
