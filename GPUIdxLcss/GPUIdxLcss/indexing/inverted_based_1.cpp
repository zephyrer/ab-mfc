#include "inverted_based.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <map>
#include "lcss.h"
#include "intersection.h"
#include <setting/defines.h>
#include <lib/browse_dir.h>
#include <lib/timer.h>
#include <lib/util.h>

using namespace Time;

//#define ENABLE_DETAIL_LOG

#ifdef ENABLE_DETAIL_LOG


static FILE * pfDUMP1 = fopen("debug_inverted_1.log","w");
static void Dump(const char* szFmt, ...)
{
    va_list ap;
    va_start(ap, szFmt);
    vfprintf(pfDUMP1, szFmt, ap);	
    va_end(ap);	
    fprintf(pfDUMP1,"\n");
}
#else

#ifdef _MSC_VER
#define Dump(x) NULL		
#pragma warning(disable: 4002)
#else
#define Dump(...) 		
#endif 

#endif // #ifdef ENABLE_DETAIL_LOG


const char * SolutionTypeName[] = 
{
    "FindOneSolution",
    "MayHaveSolution",
    "NoSolution"
};



namespace ListIndexing
{


static ListSet gIndexList[AlphabetLength][SeriesLength];
static vector<VectorSet> gSeriesVector;
static vector<string> gSeriesNameVector;
static int gSeriesID = 0;
static int gMaxSeriesLen = 0;
static int gMinSeriesLen = 100000;



struct HeadTail
{
    ListSet::iterator IteratorBegin;
    ListSet::iterator IteratorEnd;
};
typedef vector<HeadTail> HeadTailVector;
typedef list<HeadTail> HeadTailList;

struct HeadTailListWithCount 
{
    int Count; // SameHeadValueHeadList的大小
    HeadTailList SameHeadValueHeadList; //所有IteratorBegin对应值相同的 HeadTail
    HeadTailListWithCount():Count(0){};
};
typedef map<int,HeadTailListWithCount> HeadTailListMap;


static  HeadTailListMap headTailListMap;

static  map<int,int> articleCountInSet;




static void ProcessFile(const char * fileName)
{
	ifstream fin(fileName);
	int dataIdx = 0;
	int dataValue;
	VectorSet set;
	while ( fin >> dataValue && dataIdx < SeriesLength)
	{
		AssertLog(0 <= dataValue && dataValue < AlphabetLength,"(dataValue %d)",dataValue);
		gIndexList[dataValue][dataIdx].push_back(gSeriesID);
		set.push_back(dataValue);
		gSeriesNameVector.push_back(fileName);		
		++dataIdx;
	}
	fin.close();
	gSeriesVector.push_back(set);
	++gSeriesID;
}

int ConstructIndexFromFile(const char * strDir)
{
	CBrowseDir browser;
	if( browser.SetInitDir(strDir) )
	{
		gSeriesID = 0;
		gSeriesNameVector.clear();
		browser.Browse("*.log",ProcessFile);
 		printf("(TotalRecordCount %d)\n",
 			gSeriesID+1);
        return gSeriesID + 1;
	}
	else
	{
		printf("Error:void ConstructIndexFromFile(%s)\n",strDir);
        exit(1);
	}
    return 0;
}

void ConstructIndexByRandom(unsigned int seed,int recordCount)
{
	VectorSet set(SeriesLength);
	srand(seed);
	int value;
	for(int recordIdx = 0; recordIdx < recordCount; ++recordIdx)
	{
		for (int dataIdx = 0; dataIdx < SeriesLength; ++dataIdx)
		{
			value = (AlphabetLength -1) * rand() / RAND_MAX;
			set[dataIdx] = value;
			gIndexList[value][dataIdx].push_back(recordIdx);
		}
		gSeriesVector.push_back(set);
	}
	PrintfStdOutAndErr("(TotalRecordCount %d)\n",recordCount);
}


// 根据样本检索
// delta是允许的位置偏差
// minMatchCount表示最小匹配数
void SearchByLB_DLCS_1(VectorSet sample,int delta, int minMatchCount)
{
	Intersection intersec;
	for (int idx = 0 ; idx < SeriesLength; ++idx)
	{
		int alpha = sample[idx];
		for (int deltaIdx = -delta; deltaIdx <= delta; ++ deltaIdx)
		{
			int lenIdx = idx + deltaIdx;
			if(0 <= lenIdx && lenIdx < SeriesLength)
			{
				intersec.PushSet(gIndexList[alpha][lenIdx]);
			}
		}		
	}
	intersec.UpdateResultBySort(minMatchCount);
	
	int firstSelectCount = static_cast<int>(intersec.mResultList.size()) ;
	int finalMatchCount = 0;	

	for ( vector<VectorSet>::iterator it = gSeriesVector.begin(); it != gSeriesVector.end(); ++it)
	{
		if( LCSSDelta(sample, *it, delta) > minMatchCount )
		{
			++finalMatchCount;
		}
	}
	PrintfStdOutAndErr("(firstSelectCount %d)(finalMatchCount %d)\n",firstSelectCount,finalMatchCount);
}


/**************************************************
 建一个总记录数大小的数组，统计每个记录出现的次数。
 之后遍历这个数组， 如果记录出现次数大于minMatchCount，
 则进行计算LCSSDelta的值是否真正符合要求
***************************************************/
void SearchByLB_DLCS_2(VectorSet sample,int delta, int minMatchCount)
{
	AssertLog(sample.size() == SeriesLength,"(sample.size %d)",sample.size());
	vector <int> articleCountInSet(gSeriesVector.size());
	Intersection intersec;
	for (int idx = 0 ; idx < SeriesLength; ++idx)
	{	
		int alpha = sample[idx];
		for (int deltaIdx = -delta; deltaIdx <= delta; ++ deltaIdx)
		{
			int lenIdx = idx + deltaIdx;

			if(0 <= lenIdx && lenIdx < SeriesLength)
			{
				ListSet::iterator begin = gIndexList[alpha][lenIdx].begin();
				ListSet::iterator end = gIndexList[alpha][lenIdx].end();
				for (ListSet::iterator it =begin; it != end; ++it)
				{
					++articleCountInSet[*it];
				}
			}
		}		
	}
	
	int finalMatchCount  = 0;
	int firstSelectCount = 0;
	for ( int articleIdx =  0; articleIdx < static_cast<int>(gSeriesVector.size()); ++articleIdx)
	{
		if( articleCountInSet[articleIdx] > minMatchCount)
		{
			++firstSelectCount;
			if( LCSSDelta(sample, gSeriesVector[articleIdx], delta) > minMatchCount )
			{
				++finalMatchCount;
			}			
		}
	}
	printf("(firstSelectCount %d)(finalMatchCount %d)\n",firstSelectCount,finalMatchCount);
}


/**************************************************
建一个map，统计记录出现的次数。之后遍历这个map，
如果记录出现次数大于minMatchCount，则计算LCSSDelta
的值是否真正符合要求。
***************************************************/
void SearchByLB_DLCS_3(VectorSet sample,int delta, int minMatchCount)
{
    PrintfStdOutAndErr("List, SearchByLB_DLCS_3():\n");
    Timer timer;
    timer.Start();
    
    map<int,int>::iterator mapIterator;
   
    Intersection intersec;
    for (int idx = 0 ; idx < SeriesLength; ++idx)
    {	
        int alpha = sample[idx];
        for (int deltaIdx = -delta; deltaIdx <= delta; ++ deltaIdx)
        {
            int lenIdx = idx + deltaIdx;

            if(0 <= lenIdx && lenIdx < SeriesLength)
            {
                ListSet::iterator begin = gIndexList[alpha][lenIdx].begin();
                ListSet::iterator end = gIndexList[alpha][lenIdx].end();
                for (ListSet::iterator it =begin; it != end; ++it)
                {
                    int value = *it;
                    mapIterator = articleCountInSet.find(value);
                    if(mapIterator != articleCountInSet.end())
                    {
                        ++mapIterator->second;
                    }
                    else
                    {
                        articleCountInSet[value] = 0;
                    }               
                }
            }
        }		
    }
    PrintfStdOutAndErr("(FirstSelectTime %d)\n",timer.ElapsedSecond());

    int finalMatchCount  = 0;
    int firstSelectCount = 0;
    for (map<int,int>::iterator iter = articleCountInSet.begin(); iter != articleCountInSet.end(); ++iter)
    {
        int articleId = iter->first;
        int count = iter->second;
        if (count >= minMatchCount)
        {
            ++firstSelectCount;
            if( LCSSDelta(sample, gSeriesVector[articleId], delta) > minMatchCount )
            {
                ++finalMatchCount;
            }			
        }
    }
    PrintfStdOutAndErr("(firstSelectCount %d)(finalMatchCount %d)\n",firstSelectCount,finalMatchCount);
}



Solution SearchSeries(int minMatchCount)
{
    Solution solution;
    HeadTailListMap::iterator mapIter;
    int accumulateCount = 0;
    int maxHeadValue = -1;
    for (mapIter = headTailListMap.begin(); mapIter != headTailListMap.end(); ++mapIter)
    {
        int seriesID = mapIter->first;
        int seriesCount = mapIter->second.Count;
        accumulateCount += seriesCount;
        if(accumulateCount >= minMatchCount)
        {
            maxHeadValue = seriesID; 
            break;
        }
       // printf("(%d %d)\n", mapIter->first,mapIter->second.Count); 
    }
    if(accumulateCount >= minMatchCount)
    {
        solution.Value =  maxHeadValue;
        // 第一个满足要求
        if(maxHeadValue == headTailListMap.begin()->first)
        {
            solution.Type = FindOneSolution;
        }
        //需要重新排序
        else
        {
            solution.Type = MayHaveSolution;
        }
    }
    //accumulateCount太小，根本不可能找到合格的序列了
    else
    {
        solution.Type = NoSolution;
    }
    return solution;
}


void ReArangeList(HeadTailList &headTailList, int minHeadValue)
{
#ifdef ENABLE_DETAIL_LOG
    Dump("\nReArangeList_1(%d), :-------------------------",headTailList.size());  
#endif
    HeadTailListMap::iterator mapIter;
 
    for ( HeadTailList::iterator itHL = headTailList.begin();
          itHL != headTailList.end(); ++itHL)
    {
#ifdef ENABLE_DETAIL_LOG
        int searchCount = 0;
        int eleCount =  static_cast< int >( distance(itHL->IteratorBegin,itHL->IteratorEnd) );
#endif  
        for ( ListSet::iterator it = itHL->IteratorBegin; 
              it != itHL->IteratorEnd; ++it )
        {
#ifdef ENABLE_DETAIL_LOG
            ++searchCount;
#endif
            int key = *it;
            if( key >= minHeadValue)
            {
#ifdef ENABLE_DETAIL_LOG
                Dump("(eleCount %d)(minHeadValue %d)(searchCount %d)",eleCount, minHeadValue, searchCount);
#endif
                itHL->IteratorBegin = it;
                mapIter = headTailListMap.find(key);
                if(mapIter != headTailListMap.end())
                {
                    mapIter->second.SameHeadValueHeadList.push_back(*itHL);
                    ++mapIter->second.Count;
                }
                else
                {
                    HeadTailListWithCount headTailListWithCount;
                    headTailListWithCount.Count = 1;
                    headTailListWithCount.SameHeadValueHeadList.push_back(*itHL);
                    headTailListMap[key] = headTailListWithCount;
                }
                break;
            }
        }
    }
}

/**************************************************

***************************************************/
void SearchByLB_DLCS_4(VectorSet sample,int delta, int minMatchCount)
{
    PrintfStdOutAndErr("List, SearchByLB_DLCS_4():\n");

    Timer timer;
    timer.Start();
    HeadTailList listIterators; 
    HeadTail listHeadTail;   
    for (int idx = 0 ; idx < SeriesLength; ++idx)
    {
        int alpha = sample[idx];
        for (int deltaIdx = -delta; deltaIdx <= delta; ++ deltaIdx)
        {
            int lenIdx = idx + deltaIdx;
            if(0 <= lenIdx && lenIdx < SeriesLength && gIndexList[alpha][lenIdx].size() > 0)
            {
                listHeadTail.IteratorBegin = gIndexList[alpha][lenIdx].begin();
                listHeadTail.IteratorEnd = gIndexList[alpha][lenIdx].end();
                listIterators.push_back(listHeadTail);
            }
        }		
    }

    //初始化headTailListMap
    HeadTailListMap::iterator mapIter;
    int key = 0;
    for (HeadTailList::iterator listIter = listIterators.begin(); listIter != listIterators.end(); ++listIter)
    {
        key = *listIter->IteratorBegin;
        mapIter =  headTailListMap.find(key);
        if ( mapIter != headTailListMap.end() )
        {
            mapIter->second.SameHeadValueHeadList.push_back(*listIter);
            ++mapIter->second.Count;
        }
        else
        {
            HeadTailListWithCount headTailListWithCount;
            headTailListWithCount.Count = 1;
            headTailListWithCount.SameHeadValueHeadList.push_back(*listIter);
            headTailListMap[key] = headTailListWithCount;
        }
    }

    Solution solution;
    ListSet resultList;
    int minHeadValue = 0;
    int searchCount = 0;
    while (true)
    {
       solution = SearchSeries( minMatchCount);
       //printf("(solution %s %d)\n",SolutionTypeName[solution.Type], solution.Value);
       if (solution.Type == NoSolution) 
       {
           break;
       }
       if (solution.Type == FindOneSolution)
       {
           resultList.push_back(solution.Value);
           //下一个解的最小值为solution.Value + 1
           minHeadValue = solution.Value + 1; 
       }
       if (solution.Type == MayHaveSolution) 
       {
           //下一个解的最小值为solution.Value
           minHeadValue = solution.Value;
       }

       // 删除小于minHeadValue的元素
       for (mapIter = headTailListMap.begin(); mapIter != headTailListMap.end();)
       {            
            if( mapIter->first < minHeadValue )
            {         
                HeadTailList headTailList; 
                headTailList.splice(headTailList.begin(),mapIter->second.SameHeadValueHeadList);
                //删除节点
                HeadTailListMap::iterator deleteIter = mapIter;
                ++mapIter;
                headTailListMap.erase(deleteIter);
                //重新安排节点
                ReArangeList(headTailList,minHeadValue);
            }
            else
            {
                break;
            }
       }

        ++searchCount;
        //printf("(searchCount %d)=============================:\n" ,searchCount);
        //for (mapIter = headTailListMap.begin(); mapIter != headTailListMap.end(); ++mapIter)
        //{
        //    printf("(%d %d)\n", mapIter->first, mapIter->second.Count);
        //}
       
        //if(searchCount == 50 )break;
    }
    PrintfStdOutAndErr("(FirstSelectTime %d)\n",timer.ElapsedSecond());

    
    int firstSelectCount = 0;  
    int finalMatchCount  = 0;
    for (ListSet::iterator iter = resultList.begin(); iter != resultList.end(); ++iter)
    {
        ++firstSelectCount;
        int seriesID = *iter;
        if (LCSSDelta(gSeriesVector[seriesID],sample,delta) > minMatchCount)
        {
            ++finalMatchCount;
        }
    }
    
    PrintfStdOutAndErr("(firstSelectCount %d)(finalMatchCount %d)\n",firstSelectCount,finalMatchCount);
}


void SearchByLB_DLCS(int sampleID,int delta,float maxDistance)
{
    //最小匹配次数
    int minMatchCount = static_cast<int>(SeriesLength * (1.0f - maxDistance));
    PrintfStdOutAndErr("(minMatchCount %d)\n",minMatchCount);
	SearchByLB_DLCS_3(gSeriesVector[sampleID],delta, minMatchCount);
}

void SearchByDLCS(int sampleID,int delta,float maxDistance)
{
	//最小匹配次数
	int minMatchCount = static_cast<int>(SeriesLength * (1.0f - maxDistance));
	int recordCount = static_cast< int >(gSeriesVector.size());
	int matchRecordCount = 0;
	VectorSet sample = gSeriesVector[sampleID];

	for ( int recordIdx = 0; recordIdx < recordCount; ++recordIdx)
	{
		if( LCSSDelta(sample,gSeriesVector[recordIdx],delta) > minMatchCount )
		{
			++matchRecordCount;
			//printf("(%d %s)\n",matchRecordCount,gFileNameVector[recordIdx].c_str());
		}
	}
	PrintfStdOutAndErr("(matchCount %d)\n",matchRecordCount);
}

};
