#include "util.h"

#include <iostream>
#include <cstdarg>

using namespace std;

void PrintInt(int val)
{
	cout<<val<<' ';
}

void PrintLnInt(int val)
{
	cout<<val<<endl;
}


void PrintString(std::string &val)
{
	cout<<val<<' ';
}

void PrintLnString(std::string &val)
{
	cout<<val<<endl;
}

void PrintLnString(const char * str)
{
	cout<<str<<endl;
}

void PrintfStdOutAndErr(const char* szFmt, ...)
{
    va_list ap;
    va_start(ap, szFmt);
    vfprintf(stdout, szFmt, ap);	
    vfprintf(stderr, szFmt, ap);	
    va_end(ap);	    
}

