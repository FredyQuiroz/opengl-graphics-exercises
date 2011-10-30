#pragma once
#include <crtdbg.h>

#ifdef _DEBUG
#define TRACEMAXSTRING	1024

void TRACE(const char* format,...);

#define TraceEx _snprintf(szBuffer,TRACEMAXSTRING,"%s(%d): ", \
                          &strrchr(__FILE__,'\\')[1],__LINE__); \
_RPT0(_CRT_WARN,szBuffer); \
Trace

#else
# define TRACE(x)
 // void TRACE(0);
 // void TRACEEX(0);
#endif