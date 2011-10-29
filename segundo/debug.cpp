# include "debug.h"
# include <windows.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void TRACE(const char* format,...)
{
 va_list args;
 int nBuf;

 va_start(args,format);
 
 char szBuffer[TRACEMAXSTRING];
 nBuf = _vsnprintf(szBuffer,
                   TRACEMAXSTRING,
                   format,
                   args);
 va_end(args);

 _RPT0(_CRT_WARN,szBuffer);
}