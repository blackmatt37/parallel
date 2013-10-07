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

void display(char[][WIDTH]);
int burnStep(char[][WIDTH]);
int inBound(int, int);
int burn(double);


int main(int argc, char const *argv[])
{
	srandom(1234);
	for (double prob = 0.1; prob < 1.0; prob+=0.1)
	{
		int total = 0;
		for (int trial = 0; trial < 10000; ++trial)
		{
			total += burn(prob);
		}
		printf("%f %f\n", prob, total/(10000.0*WIDTH));
	}
	return 0;
	
}







void display(char f[][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (f[i][j] == 'T')
				printf("T");
			else if(f[i][j] == '*')
				printf("*");
			else if(f[i][j] == '?')
				printf("?");
			else
				printf(" ");
			// printf("%d\n", forest[i][j]);
		}
		printf("\n");
	}
}
int burnStep(char f[][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == '*') // if the tree is on fire
			{
				f[i][j] = 0; //remove the currently burning tree
				if(inBound(i+1, j) && f[i+1][j] == 'T')
					f[i+1][j] = '?';
				if(inBound(i-1, j) && f[i-1][j] == 'T')
					f[i-1][j] = '?';
				if(inBound(i, j+1) && f[i][j+1] == 'T')
					f[i][j+1] = '?';
				if(inBound(i, j-1) && f[i][j-1] == 'T')
					f[i][j-1] = '?';
			}
		}
	}
	int q = 0;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if(f[i][j] == '?') //change the intermediate to a fire
			{	
				f[i][j] = '*';
				q++;
			}
		}
	}
	return q;
}
int inBound(int h, int w)
{
	return (h < HEIGHT && h >= 0 && w >=0 && w < WIDTH);
}


int burn(double prob)
{
	char forest[HEIGHT][WIDTH];
	bzero(forest, sizeof(char)*HEIGHT*WIDTH);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			float p = 1.0*random()/RAND_MAX;
			if(p < prob)
				forest[i][j] = 'T';
		}
	}
	for (int i = 0; i < HEIGHT; ++i)
	{
		if(forest[i][0] == 'T')
			forest[i][0] = '*';
	}
	int stepCount = 0;
	while (1)
	{
		if(!burnStep(forest))
			break;
		stepCount++;
		// display(forest);
		// printf("\n");
	}
	return stepCount;
}