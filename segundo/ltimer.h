/* ----------------------------------------------------------------------------
Copyright (c) 1999-2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.
    * The name of the author may not be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#ifndef LTIMER_H
#define LTIMER_H

#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif

class LTimer
{
public: 
    LTimer();
    virtual ~LTimer();
    /** UpdateFrequency is called here */
    void Initialize();
    /** returns UpdateTicks() */
    int64 GetTicks();
    /** returns system uptime in microseconds */
    float GetTime();
    /** returns time since last TimeElapsed call, in microseconds */
    float TimeElapsed();
    /** returns the processor frequency */
    int64 GetFrequency();
    /** returns the fps */
    int GetFPS();
    // recalcs the time
    void NewFrame();

	void SetReference();
	float SinceReference();
protected: // Protected methods
    /** update the rtdcs counter */
    int64 UpdateTicks();
    /** this is called to get the resolution, normally /proc/cpuinfo if used,
        if this file cant be opened the func measures the time for 5 seconds to
        find out the resolution */
    void UpdateFrequency();
    /** the rdtcs counter */
    int64 m_ticks;
    /** ticks per second */
    int64 m_frequency;
    /** the last measured ticks */
    int64 m_oldTicks;
};

LTimer* Timer();

#endif
