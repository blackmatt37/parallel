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
int main(int argc, char const *argv[])
{
	int forest[HEIGHT][WIDTH];
	bzero(forest, sizeof(int)*HEIGHT*WIDTH);
	srandom(1234);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			int p = random();
			if(p > RAND_MAX/2)
				forest[i][j] = 1;
		}
	}
	display(forest);
	for (int i = 0; i < HEIGHT; ++i)
	{
		forest[i][0] = 2;
	}
	display(forest);
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