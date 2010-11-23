#include "point.h"

using namespace SIE;

Point::Point()
{

}

Point::~Point()
{

}

const double Point::GetDistanceSquareTo( const Point &point) const
{
	double dDistSquare = 0.0;
	double dDiff;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		dDiff = m_dData[idx] - point.m_dData[idx];
		dDistSquare += ( dDiff * dDiff );
	}
	return dDistSquare;
}


const double Point::LengthSquare()
{
	double dDistSquare = 0.0;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		dDistSquare += ( m_dData[idx] * m_dData[idx] );
	}
	return dDistSquare;
}

void Point::Fill(double value)
{
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		m_dData[idx] = value;
	}
}


const Point& Point::operator += (const Point &point)
{
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		m_dData[idx] += point.m_dData[idx];
	}
	return *this;
}

const Point& Point::operator -= (const Point &point)
{
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		m_dData[idx] -= point.m_dData[idx];
	}
	return *this;
}

const Point& Point::operator *= ( double value)
{
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		m_dData[idx] *= value;
	}
	return *this;
}

const Point& Point::operator /= ( double value)
{
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		m_dData[idx] /= value;
	}
	return *this;
}

Point Point::operator-() const
{
	Point point;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		point.m_dData[idx] =  -m_dData[idx];
	}
	return point;

}

Point Point::operator+(const Point & point) const
{
	Point tmpPoint;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		tmpPoint.m_dData[idx] = m_dData[idx] + point.m_dData[idx];
	}
	return tmpPoint;
}

Point Point::operator-(const Point & point) const
{
	Point tmpPoint;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		tmpPoint.m_dData[idx] = m_dData[idx] - point.m_dData[idx];
	}
	return tmpPoint;
}

Point Point::operator * ( double value) const
{
	Point tmpPoint;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		tmpPoint.m_dData[idx] = m_dData[idx] * value;
	}
	return tmpPoint;
}

Point Point::operator / ( double value) const
{
	Point tmpPoint;
	for( int idx  = 0; idx < DIMENSIONALITY; ++idx )
	{
		tmpPoint.m_dData[idx] = m_dData[idx] / value;
	}
	return tmpPoint;
}
/*

void Point::Printf(FILE * pf,const char * strFormat)
{
	::fprintf(pf,"(");
	for ( int dimIdx = 0; dimIdx < DIMENSIONALITY; ++dimIdx )
	{
		::fprintf(pf,strFormat,m_dData[dimIdx]);
	}
	::fprintf(pf,")");
}
*/