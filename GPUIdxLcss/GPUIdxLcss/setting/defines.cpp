#include "defines.h"
#include <stdarg.h>


void TerminalPrint(const char* szFmt, ...)
{
	va_list ap;
	va_start(ap, szFmt);
	::vfprintf(stdout, szFmt, ap);	
	va_end(ap);	
	::fprintf(stdout,"\n");
	::fflush(stdout);       
}


