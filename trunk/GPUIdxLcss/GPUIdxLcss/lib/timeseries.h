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

		// ��ֵ����
		const Point & operator[](const int  idx) const 
		{ 
			AssertLog(0 <= idx && idx < (int)m_serise.size(), "Error:invalid idx!");
			return m_serise[idx]; 
		}	

		// ��ֵ����
		Point & operator[](const int  idx) 
		{ 
			AssertLog(0 <= idx && idx < (int)m_serise.size(), "Error:invalid idx!");
			return m_serise[idx]; 
		}

		// ��ȡ���г���
		const int GetLength()const
		{
			return (int)m_serise.size();
		}
		void Resize( int newSize )
		{
			m_serise.resize(newSize);	
		}
		// Ԥ��������
		void Reserve(int size)
		{
			m_serise.reserve(size);
		}
		//׷�ӵ�
		void PushBack(const Point &point)
		{
			m_serise.push_back(point);
		}

		// ��������ƽ���� ������
		const Point & CalcAveragePoint();

		// �������е�ƽ����
		const Point & GetAveragePoint();

		// ���ļ��ж�ȡʱ������
		void ReadFromFile( const char * fileName);

		// ��ʱ������д���ļ���
		void WriteToFile( const char * fileName);

	private:
		std::vector<Point> m_serise;
		Point m_averagePoint;
	};

};

#endif