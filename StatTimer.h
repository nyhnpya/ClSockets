/*----------------------------------------------------------------------------*/
/*                                                                            */
/* StatTimer.h: interface for the CStatTimer class.                           */
/*                                                                            */
/* Author: Mark Carrier (mark@carrierlabs.com)                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Copyright (c) 2013 CarrierLabs, LLC. All Rights Reserved.                  *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   *
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                *
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN *
 *  NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  *
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     *
 *  OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 *  USE OR OTHER DEALINGS IN THE SOFTWARE.                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*/
#ifndef __CSTATTIMER_H__
#define __CSTATTIMER_H__

#include <string.h>

#if WIN32
  #include <Winsock2.h>
  #include <time.h>
#endif 

#ifdef _LINUX
  #include <stdio.h>
  #include <sys/time.h>
#endif

#include "host.h"

#if defined(WIN32)
  #define GET_CLOCK_COUNT(x) QueryPerformanceCounter((LARGE_INTEGER *)x)
#else
  #define GET_CLOCK_COUNT(x) gettimeofday(x, NULL)
#endif

#define MILLISECONDS_CONVERSION 1000
#define MICROSECONDS_CONVERSION 1000000

/// Class to abstract socket communications in a cross platform manner.
/// This class is designed 
class CStatTimer {
public:
    CStatTimer() 
    {
    };

    ~CStatTimer()
    {
    };

    void Initialize() 
    { 
        memset(&m_startTime, 0, sizeof(struct timeval));
        memset(&m_endTime, 0, sizeof(struct timeval));
    };

    struct timeval GetStartTime() { return m_startTime; };
    void SetStartTime() { GET_CLOCK_COUNT(&m_startTime); };

    struct timeval GetEndTime() { return m_endTime; };
    void SetEndTime() { GET_CLOCK_COUNT(&m_endTime); };

    uint32_t GetMilliSeconds() { return (CalcTotalUSec() / MILLISECONDS_CONVERSION); };
    uint32_t GetMicroSeconds() { return (CalcTotalUSec()); };
    uint32_t GetSeconds() { return (CalcTotalUSec() / MICROSECONDS_CONVERSION); };

    uint32_t GetCurrentTime() 
    { 
        struct timeval tmpTime;
        GET_CLOCK_COUNT(&tmpTime); 
        return ((tmpTime.tv_sec * MICROSECONDS_CONVERSION) + tmpTime.tv_usec);
    };

private:
    uint32_t CalcTotalUSec() { return (((m_endTime.tv_sec - m_startTime.tv_sec) * MICROSECONDS_CONVERSION) + 
                                   (m_endTime.tv_usec - m_startTime.tv_usec)); };


private:
    struct timeval  m_startTime;
    struct timeval  m_endTime;
};

#endif // __CSTATTIMER_H__
