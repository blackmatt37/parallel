#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>


int WIDTH = 400;
int HEIGHT = 300;
__device__ double translatex(int x)
{
	return x/100.0 - 2.0;

}
__device__ double translatey(int y)
{
	return y/100.0 - 1.5;

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



int main(int argc, char const *argv[])
{
	int *gpu_t;
	cudaMalloc((void**)&gpu_t, HEIGHT*WIDTH*sizeof(int));
	int cpu_t[WIDTH][HEIGHT];
	dim3 grid(HEIGHT, WIDTH);
	mandel<<<grid, 1>>>(gpu_t);
	cudaMemcpy(cpu_t, gpu_t, HEIGHT*WIDTH*sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(gpu_t);
	int x;
	int y;
	for(x = 0; x<WIDTH; x++)
	{
		for(y = 0;y< HEIGHT; y++)
		{
			printf("%d\n", cpu_t[x][y]);
		}
	}

}
