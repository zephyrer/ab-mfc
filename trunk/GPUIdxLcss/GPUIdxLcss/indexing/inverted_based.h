#ifndef _INVERTED_BASED_H_
#define _INVERTED_BASED_H_ 

enum IndexingConstants
{
   SeriesLength   = 20, // ���г���
   AlphabetLength = 10  // ��ĸ����С
};

enum 
{
    FindOneSolution, // �ҵ�һ���⣬���Խ�������һ����
    MayHaveSolution, // �����н⣬��Ҫ������
    NoSolution,      // �޽��ˣ�����������
};

struct Solution 
{
    int Type;  // ��������
    int Value; // ֵ�����Typ = FindOneSolution����ʾ
    Solution():Type(NoSolution),Value(0){}
};

const char * SolutionTypeName[];

namespace ListIndexing
{

// ��ȡ���е������ļ�����������,�����ܼ�¼��
int ConstructIndexFromFile(const char * strDir);

// �������recordCount�����ݣ���ʼ���������ΪrandNumber
void ConstructIndexByRandom(unsigned int seed,int recordCount);

// ��LB_DLCS�������Ͻ��м���
void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

// ��DLCS˳�����
void SearchByDLCS(int sampleID,int delta,float maxDistance);
};



namespace VectorIndexing
{
    // ��ȡ���е������ļ�����������,�����ܼ�¼��
    int ConstructIndexFromFile(const char * strDir);

    // �������recordCount�����ݣ���ʼ���������ΪrandNumber
    void ConstructIndexByRandom(unsigned int seed,int recordCount);
    // ��LB_DLCS�������Ͻ��м���
    void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

    // ��DLCS˳�����
    void SearchByDLCS(int sampleID,int delta,float maxDistance);

};

namespace ThresholdIndexing
{
    // ��ȡ���е������ļ�����������,�����ܼ�¼��
    int ConstructIndexFromFile(const char * strDir);

    // �������recordCount�����ݣ���ʼ���������ΪrandNumber
    void ConstructIndexByRandom(unsigned int seed,int recordCount);
    // ��LB_DLCS�������Ͻ��м���
    void SearchByLB_DLCS(int sampleID,int delta,float maxDistance);

    // ��DLCS˳�����
    void SearchByDLCS(int sampleID,int delta,float maxDistance);

};


#endif
