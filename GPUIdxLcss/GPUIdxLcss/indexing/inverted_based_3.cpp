#include "inverted_based.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include "lcss.h"
#include "intersection.h"
#include <setting/defines.h>
#include <lib/browse_dir.h>
#include <lib/timer.h>
#include <lib/util.h>

using namespace Time;

//#define ENABLE_DETAIL_LOG

#ifdef ENABLE_DETAIL_LOG

static FILE * pfDUMP3 = fopen("debug_inverted_3.log","w");
static void Dump(const char* szFmt, ...)
{
    va_list ap;
    va_start(ap, szFmt);
    vfprintf(pfDUMP3, szFmt, ap);	
    va_end(ap);	
    fprintf(pfDUMP3,"\n");
}
#else

#ifdef _MSC_VER
#define Dump(x) NULL		
#pragma warning(disable: 4002)
#else
#define Dump(...) 		
#endif 

#endif // #ifdef ENABLE_DETAIL_LOG

namespace ThresholdIndexing
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
    typedef vector<HeadTail> HeadTailVector;


    static HeadTailVector gAllHeadTails;

    // 存放不完全求交的结果
    static ListSet gResultList; 

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
        
        //每个倒排索引后加一个INT_MAX，以便实现论文中的算法
        for (int alphabetIdx = 0; alphabetIdx < AlphabetLength; ++alphabetIdx)
        {
            for (int lenIdx = 0; lenIdx < SeriesLength; ++lenIdx)
            {    
                gIndexList[alphabetIdx][lenIdx].push_back(INT_MAX);
            }
        }
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

    //每个倒排索引后加一个INT_MAX，以便实现论文中的算法
    for (int alphabetIdx = 0; alphabetIdx < AlphabetLength; ++alphabetIdx)
    {
        for (int lenIdx = 0; lenIdx < SeriesLength; ++lenIdx)
        {    
            gIndexList[alphabetIdx][lenIdx].push_back(INT_MAX);
        }
    }
    printf("(TotalRecordCount %d)\n",recordCount);
}

bool UnboundedSearch(int m, int arrayIdx)
{

    VectorSet::iterator iterBegin = gAllHeadTails[arrayIdx].IteratorBegin;
    VectorSet::iterator iterEnd = gAllHeadTails[arrayIdx].IteratorEnd; 
    VectorSet::iterator left = iterBegin;
    VectorSet::iterator right = iterEnd-1; 
    int key = 0; 
    VectorSet::iterator keyIter;
    bool findKey = false;

    //判断是否用二分查找
    bool binarySearch = false;        
    int eleCount = static_cast<int>(right - left + 1);
#ifdef ENABLE_DETAIL_LOG
    int searchCount = 0;
#endif               
    //元素个数少时顺序查找皆可
    if(eleCount > 3)
    {
        int binSearchCount = static_cast<int>(log(static_cast<double>(eleCount)));
        int minSeqSearchCount = m - *left;
        binarySearch = (binSearchCount < minSeqSearchCount); 
        Dump("(binSearchCount %d)(minSeqSearchCount %d)(*left %d)",binSearchCount,minSeqSearchCount, *left);
    }

    if( !binarySearch)
    {
        for ( VectorSet::iterator it  = iterBegin; 
            it != iterEnd; ++it )
        {
#ifdef ENABLE_DETAIL_LOG
            ++searchCount;
#endif
            key = *it;
            if( key >= m)
            {
                findKey = (key == m);                
                keyIter = it;
                break;
            }
        }
    }
    // 折半查找
    else 
    {
         if( m < *left)
         {
             findKey = false;
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
                key = * mid;
                if(key == m)
                {
                    findKey = true;
                    keyIter = mid;
                    break;
                }
                else if(key < m )
                {
                    left = mid ;
                }
                else
                {
                    //如果根本找不到m，也挑出循环
                    if(*(mid -1) < m)
                    {
                        findKey = false;
                        keyIter = mid;                        
                        break;
                    }
                    else
                    {                                                                     
                        right = mid;  
                    }
                }
			 }
       }
    }
#ifdef ENABLE_DETAIL_LOG
        Dump("(eleCount %d)(binarySearch %s)(m %d)(searchCount %d)",
            eleCount,
            binarySearch ? "true" : "false",
            m, searchCount);
#endif  
    gAllHeadTails[arrayIdx].IteratorBegin = keyIter;        

    return findKey;
}


