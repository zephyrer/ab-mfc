#ifndef _INVERTED_BASED_H_
#define _INVERTED_BASED_H_ 

enum IndexingConstants
{
   SeriesLength   = 20, // 序列长度
   AlphabetLength = 10  // 字母集大小
};

enum 
{
    FindOneSolution, // 找到一个解，可以接着找下一个解
    MayHaveSolution, // 肯能有解，需要接着找
    NoSolution,      // 无解了，不用再找了
};

struct Solution 
{
    int Type;  // 返回类型
    int Value; // 值，如果Typ = FindOneSolution，表示
    Solution():Type(NoSolution),Value(0){}
};

const char * SolutionTypeName[];

namespace ListIndexing
{

// 读取所有的数据文件，建立索引,返回总记录数
int ConstructIndexFromFile(const char * strDir);

// 随机创建recordCount条数据，初始化随机种子为randNumber
void ConstructIndexByRandom(unsigned int seed,int recordCount);

// 用LB_DLCS在索引上进行检索
void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

// 用DLCS顺序检索
void SearchByDLCS(int sampleID,int delta,float maxDistance);
};



namespace VectorIndexing
{
    // 读取所有的数据文件，建立索引,返回总记录数
    int ConstructIndexFromFile(const char * strDir);

    // 随机创建recordCount条数据，初始化随机种子为randNumber
    void ConstructIndexByRandom(unsigned int seed,int recordCount);
    // 用LB_DLCS在索引上进行检索
    void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

    // 用DLCS顺序检索
    void SearchByDLCS(int sampleID,int delta,float maxDistance);

};

namespace ThresholdIndexing
{
    // 读取所有的数据文件，建立索引,返回总记录数
    int ConstructIndexFromFile(const char * strDir);

    // 随机创建recordCount条数据，初始化随机种子为randNumber
    void ConstructIndexByRandom(unsigned int seed,int recordCount);
    // 用LB_DLCS在索引上进行检索
    void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

    // 用DLCS顺序检索
    void SearchByDLCS(int sampleID,int delta,float maxDistance);

};


#endif
