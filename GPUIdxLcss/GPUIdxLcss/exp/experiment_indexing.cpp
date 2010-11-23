#include <algorithm>
#include <iostream>
#include <indexing/inverted_based.h>
#include <lib/timer.h>
#include <lib/util.h>

using namespace std;
using namespace Time;


bool byRandom = true;

int recordCount = 100000;
int sampleId = 0;
int delta = 1;
float maxDist = 0.2f;
long long  forceTime = 1;
unsigned int seed = 0;




void TestVectorIndexing()
{
	Timer timer;

    PrintfStdOutAndErr("\nTestVectorIndexing()--------------------------------\n");
     
    timer.Start();
       		
  
    if (byRandom)
    {
        PrintfStdOutAndErr("(ConstructIndexByRandom():\n"); 
        VectorIndexing::ConstructIndexByRandom(seed, recordCount);
    }
    else
    {
        PrintfStdOutAndErr("(ConstructIndexFromFile():\n"); 
        recordCount = VectorIndexing::ConstructIndexFromFile("E:\\Fund\\LCSSIndexingCGB\\FeatureData");
    }
    
    PrintfStdOutAndErr("(recordCount %d)(ConstructIndexByRandom  Time  %ds)\n",
        recordCount, timer.ElapsedSecond());
	timer.Start();
	VectorIndexing::SearchByLB_DLCS(sampleId,delta,maxDist);
    long long  dlcsTime = timer.ElapsedMillisecond();
	PrintfStdOutAndErr("------(LB_DLCS Time  %d ms)\n",dlcsTime);
    PrintfStdOutAndErr("------(Ratio %.2f)\n",forceTime * 1.0f / dlcsTime);

}


void TestListIndexing()
{
    Timer timer;

    PrintfStdOutAndErr("\nTestListIndexing()-----------------------------------\n");   
 
    timer.Start();
    if (byRandom)
    {
         PrintfStdOutAndErr("(ConstructIndexByRandom():\n");
         ListIndexing::ConstructIndexByRandom(seed, recordCount);
    }
    else
    {
        PrintfStdOutAndErr("(ConstructIndexFromFile():\n");
        recordCount =  ListIndexing::ConstructIndexFromFile("E:\\Fund\\LCSSIndexingCGB\\FeatureData");
    }   
    PrintfStdOutAndErr("(recordCount %d)(ConstructIndexByRandom  Time  %ds)\n",    
            recordCount, timer.ElapsedSecond()
            );
  

    timer.Start();
    ListIndexing::SearchByDLCS(sampleId,delta,maxDist);
    forceTime = timer.ElapsedMillisecond();
    PrintfStdOutAndErr("------(Brute Force Time  %d ms)\n",forceTime);
   

    timer.Start();
    ListIndexing::SearchByLB_DLCS(sampleId,delta,maxDist);
    long long  dlcsTime = timer.ElapsedMillisecond();
    PrintfStdOutAndErr("------(LB_DLCS Time  %d ms)\n",dlcsTime);
    PrintfStdOutAndErr("------(Ratio %.2f)\n",forceTime * 1.0f / dlcsTime);
  
}


void TestThresholdIndexing()
{
    Timer timer;

    PrintfStdOutAndErr("\nTestThresholdIndexing()-----------------------------------\n");   

    timer.Start();
    if (byRandom)
    {
        PrintfStdOutAndErr("(ConstructIndexByRandom():\n");
        ThresholdIndexing::ConstructIndexByRandom(seed, recordCount);
    }
    else
    {
        PrintfStdOutAndErr("(ConstructIndexFromFile():\n");
        recordCount =  ThresholdIndexing::ConstructIndexFromFile("E:\\Fund\\LCSSIndexingCGB\\FeatureData");
    }   
    PrintfStdOutAndErr("(recordCount %d)(ConstructIndexByRandom  Time  %ds)\n",    
        recordCount, timer.ElapsedSecond()
        );


    timer.Start();
    ThresholdIndexing::SearchByLB_DLCS(sampleId,delta,maxDist);
    long long  dlcsTime = timer.ElapsedMillisecond();
    PrintfStdOutAndErr("------(LB_DLCS Time  %d ms)\n",dlcsTime);
    PrintfStdOutAndErr("------(Ratio %.2f)\n",forceTime * 1.0f / dlcsTime);

}

void TestIndexingOneTime()
{
    PrintfStdOutAndErr("\n===============================================================================\n");
    PrintfStdOutAndErr("(recordCount %d)(SeriesLength %d)(AlphabetLength %d)(delta %d)(maxDist %.2f)\n",
        recordCount,SeriesLength, AlphabetLength,delta,maxDist);
    TestListIndexing();
    //TestVectorIndexing();
    TestThresholdIndexing();
}
void TestIndexing()
{
    TestIndexingOneTime();
}

