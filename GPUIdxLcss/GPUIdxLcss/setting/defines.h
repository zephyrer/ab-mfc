#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdio.h>
#include <stdlib.h>

//关闭此宏定义，自动关闭调试相关的宏定义
#define DEBUG_PROJECT

// 标准终端输入开关，Print
#define LOG_TERMINAL


#ifndef DEBUG_PROJECT
  #undef LOG_TERMINAL
#endif


#pragma   warning(disable:4996)   //全部关掉4996警告


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