//参照论文实现，使用数组
void InserctionByT_Threshold_Array(int t) 
{   
    int k = static_cast<int>(gAllHeadTails.size());
    int k_t_add_1 = k - t + 1;
    int k_t = k - t;
    int t_1 = t - 1;
    int * YES   = new int [k];
    int * NO    = new int [k];
    int * H     = new int [k];
    int * MAYBE = new int [k];
    int maxYesIdx = -1;
    int maxNoIdx = -1;
    int maxMaybeIdx = -1;
    int maxHIdx = -1;
    int m = 0;
    for (int idx = 0; idx < k_t_add_1; ++idx)
    {
        H[++maxHIdx] = idx;
    }
    for (int idx = k_t_add_1; idx < k; ++idx)
    {
        MAYBE[++maxMaybeIdx] = idx;
    }
    
    while (m != INT_MAX)
    {
        //把H_ListSize填满
        while(maxHIdx < k_t)
        {
            if (maxYesIdx >= 0 )
            {                
                H[++maxHIdx] = YES[maxYesIdx--];                 
            }
            else
            {
                break;
            }
        }
        while(maxHIdx < k_t)
        {
            if (maxNoIdx >= 0 )
            {                
                H[++maxHIdx] = NO[maxNoIdx--];                
            }
            else
            {
                break;
            }
        }   

        //把剩下的添加到MAYBE中
        while ( maxYesIdx >=0 )
        {
            MAYBE[++maxMaybeIdx] = YES[maxYesIdx--];
        }
        while ( maxNoIdx >=0 )
        {
            MAYBE[++maxMaybeIdx] = NO[maxNoIdx--];
        }

        //计算m,
        m = INT_MAX;
        for (int idx = 0 ; idx <= maxHIdx; ++idx )
        {
            m = min(m,*gAllHeadTails[H[idx]].IteratorBegin);
        }
       
        if(m == INT_MAX)  break;
        // 处理H中的数据
        while (maxHIdx >=0 )
        {
            int arrayIdx = H[maxHIdx];
            if (m == *gAllHeadTails[arrayIdx].IteratorBegin)
            {
                YES[++maxYesIdx] = arrayIdx;                
            }
            else
            {
                NO[++maxNoIdx] = arrayIdx;
            }
            --maxHIdx;
        }

        // 处理MAYBE中的数据
        while (maxYesIdx < t_1 && maxNoIdx < k_t)
        {
            while (maxMaybeIdx >= 0)
            {
                int arrayIdx = MAYBE[maxMaybeIdx];
                if(arrayIdx  < 0 ||  200 < arrayIdx )
                {
                    int i = 0;
                }
                if( UnboundedSearch(m,arrayIdx) )
                {
                    YES[++maxYesIdx] = arrayIdx;
                }
                else
                {
                    NO[++maxNoIdx] = arrayIdx;
                }
                --maxMaybeIdx;
            }
        }
        if(maxYesIdx >= t_1)
        {
            gResultList.push_back(m);            
        }
        for (int idx = 0; idx <= maxYesIdx; ++idx)
        {
            int arrayIdx = YES[idx];
            if(*gAllHeadTails[arrayIdx].IteratorBegin != INT_MAX)
            {
                ++gAllHeadTails[arrayIdx].IteratorBegin;
            } 
            else
            {
                int i = 0;
            }
        }
    }

    delete [] YES;
    delete [] NO;
    delete [] MAYBE;
    delete [] H;    
}

struct IndexVal 
{
    int Index;
    int Value;
    IndexVal(){}
    IndexVal( int idx, int val): Index(idx),Value(val){}
};

struct Greater
{
    bool operator()(const IndexVal &a1, const IndexVal a2) const
    {
        return a1.Value > a2.Value;
    }
};

typedef  priority_queue< IndexVal, vector<IndexVal>, Greater>  PriorityQueue;

