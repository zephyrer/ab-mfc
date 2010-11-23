#ifndef _TIMESERIES_H_
#define _TIMESERIES_H_

#include <setting/defines.h>
#include "point.h"
#include <vector>

namespace SIE
{
	class TimeSeries
	{
	public:
		TimeSeries();
		~TimeSeries();

		// 右值访问
		const Point & operator[](const int  idx) const 
		{ 
			AssertLog(0 <= idx && idx < (int)m_serise.size(), "Error:invalid idx!");
			return m_serise[idx]; 
		}	

		// 左值访问
		Point & operator[](const int  idx) 
		{ 
			AssertLog(0 <= idx && idx < (int)m_serise.size(), "Error:invalid idx!");
			return m_serise[idx]; 
		}

		// 获取序列长度
		const int GetLength()const
		{
			return (int)m_serise.size();
		}
		void Resize( int newSize )
		{
			m_serise.resize(newSize);	
		}
		// 预分配容量
		void Reserve(int size)
		{
			m_serise.reserve(size);
		}
		//追加点
		void PushBack(const Point &point)
		{
			m_serise.push_back(point);
		}

		// 计算序列平均点 并返回
		const Point & CalcAveragePoint();

		// 返回序列的平均点
		const Point & GetAveragePoint();

		// 从文件中读取时间序列
		void ReadFromFile( const char * fileName);

		// 从时间序列写到文件中
		void WriteToFile( const char * fileName);

	private:
		std::vector<Point> m_serise;
		Point m_averagePoint;
	};

};

#endif