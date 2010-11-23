#include "inverted_based.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
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

static FILE * pfDUMP2 = fopen("debug_inverted_2.log","w");
static void Dump(const char* szFmt, ...)
{
    va_list ap;
    va_start(ap, szFmt);
    vfprintf(pfDUMP2, szFmt, ap);	
    va_end(ap);	
    fprintf(pfDUMP2,"\n");
}
#else

#ifdef _MSC_VER
#define Dump(x) NULL		
#pragma warning(disable: 4002)
#else
#define Dump(...) 		
#endif 

#endif // #ifdef ENABLE_DETAIL_LOG


namespace VectorIndexing
{
    static VectorSet gIndexList[AlphabetLength][SeriesLength];
    static vector<VectorSet> gSeriesVector;
    static vector<string> gSeriesNameVector;
    static int gSeriesID = 0;

    struct HeadTail
    {
        VectorSet::iterator IteratorBegin;
        VectorSet::iterator IteratorEnd;
    };
    typedef list<HeadTail> HeadTailList;

    struct HeadTailListWithCount 
    {
        int Count; // SameHeadValueHeadList�Ĵ�С
        HeadTailList SameHeadValueHeadList; //����IteratorBegin��Ӧֵ��ͬ�� HeadTail
        HeadTailListWithCount():Count(0){};
    };
    typedef map<int,HeadTailListWithCount> HeadTailListMap;


    static HeadTailListMap headTailListMap;

    static map<int,int> articleCountInSet;


void ProcessFile(const char * fileName)
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
    printf("(TotalRecordCount %d)\n",recordCount);
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
        // ��һ������Ҫ��
        if(maxHeadValue == headTailListMap.begin()->first)
        {
            solution.Type = FindOneSolution;
        }
        //��Ҫ��������
        else
        {
            solution.Type = MayHaveSolution;
        }
    }
    //accumulateCount̫С�������������ҵ��ϸ��������
    else
    {
        solution.Type = NoSolution;
    }
    return solution;
}


void ReArangeList_1(HeadTailList &headTailList, int minHeadValue)
{
#ifdef ENABLE_DETAIL_LOG
    Dump("\nReArangeList_1(%d), :-------------------------",headTailList.size());  
#endif
    HeadTailListMap::iterator mapIter;
    for ( HeadTailList::iterator itHL = headTailList.begin();
        itHL != headTailList.end(); ++itHL)
    {

        VectorSet::iterator left = itHL->IteratorBegin;
        VectorSet::iterator right = itHL->IteratorEnd -1;        
               
        //û������ �� ��������С��minHeadValue ����
        if(right == left) continue;
        if(*right < minHeadValue) continue;

        int key = 0; 
        VectorSet::iterator keyIter;
        bool findKey = false;

        //�ж��Ƿ��ö��ֲ���
        
        bool binarySearch = false;        
        int eleCount = static_cast<int>(right - left + 1);
#ifdef ENABLE_DETAIL_LOG
        int searchCount = 0;
#endif               
        //Ԫ�ظ�����ʱ˳����ҽԿ�
        if(eleCount > 3)
        {
            int binSearchCount = static_cast<int>(log(static_cast<double>(eleCount)));
            int minSeqSearchCount = minHeadValue - *left;
            binarySearch = (binSearchCount < minSeqSearchCount); 
            Dump("(binSearchCount %d)(minSeqSearchCount %d)(*left %d)",binSearchCount,minSeqSearchCount, *left);
        }

        if( !binarySearch)
        {
            for ( VectorSet::iterator it  = itHL->IteratorBegin; 
                it != itHL->IteratorEnd; ++it )
            {
#ifdef ENABLE_DETAIL_LOG
                ++searchCount;
#endif
                key = *it;
                if( key >= minHeadValue)
                {
                    findKey = true;
                    keyIter = it;
                    break;
                }
            }
        }
        // �۰����
        else 
        {
            // �ж�һ��Ԫ���Ƿ�>= minHeadValue
            if(*left >= minHeadValue)
            {
#ifdef ENABLE_DETAIL_LOG
                ++searchCount;
#endif
                findKey = true;
                key = *left;
                keyIter = left;
            }
            else
            {
                
                while (true)
                {
#ifdef ENABLE_DETAIL_LOG
                    ++searchCount;
#endif
                    VectorSet::iterator mid = left + ((right - left) >> 1); 
                    int test = *mid;
                    if(*mid < minHeadValue )
                    {
                        //mid = left + ((right - left) >> 1);                         
                        left = mid ;
                    }
                    else
                    {
                        // *(mid -1) < minHeadValue <= *mid ��������Ҫ��
                        if(*(mid -1) < minHeadValue)
                        {
                            findKey = true;
                            keyIter = mid;
                            key = * mid;
                            break;
                        }
                        else
                        {                                             
                            //mid = left + ((right - left) >> 1);
                            right = mid;  
                        }
                    }
                }
            }                      
        }


        if( findKey)
        {
#ifdef ENABLE_DETAIL_LOG
            Dump("(eleCount %d)(binarySearch %s)(minHeadValue %d)===(searchCount %d)",
                eleCount,
                binarySearch ? "true" : "false",
                minHeadValue, searchCount);
#endif            
            itHL->IteratorBegin = keyIter;
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
    
        }
    }
}


