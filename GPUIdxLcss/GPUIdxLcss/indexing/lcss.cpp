#include "lcss.h"
#include "inverted_based.h"
#include <cmath>
#include <iostream>


using namespace std;

//const int MAX_COUNT = 800;
//int matrix[MAX_COUNT][MAX_COUNT];
int matrix[SeriesLength+1][SeriesLength+1];

int LCSS(std::vector<int> &vec1, std::vector<int> &vec2)
{
	memset(matrix,0,sizeof(matrix));
	int rowCount = static_cast<int>(vec1.size());
	int colCount = static_cast<int>(vec2.size());

	// ��0�е�0�ж�Ϊ0����Ϊ��ʼֵ
	for ( int rowIdx = 1; rowIdx <= rowCount; ++rowIdx)
	{
		for (int colIdx = 1; colIdx <= colCount; ++colIdx )
		{
			if( vec1[rowIdx-1] == vec2[colIdx-1] ) // �ַ������������ô�0����
			{
				matrix[rowIdx][colIdx] = matrix[rowIdx-1][colIdx-1] + 1;	
			}
			else if(matrix[rowIdx][colIdx-1] > matrix[rowIdx-1][colIdx])
			{
				matrix[rowIdx][colIdx] = matrix[rowIdx][colIdx-1];
			}
			else
			{
				matrix[rowIdx][colIdx] = matrix[rowIdx-1][colIdx];
			}
		}
	}
	return matrix[rowCount][colCount];
}

int LCSSDelta(std::vector<int> &vec1, std::vector<int> &vec2, int delta)
{
	memset(matrix,0,sizeof(matrix));
	int rowCount = static_cast<int>(vec1.size());
	int colCount = static_cast<int>(vec2.size());

	for (int rowIdx=1;rowIdx<=rowCount;++rowIdx)
	{
		for (int colIdx= 1;colIdx<= colCount ;++colIdx)
		{
			if(vec1[rowIdx-1] == vec2[colIdx-1] && abs(rowIdx - colIdx) <= delta)
			{
				matrix[rowIdx][colIdx]=1+matrix[rowIdx-1][colIdx-1];//��ǰ���ַ����
			}
			else if (matrix[rowIdx][colIdx-1]>matrix[rowIdx-1][colIdx])
			{
				matrix[rowIdx][colIdx]=matrix[rowIdx][colIdx-1];
			}
			else 
			{
				matrix[rowIdx][colIdx]=matrix[rowIdx-1][colIdx];
			}
		}
	}
	return matrix[rowCount][colCount];
}


