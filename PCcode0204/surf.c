#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double sigmoid(double i)
{
	return 1.0/(1.0+exp(-i));
}



int main(int argc, char const *argv[])
{
	// printf("%f\n", (double)random()/RAND_MAX );
	double x,y,z;
	for(x=-40;x<=50;x+=(50+40)/41)
	{
		for(y=-1490;y<=1000;y+=(1000+1490)/41)
		{
			int trial;
			double error = 0.0;
			for(trial = 0; trial<10000;trial++)
			{
				double prob = ((double)trial)/10000.0;
				// printf("%f\n", prob);
				int major = 0;
				double total = 0.0;
				int i;
				for(i = 0;i<149;i++)
				{
					if( (double)random()/RAND_MAX > prob)
					{
						total += x;
						major += 1;
					}
					else
					{
						major -= 1;
					}
				}
				total += y;
				if(major > 1)
					major = 1;
				else
					major = 0;
				error += (major-sigmoid(total))*(major-sigmoid(total));
			}
			error /= 10000.0;
			printf("%f %f %f\n", x,y, error);
		}
		printf("\n");
	}
	return 0;
}