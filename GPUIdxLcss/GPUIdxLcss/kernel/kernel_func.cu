#ifndef _LCSS_KERNELFUN_H_
#define _LCSS_KERNELFUN_H_
#include <cutil.h>
#include <cutil_inline.h>
__global__ static void kernel(float* source,int len,float *result)
{
	int i;
	float sum;
	sum = 0;
	for (i=0;i<len;i++)
		sum+=*(source+i);
	*result = sum;
}

__global__ static void ArrayAdd(float* res,float a,clock_t* time)
{
	clock_t start = clock();
	int idx = blockIdx.x*blockDim.x+threadIdx.x;
	if (idx<32)
	{
		res[idx] = idx + a;
	}
	*time = clock() - start;
}
__global__ static void SumMatrix(float* A,float* B)
{
	int bid = blockIdx.x;
	int tid = threadIdx.x;
	__shared__ int s_data[128];
	s_data[tid]=A[bid*128+tid];
	__syncthreads();
	for (int i=64;i>0;i/=2)
	{
		if(tid<i)
		{
			s_data[tid]=s_data[tid]+s_data[tid+i];
		}
		__syncthreads();
	}
	if (tid==0)
	{
		B[bid]=s_data[0];
	}
}
#endif