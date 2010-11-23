#include "timer.h"

using namespace Time;

#ifdef _WIN32
TimeType Timer::Frequency = 0;
#endif


void Timer::Delay(int nMS)
{
	TimeType startTime,curTime,delayTime;
	
#ifdef	WIN32
	static TimeType frequency = 0;
	if( frequency == 0 )
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);  
	}
	
	LARGE_INTEGER li;        
	QueryPerformanceCounter(&li);       
	startTime = static_cast<TimeType>(li.QuadPart);
	curTime =  startTime;

	delayTime = 0;
	while ( delayTime < nMS )
	{
		QueryPerformanceCounter(&li);       
		curTime = static_cast<TimeType>(li.QuadPart);
		delayTime = (curTime - startTime) * 1000 /frequency;
	}
#else
	struct timeval tv;     
	gettimeofday(&tv, 0);
	startTime = static_cast<TimeType>(1000000*(tv.tv_sec)+tv.tv_usec);
	curTime =  startTime;
	delayTime = 0;
	while ( delayTime < nMS )
	{
		gettimeofday(&tv, 0);   
		curTime = static_cast<TimeType>(1000000*(tv.tv_sec)+tv.tv_usec);
		delayTime = (curTime - startTime) / 1000;
	}

#endif
}

