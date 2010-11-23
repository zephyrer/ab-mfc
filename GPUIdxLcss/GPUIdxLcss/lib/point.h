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
			DIMENSIONALITY = 93 // 点的维度
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

		// 将点的每一维都设为同样的值
		void Fill(double value); 

		// 将点的值设为0
		void Zero()
		{
			Fill(0.0);
		}

		// 右值访问
		double operator[](const int  idx) const 
		{ 
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			return m_dData[idx]; 
		}	

		// 左值访问
		double & operator[](const int  idx) 
		{ 
			AssertLog(0 <= idx && idx < DIMENSIONALITY, "Error:invalid idx!");
			return m_dData[idx]; 
		}

		// 重载操作符 =
		const Point& operator = (const Point &point)
		{
			 memcpy(m_dData,point.m_dData,sizeof(m_dData));
			 return *this;
		}

		// 重载操作符 +-
		const Point& operator += (const Point &point);
	
		// 重载操作符 -=
		const Point& operator -= (const Point &point);

		// 重载操作符 *=
		const Point& operator *= ( double value );
		
		// 重载操作符 /=
		const Point& operator /= ( double value );

		// 一元操作符 -  
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

		// 输出到文件
		void Printf(FILE * pf,const char * strFormat);

		// 点乘 TODO
		double Dot(const Point &point) const;
	
		// 叉乘 TODO
		double  Cross(const Point &point ) const;

	private:
		double m_dData[DIMENSIONALITY];
	};


};
#endif