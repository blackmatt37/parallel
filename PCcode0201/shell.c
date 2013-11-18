// 
// Torbert, 30 October 2013
// 
// gcc -lGLU -lglut glshell.c
// 
#include <stdio.h>
#include <GL/glut.h>
// 
#define N 600 
// 
int count =  0 ; 
int ascii = 48 ; 
// 
void idlefunc(void)
{
   ++count ;
   //
   if( count == 1000000 )
   {
      count = 0 ;
      //
      ++ascii ;
      //
      if( ascii ==  58 ) ascii = 65 ;
      if( ascii ==  91 ) ascii = 97 ;
      if( ascii == 123 ) ascii = 48 ;
      //
      glutPostRedisplay() ;
   }
}
//
void displayfunc(void)
{
   int x , y ;
   //
   glClear(GL_COLOR_BUFFER_BIT);
   //
   for( x = 100 ; x < N ; x++ )
   {
      for( y = 100 ; y < N ; y++ )
      { 
         glColor3f( 0.6 , 0.3 , 0.0 ) ; // brown
         //
         glBegin(GL_POINTS);
         glVertex2f(x,y);
         glEnd();
      }
   }
   //
   glColor3f( 0.0 , 0.0 , 0.0 ) ;
   glBegin( GL_TRIANGLES ) ;
   glVertex2f( 0.25 * N , 0.25 * N ) ;
   glVertex2f( 0.75 * N , 0.25 * N ) ;
   glVertex2f( 0.50 * N , 0.50 * N ) ;
   glEnd() ;
   //
   glColor3f( 1.0 , 1.0 , 1.0 ) ;
   glRasterPos2f( 0.85*N , 0.9*N ) ;
   glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , (char)ascii ) ;
   //
   glutSwapBuffers() ;
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)N,(GLsizei)N);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*N,0.0,1.0*N);
   glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
   {
      printf("Left mouse clicked.\n");
   }
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
   {
      printf("Right mouse clicked.\n");
   }
}
void motionfunc(int xscr,int yscr)
{
   printf("Motion ( %d , %d ).\n" , xscr , yscr ) ;
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   printf("Key %c pressed.\n" , key);
}
void specialfunc(int key,int xscr,int yscr)
{
   if( key == GLUT_KEY_UP )
   {
      printf("Up arrow pressed.\n");
   }
}
void closefunc(void)
{
   printf("Window closed.\n");
}
int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(N,N);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   glutIdleFunc(idlefunc);
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyfunc);
   glutSpecialFunc(specialfunc);
   glutWMCloseFunc(closefunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
