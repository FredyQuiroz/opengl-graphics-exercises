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
 inline void TRACE(0);
 inline void TRACEEX(0);
#endif