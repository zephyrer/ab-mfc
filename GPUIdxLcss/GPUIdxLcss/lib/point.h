#ifndef _POINT_H_
#define _POINT_H_

#include <cmath>
#include <setting/defines.h>
#include <string.h>


namespace SIE
{
	class Point
	{
	public:
		enum Constants
		{
			DIMENSIONALITY = 93 // ���ά��
		};
		Point();
		~Point();

		const double Get( int idx)
		{
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			return m_dData[idx];
		}

		void Set( int idx , double value)
		{
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			m_dData[idx] = value;
		}

		// �����ÿһά����Ϊͬ����ֵ
		void Fill(double value); 

		// �����ֵ��Ϊ0
		void Zero()
		{
			Fill(0.0);
		}

		// ��ֵ����
		double operator[](const int  idx) const 
		{ 
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			return m_dData[idx]; 
		}	

		// ��ֵ����
		double & operator[](const int  idx) 
		{ 
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			return m_dData[idx]; 
		}

		// ���ز����� =
		const Point& operator = (const Point &point)
		{
			 memcpy(m_dData,point.m_dData,sizeof(m_dData));
			 return *this;
		}

		// ���ز����� +-
		const Point& operator += (const Point &point);
	
		// ���ز����� -=
		const Point& operator -= (const Point &point);

		// ���ز����� *=
		const Point& operator *= ( double value );
		
		// ���ز����� /=
		const Point& operator /= ( double value );

		// һԪ������ -  
		Point operator-() const;
		
		Point operator+(const Point & point) const;
		Point operator-(const Point & point) const;
		Point operator*( double value) const;
		Point operator/( double value) const;		

		const double LengthSquare();

		const double Length()
		{
			return sqrt( LengthSquare());
		}

		const double GetDistanceSquareTo( const Point &point) const;

		const double GetDistanceTo( const Point &point) const
		{
			return sqrt( GetDistanceSquareTo( point ) );
		}

		// ������ļ�
		void Printf(FILE * pf,const char * strFormat);

		// ��� TODO
		double Dot(const Point &point) const;
	
		// ��� TODO
		double  Cross(const Point &point ) const;

	private:
		double m_dData[DIMENSIONALITY];
	};


};
#endif