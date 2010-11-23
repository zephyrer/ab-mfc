#ifndef _TIME_TIMER_H_
#define _TIME_TIMER_H_

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#pragma warning(disable:4996)
#else
#include <sys/time.h>
#include <time.h>
#endif

namespace Time
{
	struct DateTime 
	{
		int Year;
		int Month;
		int Day;
		int Hour;
		int Minute;
		int Second;
	};

	//! A 64-bits time type.
	typedef unsigned long long TimeType;

	class Timer
	{
	public:    

#ifdef _WIN32 
		static TimeType Frequency;
#endif

		Timer()
		{
#ifdef _WIN32
			if (Frequency == 0)
			{        
				QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency); 
			}
			//secondsPerTick = 1.0 / Frequency ;
#endif
		} 

		virtual ~Timer()
		{
		}


		// 开始计时
		inline void Start()
		{
#ifdef _WIN32    
			LARGE_INTEGER li;        
			QueryPerformanceCounter(&li);       
			m_start = static_cast<TimeType>(li.QuadPart);
#else        
			struct timeval tv;     
			gettimeofday(&tv, 0);
			m_start = static_cast<TimeType>(1000000*(tv.tv_sec)+tv.tv_usec);
#endif
		}

		//从调用Start(),到调用改方法，逝去的时间，单位：us
		inline TimeType ElapsedMicrosecond()
		{
#ifdef _WIN32    
			LARGE_INTEGER li;        
			QueryPerformanceCounter(&li);       
			return (static_cast<TimeType>(li.QuadPart)-m_start)*1000000/Frequency;

#else        
			struct timeval tv;     
			gettimeofday(&tv, 0);
			return 1000000*(tv.tv_sec)+tv.tv_usec-m_start;
#endif
		}

		// 延迟n毫秒
		static void Delay(int nMS);

		//从调用Start()到调用改方法 逝去的时间，单位：ms
		inline TimeType ElapsedMillisecond()
		{
			return  ElapsedMicrosecond() / 1000;
		}

		//从调用Start()到调用改方法 逝去的时间，单位：s
		inline TimeType ElapsedSecond()
		{
			return  ElapsedMicrosecond() /1000000;
		}

		// 获取本地当前时间
		inline static void LocalTime(DateTime & dataTime)
		{
			time_t ltime;
			struct tm *now;
#ifdef _WIN32
			_time64(&ltime);
			now = _localtime64(&ltime);
#else
			time(&ltime);   
			now   =   localtime(&ltime);   
#endif
			dataTime.Year    = now->tm_year + 1900;
			dataTime.Month   = now->tm_mon +1;
			dataTime.Day     = now->tm_mday;
			dataTime.Hour    = now->tm_hour;
			dataTime.Minute  = now->tm_min;
			dataTime.Second  = now->tm_sec;
		}

	private:
		TimeType m_start;
		//double secondsPerTick;
	};

}; //Time


#endif //_TIME_TIMER_H_

