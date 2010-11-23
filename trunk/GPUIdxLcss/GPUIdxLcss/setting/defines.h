#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdio.h>
#include <stdlib.h>

//�رմ˺궨�壬�Զ��رյ�����صĺ궨��
#define DEBUG_PROJECT

// ��׼�ն����뿪�أ�Print
#define LOG_TERMINAL


#ifndef DEBUG_PROJECT
  #undef LOG_TERMINAL
#endif


#pragma   warning(disable:4996)   //ȫ���ص�4996����


#ifdef LOG_TERMINAL
   void TerminalPrint(const char* szFmt, ...);
   
   #define Print TerminalPrint
   #define TRACE_LINE Print("%s(%d)", __FILE__, __LINE__)
   #define AssertLog(x,...) if(!(x)){TRACE_LINE;Print(__VA_ARGS__);exit(0);}
#else
  #ifdef _MSC_VER
    #define Print(x) NULL
    #define AssertLog(x) NULL
  #else
    #define Print(...)
    #define AssertLog(...) 
  #endif
#endif


#endif //_DEFINES_H_


