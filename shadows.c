#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HEIGHT 480

// Eye
double ex=0.500000;
double ey=0.500000;
double ez=-1.000000;

// Screen
double pz=0.000000;

// Floor
double fy=0.333333;

// Light
double lx=0.000000;
double ly=1.000000;
double lz=-0.500000;

// Sphere 0

double cx[3];
double cy[3];
double cz[3];
double r[3];
double red[3];
double green[3];
double blue[3];





void display(int rgb[HEIGHT][WIDTH][3])
{
	FILE* fout ;
	fout = fopen( "raytrace.ppm" , "w" ) ;
	fprintf( fout , "P3\n" ) ;
	fprintf( fout , "%d %d\n" , WIDTH , HEIGHT ) ;
	fprintf( fout , "255\n" ) ;
	int x,y;
	for( y = 0 ; y < HEIGHT ; y++ )
	{
		for( x = 0 ; x < WIDTH ; x++)
		{
			fprintf( fout , "%d %d %d\n" ,
			rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
		}
	}
	close( fout ) ;
}

double unit(double a, double b, double c)
{
	return sqrt(a*a + b*b + c*c);
}


int main(int argc, char const *argv[])
{
	cx[0]=0.500000;
	cy[0]=0.500000;
	cz[0]=0.166667;
	r[0]=0.166667;
	red[0]=0;
	green[0]=0;
	blue[0]=255;

	cx[1]=0.833333;
	cy[1]=0.500000;
	cz[1]=0.500000;
	r[1]=0.166667;
	red[1]=0;
	green[1]=255;
	blue[1]=0;

	cx[2]=0.333333;
	cy[2]=0.666667;
	cz[2]=0.666667;
	r[2]=0.333333;
	red[2]=255;
	green[2]=0;
	blue[2]=0;
	int rgb[HEIGHT][WIDTH][3] ;

	int x,y;
	for(y = 0;y < HEIGHT; y++)
	{
		for(x = 0;x < WIDTH; x++)
		{
			double px = (double)x/WIDTH;
			double py = 1.0 - (double)y/HEIGHT;

			double rx = px-ex;
			double ry = py-ey;
			double rz = pz-ez;

			double mag = unit(rx, ry, rz);
			rx/= mag;
			ry/= mag;
			rz/= mag;
			
			double current_t = 10000000;
			int    current_sphere = -1;

			rgb[y][x][0] = 0;
			rgb[y][x][1] = 0;
			rgb[y][x][2] = 0;
			// printf("%f %f %f\n", rx, ry, rz);
			int i;
			for(i = 0;i < 3; i++)
			{
				double a = 1;
				double b = 2.0*(ex*rx + ey*ry + ez*rz) - 2.0*(cx[i]*rx + cy[i]*ry + cz[i]*rz);
				double c = (cx[i]*cx[i] + cy[i]*cy[i] + cz[i]*cz[i]) - 2.0*(ex*cx[i] + ey*cy[i] + ez*cz[i]) + (ex*ex + ey*ey + ez*ez) - r[i]*r[i];
				// Billy moses is awesome
				if(b*b - 4*a*c > 0)
				{
					double t1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
					double t2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
					if(t1 > 0)
					{
						if(t1 < current_t)
						{
							current_t = t1;
							current_sphere = i;
						}
					}
					if(t2 > 0)
					{
						if(t2 < current_t)
						{
							current_t = t2;
							current_sphere = i;
						}
					}
				}
			}
			if(current_sphere != -1)
			{
				rgb[y][x][0] = 0.5*red[current_sphere];
				rgb[y][x][1] = 0.5*green[current_sphere];
				rgb[y][x][2] = 0.5*blue[current_sphere];
				// Reflection back to light
				
				// Point on sphere
				double dx = rx*current_t + ex;
				double dy = ry*current_t + ey;
				double dz = rz*current_t + ez;

				// Ray towards light
				double sx = lx - dx;
				double sy = ly - dy;
				double sz = lz - dz;

				mag = unit(sx, sy, sz);
				sx /= mag;
				sy /= mag;
				sz /= mag;


				current_t = 10000000;
				for(i = 0;i < 3; i++)
				{
					double a = 1;
					double b = 2.0*(dx*sx + dy*sy + dz*sz) - 2.0*(cx[i]*sx + cy[i]*sy + cz[i]*sz);
					double c = (cx[i]*cx[i] + cy[i]*cy[i] + cz[i]*cz[i]) - 2.0*(dx*cx[i] + dy*cy[i] + dz*cz[i]) + (dx*dx + dy*dy + dz*dz) - r[i]*r[i];
					// Billy moses is awesome
					if(b*b - 4*a*c > 0)
					{
						double t1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
						double t2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
						if(t1 > 0 && i != current_sphere)
						{
							if(t1 < current_t) 
							{
								current_t = t1;
							}
						}
						if(t2 > 0 && i != current_sphere)
						{
							if(t2 < current_t)
							{
								current_t = t2;
							}
						}
					}
				}
				if(current_t == 10000000)
				{
					// Normal to the sphere
					double nx = (dx - cx[current_sphere])/r[current_sphere];
					double ny = (dy - cy[current_sphere])/r[current_sphere];
					double nz = (dz - cz[current_sphere])/r[current_sphere];

					double dot = nx*sx + ny*sy + nz*sz;
					if(dot > 0)
					{
						rgb[y][x][0] += 0.5*red[current_sphere]*dot;
						rgb[y][x][1] += 0.5*green[current_sphere]*dot;
						rgb[y][x][2] += 0.5*blue[current_sphere]*dot;
					}
				}


			}
			else if(ry < 0)
			{
				double current_t = (fy - ey)/ry;

				double dx = rx*current_t + ex;
				double dy = ry*current_t + ey;
				double dz = rz*current_t + ez;

				// Ray towards light
				double sx = lx - dx;
				double sy = ly - dy;
				double sz = lz - dz;

				mag = unit(sx, sy, sz);
				sx /= mag;
				sy /= mag;
				sz /= mag;


				rgb[y][x][0] = 255*0.5;
				rgb[y][x][1] = 100*0.5;
				rgb[y][x][2] = 10*0.5;

				current_t = 10000000;
				for(i = 0;i < 3; i++)
				{
					double a = 1;
					double b = 2.0*(dx*sx + dy*sy + dz*sz) - 2.0*(cx[i]*sx + cy[i]*sy + cz[i]*sz);
					double c = (cx[i]*cx[i] + cy[i]*cy[i] + cz[i]*cz[i]) - 2.0*(dx*cx[i] + dy*cy[i] + dz*cz[i]) + (dx*dx + dy*dy + dz*dz) - r[i]*r[i];
					// Billy moses is awesome
					if(b*b - 4*a*c > 0)
					{
						double t1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
						double t2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
						if(t1 > 0 && i != current_sphere)
						{
							if(t1 < current_t) 
							{
								current_t = t1;
							}
						}
						if(t2 > 0 && i != current_sphere)
						{
							if(t2 < current_t)
							{
								current_t = t2;
							}
						}
					}
				}
				// Normal to the sphere
				double nx = 0;
				double ny = 1;
				double nz = 0;

				double dot = nx*sx + ny*sy + nz*sz;


				if(current_t == 10000000)
				{
					rgb[y][x][0] += 0.5*255*dot;
					rgb[y][x][1] += 0.5*100*dot;
					rgb[y][x][2] += 0.5*10*dot;
				}
			}
		}
	}
	display(rgb);






	return 0;
}