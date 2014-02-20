//
// Torbert, 3 February 2014
//
#include <stdio.h>
//
#define WIDTH 640
#define HIGHT 480
//
int main(void)
{
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   //
   FILE* fout ;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         rgb[y][x][0] = 0   ; // red
         rgb[y][x][1] = 255 ; // green
         rgb[y][x][2] = 0   ; // blue
      }
   }
   //
   //
   //
   fout = fopen( "allgreen.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , WIDTH , HIGHT ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   close( fout ) ;
   //
   return 0 ;
}
//
// end of file
//