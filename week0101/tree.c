#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#ifndef HEIGHT
#define HEIGHT 8
#endif
#ifndef WIDTH
#define WIDTH 50
#endif
void display(int[][WIDTH]);
void burn(int[][WIDTH]);
int inBound(int, int);
int main(int argc, char const *argv[])
{
	int forest[HEIGHT][WIDTH];
	bzero(forest, sizeof(int)*HEIGHT*WIDTH);
	srandom(1234);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			float p = random()/RAND_MAX;
			if(p > 0.5)
				forest[i][j] = 1;
		}
	}
	display(forest);
	printf("\n");
	for (int i = 0; i < HEIGHT; ++i)
	{
		if(forest[i][0] == 1)
			forest[i][0] = 2;
	}
	display(forest);
	printf("\n");
	for (int i = 0; i < WIDTH; ++i)
	{
		burn(forest);
		display(forest);
		printf("\n");

	}
	
	

}

void display(int f[][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (f[i][j] == 1)
				printf("T");
			else if(f[i][j] == 2)
				printf("*");
			else if(f[i][j] == 3)
				printf("?");
			else
				printf(" ");
			// printf("%d\n", forest[i][j]);
		}
		printf("\n");
	}
}
void burn(int f[][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == 2)
			{
				f[i][j] = 0;
				if(inBound(i+1, j) && f[i+1][j] == 1)
					f[i+1][j] = 3;
				if(inBound(i-1, j) && f[i-1][j] == 1)
					f[i-1][j] = 3;
				if(inBound(i, j+1) && f[i][j+1] == 1)
					f[i][j+1] = 3;
				if(inBound(i, j-1) && f[i][j-1] == 1)
					f[i][j-1] = 3;
			}
		}
	}
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == 3)
				f[i][j] = 2;
		}
	}
}
int inBound(int h, int w)
{
	return (h < HEIGHT && h >= 0 && w >=0 && w < WIDTH);
}