// 
// Torbert, 24 January 2014
// 
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "mpi.h"
//
#define WID 640
#define HEI 480
#define DIM 10
//
int w=WID,h=HEI;
int n=WID/DIM,m=HEI/DIM;
int dn=DIM,dm=DIM;
//
int c[WID/DIM][HEI/DIM];
//
int flag=0;
//
void display(void)
{
   int x,y;
   int j,k;
   //
   glClear(GL_COLOR_BUFFER_BIT);
   //
   glColor3f(1.0,1.0,1.0);
   for(x=0;x<n;x++)
      for(y=0;y<m;y++)
         if(c[x][y])
         {
            glBegin(GL_POINTS);
            for(j=0;j<=dn;j++)
               for(k=0;k<=dm;k++)
                  glVertex2f(x*dn+j,y*dm+k);
            glEnd();
         }
   //
   glutSwapBuffers();
}
//
void idle(void)
{
   int x,y,j,k;
   int count,xn,yn;
   int t[WID/DIM][HEI/DIM];
   //
   if(flag)
   {
      for(x=0;x<n;x++)
         for(y=0;y<m;y++)
            t[x][y]=c[x][y];
      for(x=0;x<n;x++)
         for(y=0;y<m;y++)
         {
            count=0;
            for(j=-1;j<=1;j++)
               for(k=-1;k<=1;k++)
               {
                  if(j==0 && k==0)
                     continue;
                  xn=x+j;
                  yn=y+k;
                  if(xn<0) xn=n-1;
                  if(xn>=n) xn=0;
                  if(yn<0) yn=m-1;
                  if(yn>=m) yn=0;
                  if(t[xn][yn])
                  {
                     count++;
                  }
               }
            if(t[x][y]&&(count==2 || count==3) || !t[x][y]&&count==3)
               c[x][y]=1;
            else
               c[x][y]=0;
         }
      glutPostRedisplay();
   }
}
//
void mouse(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
      if(state==GLUT_DOWN)
      {
         flag=(flag+1)%2;
      }
}
//
void keyfunc(unsigned char key,int xscr,int yscr)
{
   if(key=='q')
   {
      exit(0);
   }
}
//
void reshape(int wscr,int hscr)
{
   w=wscr; h=hscr;
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //
   gluOrtho2D(0,w-1,0,h-1);
   glMatrixMode(GL_MODELVIEW);
}
//
int main(int argc,char* argv[])
{  
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

   if(rank == 0)
   {
      //
      int x,y;
      //
      glutInit(&argc,argv);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
      glutInitWindowSize(w,h);
      glutInitWindowPosition(100,50);
      glutCreateWindow("Conway's Game of Life");
      //
      glClearColor(0.0,0.0,0.0,0.0);
      glShadeModel(GL_SMOOTH);
      //
      for(x=0;x<n;x++)
         for(y=0;y<m;y++)
            c[x][y]=0;
      //
      c[6][3]=1;   
      c[7][4]=1;   
      c[5][5]=1;   
      c[6][5]=1;   
      c[7][5]=1;   
      //
      glutDisplayFunc(display);
      glutIdleFunc(idle);
      glutMouseFunc(mouse);
      glutKeyboardFunc(keyfunc);
      glutReshapeFunc(reshape);
      //
      glutMainLoop();
      //
   }
   else
   {
      int grid[m][n/size];
      while(1)
      {
         int right = rank+1;
         int left = rank-1;
         if(right > size) right = 1;
         if(left < 1) left = size;

         if(rank % 2 == 1)
         {
            //look to right and send
            MPI_Send( &grid[n] , m , MPI_DOUBLE , right , tag , MPI_COMM_WORLD ) ;
            //look to right and receive
            //look to left and send
            MPI_Send( &grid[1] , m , MPI_DOUBLE , left , tag , MPI_COMM_WORLD ) ;
            //look to left and receive
         }
         if(rank % 2 == 0)
         {
            //look to left and receive
            //look to left and send
            MPI_Send( &grid[1] , m , MPI_DOUBLE , left , tag , MPI_COMM_WORLD ) ;
            //look to right and receive
            //look to right and send
            MPI_Send( &grid[n] , m , MPI_DOUBLE , right , tag , MPI_COMM_WORLD ) ;
         }
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
