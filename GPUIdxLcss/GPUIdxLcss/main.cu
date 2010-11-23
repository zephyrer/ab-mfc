/********************************************************************
*  main.cu
*  This is the start point of the project!
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cutil_inline.h>
#include <iostream>
#include <vector>
#include <indexing/lcss.h>
#include <kernel/kernel_func.cu>
#include <kernel/kernel_idxlcss.cu>
/************************************************************************/
/* Init CUDA                                                            */
/************************************************************************/
#if __DEVICE_EMULATION__

bool InitCUDA(void){return true;}

#else
bool InitCUDA(void)
{
	int count = 0;
	int i = 0;

	cudaGetDeviceCount(&count);
	if(count == 0) {
		fprintf(stderr, "There is no device.\n");
		return false;
	}

	for(i = 0; i < count; i++) {
		cudaDeviceProp prop;
		if(cudaGetDeviceProperties(&prop, i) == cudaSuccess) {
			if(prop.major >= 1) {
				break;
			}
		}
	}
	if(i == count) {
		fprintf(stderr, "There is no device supporting CUDA.\n");
		return false;
	}
	cudaSetDevice(i);

	printf("CUDA initialized.\n");
	return true;
}

#endif

int main(int argc, char* argv[])
{
	float h_A[32*128],h_B[128];
	float *d_A,*d_B;
	unsigned int size = 32*128*sizeof(float);


	if(!InitCUDA()) {
		return 0;
	}

	char	*device_result	= 0;
	char	host_result[12]	={0};

	cutilSafeCall( cudaMalloc((void**) &device_result, sizeof(char) * 11));

	unsigned int timer = 0;
	cutilCheckError( cutCreateTimer( &timer));
	cutilCheckError( cutStartTimer( timer));
//////////////////////////////////º¯Êýµ÷ÓÃ////////////////////////////////////////
	cudaMalloc((void**)&d_A,size);
	cudaMalloc((void**)&d_B,32*sizeof(float));
	for(int i=0;i<32*128;i++)
	{
		h_A[i] = 1;
	}
	cudaMemcpy(d_A,h_A,size,cudaMemcpyHostToDevice);
	SumMatrix<<<32,128>>>(d_A,d_B);
	cudaMemcpy(h_B,d_B,32*sizeof(float),cudaMemcpyDeviceToHost);
	for(int i=0;i<32;i++)
	{
		std::cout<<h_B[i]<<" ";
	}
	std::cout<<std::endl;
	
	std::vector<int> v1,v2;
//	int n,m,t;
// 	while(std::cin>>n>>m&&n&&m)
// 	{
// 		v1.clear();
// 		v2.clear();
// 		for(int i=0;i<n;i++)
// 		{
// 			std::cin>>t;
// 			v1.push_back(t);
// 		}
// 		for(int i=0;i<m;i++)
// 		{
// 			std::cin>>t;
// 			v2.push_back(t);
// 		}
// 		std::cout<<LCSS(v1,v2)<<std::endl;
// 	}
	int text[8] = {1,2,3,4,1,3,1,4},num;
	int res[8];
	int * d_T,*d_res;
	size = 8*sizeof(int);
	cudaMalloc((void**)&d_res,size);
	cudaMalloc((void**)&d_T,size);
	cudaMemcpy(d_T,text,size,cudaMemcpyHostToDevice);
	Inverted_List_Create<<<1,8>>>(d_T,1,d_res,num);
	cudaMemcpy(res,d_res,size,cudaMemcpyDeviceToHost);
	for(int i=0;i<num;i++)
	{
		std::cout<<res[i]<<" ";
	}
	std::cout<<std::endl;
/////////////////////////////////////////////////////////////////////////////////
	cutilCheckMsg("Kernel execution failed\n");
	cudaThreadSynchronize();
	cutilCheckError( cutStopTimer( timer));
	printf("Processing time: %f (ms)\n", cutGetTimerValue( timer));
	cutilCheckError( cutDeleteTimer( timer));

	cutilSafeCall( cudaMemcpy(host_result, device_result, sizeof(char) * 11, cudaMemcpyDeviceToHost));
	printf("%s\n", host_result);

	cutilSafeCall( cudaFree(device_result));

	return 0;
}