//参照论文实现,使用优先队列
void InserctionByT_Threshold_PriorityQueue(int t) 
{   
    int k = static_cast<int>(gAllHeadTails.size());
    int k_t_add_1 = k - t + 1;
    int k_t = k - t;
    int t_1 = t - 1;
    int * YES   = new int [k];
    int * NO    = new int [k];
    PriorityQueue  H;
    //int * H     = new int [k];
    int * MAYBE = new int [k];
    int maxYesIdx = -1;
    int maxNoIdx = -1;
    int maxMaybeIdx = -1;
   // int maxHIdx = -1;
    int m = 0;
    IndexVal indexVal;
    for (int idx = 0; idx < k_t_add_1; ++idx)
    {
        //H[++maxHIdx] = idx;
        H.push(IndexVal(idx,*gAllHeadTails[idx].IteratorBegin));
    }
    for (int idx = k_t_add_1; idx < k; ++idx)
    {
        MAYBE[++maxMaybeIdx] = idx;
    }

    while (m != INT_MAX)
    {
        //把H_ListSize填满
        while(H.size() < k_t_add_1)
        {
            if (maxYesIdx >= 0 )
            {                
                //H[++maxHIdx] = YES[maxYesIdx--]; 
                 int idx = YES[maxYesIdx--];
                 H.push(IndexVal(idx,*gAllHeadTails[idx].IteratorBegin));
            }
            else
            {
                break;
            }
        }
        while(H.size() < k_t_add_1)
        {
            if (maxNoIdx >= 0 )
            {                
                //H[++maxHIdx] = NO[maxNoIdx--];   
                int idx = NO[maxNoIdx--];
                H.push(IndexVal(idx,*gAllHeadTails[idx].IteratorBegin));
            }
            else
            {
                break;
            }
        }   

        //把剩下的添加到MAYBE中
        while ( maxYesIdx >=0 )
        {
            MAYBE[++maxMaybeIdx] = YES[maxYesIdx--];
        }
        while ( maxNoIdx >=0 )
        {
            MAYBE[++maxMaybeIdx] = NO[maxNoIdx--];
        }

        //计算m,
        m = INT_MAX;
        if(H.size() > 0)
        {
            m = H.top().Value;
        }
        
        Dump("(m %d)============",m);
        
        
        if(m == INT_MAX)  break;
        // 处理H中的数据
        while (H.size() > 0 )
        {
            
            int arrayIdx = H.top().Index;
            Dump("(value %d)",arrayIdx);
            if (m == *gAllHeadTails[arrayIdx].IteratorBegin)
            {
                YES[++maxYesIdx] = arrayIdx;                
            }
            else
            {
                NO[++maxNoIdx] = arrayIdx;
            }
            H.pop();            
        }

        // 处理MAYBE中的数据
        while (maxYesIdx < t_1 && maxNoIdx < k_t)
        {
            while (maxMaybeIdx >= 0)
            {
                int arrayIdx = MAYBE[maxMaybeIdx];
                if(arrayIdx  < 0 ||  200 < arrayIdx )
                {
                    int i = 0;
                }
                if( UnboundedSearch(m,arrayIdx) )
                {
                    YES[++maxYesIdx] = arrayIdx;
                }
                else
                {
                    NO[++maxNoIdx] = arrayIdx;
                }
                --maxMaybeIdx;
            }
        }
        if(maxYesIdx >= t_1)
        {
            gResultList.push_back(m);            
        }
        for (int idx = 0; idx <= maxYesIdx; ++idx)
        {
            int arrayIdx = YES[idx];
            if(*gAllHeadTails[arrayIdx].IteratorBegin != INT_MAX)
            {
                ++gAllHeadTails[arrayIdx].IteratorBegin;
            }          
        }
    }

    delete [] YES;
    delete [] NO;
    delete [] MAYBE; 
}



void SearchByLB_DLCS_Threshold(VectorSet sample,int delta, int minMatchCount)
{
    PrintfStdOutAndErr("SearchByLB_DLCS_Threshold():\n");
    
    Timer timer;
    timer.Start();
    HeadTail listHeadTail;   
    for (int idx = 0 ; idx < SeriesLength; ++idx)
    {
        int alpha = sample[idx];
        for (int deltaIdx = -delta; deltaIdx <= delta; ++ deltaIdx)
        {
            int lenIdx = idx + deltaIdx;
            //判断是否越界
            if(0 <= lenIdx && lenIdx < SeriesLength && gIndexList[alpha][lenIdx].size() > 0)
            {
                listHeadTail.IteratorBegin = gIndexList[alpha][lenIdx].begin();
                listHeadTail.IteratorEnd = gIndexList[alpha][lenIdx].end();
                gAllHeadTails.push_back(listHeadTail);
            }
        }		
    }

    InserctionByT_Threshold_Array(minMatchCount);
    //InserctionByT_Threshold_PriorityQueue(minMatchCount);

    PrintfStdOutAndErr("(FirstSelectTime %d)\n",timer.ElapsedSecond());


    int firstSelectCount = 0;  
    int finalMatchCount  = 0;
    for (ListSet::iterator iter = gResultList.begin(); iter != gResultList.end(); ++iter)
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
    SearchByLB_DLCS_Threshold(gSeriesVector[sampleID],delta, minMatchCount);
}

};