void SearchByLB_DLCS_4(VectorSet sample,int delta, int minMatchCount)
{
    PrintfStdOutAndErr("Vector, SearchByLB_DLCS_4():\n");
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
            //�ж��Ƿ�Խ��
            if(0 <= lenIdx && lenIdx < SeriesLength && gIndexList[alpha][lenIdx].size() > 0)
            {
                listHeadTail.IteratorBegin = gIndexList[alpha][lenIdx].begin();
                listHeadTail.IteratorEnd = gIndexList[alpha][lenIdx].end();
                listIterators.push_back(listHeadTail);
            }
        }		
    }

    //��ʼ��headTailListMap
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
      // printf("(solution %s %d)\n",SolutionTypeName[solution.Type], solution.Value);
        if (solution.Type == NoSolution) 
        {
            break;
        }
        if (solution.Type == FindOneSolution)
        {
            resultList.push_back(solution.Value);
            //��һ�������СֵΪsolution.Value + 1
            minHeadValue = solution.Value + 1; 
        }
        if (solution.Type == MayHaveSolution) 
        {
            //��һ�������СֵΪsolution.Value
            minHeadValue = solution.Value;
        }

        // ɾ��С��minHeadValue��Ԫ��
        for (mapIter = headTailListMap.begin(); mapIter != headTailListMap.end();)
        {            
            if( mapIter->first < minHeadValue )
            {         
                HeadTailList headTailList; 
                headTailList.splice(headTailList.begin(),mapIter->second.SameHeadValueHeadList);
                //ɾ���ڵ�
                HeadTailListMap::iterator deleteIter = mapIter;
                ++mapIter;
                headTailListMap.erase(deleteIter);
                //���°��Žڵ�              
                ReArangeList_1(headTailList,minHeadValue);
            }
            else
            {
                break;
            }
        }

//         ++searchCount;
//         printf("(searchCount %d)=============================:\n" ,searchCount);
//         for (mapIter = headTailListMap.begin(); mapIter != headTailListMap.end(); ++mapIter)
//         {
//             printf("(%d %d)\n", mapIter->first, mapIter->second.Count);
//         }
// 
//         if(searchCount == 5 )break;
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
    //��Сƥ�����
    int minMatchCount = static_cast<int>(SeriesLength * (1.0f - maxDistance));
    PrintfStdOutAndErr("(minMatchCount %d)\n",minMatchCount);
    SearchByLB_DLCS_4(gSeriesVector[sampleID],delta, minMatchCount);
}

void SearchByDLCS(int sampleID,int delta,float maxDistance)
{
    //��Сƥ�����
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
