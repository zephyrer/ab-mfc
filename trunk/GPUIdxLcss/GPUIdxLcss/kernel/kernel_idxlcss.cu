#ifndef _LCSS_IDXKERNEL_H_
#define _LCSS_IDXKERNEL_H_
#include <cutil.h>
#include <cutil_inline.h>
__global__ static void Inverted_List_Create(int* T,int c,int* res,int N)
{
	int idx = blockIdx.x*blockDim.x+threadIdx.x;
	__shared__ int cnt;
	__syncthreads();
	if(idx == 0)
		cnt = 0;
	__syncthreads();
	if(idx < N)
	{
		if(T[idx] == c)
		{
			res[cnt++] = idx;
		}
	}
}
#endif