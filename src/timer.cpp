/*
    Copyright 2011 Joe Hermaszewski. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY Joe Hermaszewski "AS IS" AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Joe Hermaszewski OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the
    authors and should not be interpreted as representing official policies, either expressed
    or implied, of Joe Hermaszewski.
*/

#include "timer.hpp"

#include <cstdlib>

namespace NTime
{
    CTimer::CTimer()
    {
        m_running = false;
        
    #ifdef _WIN32
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        m_recipFrequency = 1.0 / double(frequency.QuadPart);
        m_startTime.QuadPart = 0;
        m_endTime.QuadPart = 0;
    #else
        m_startTime.tv_sec = 0;
        m_startTime.tv_usec = 0;
        m_endTime.tv_sec =  0;
        m_endTime.tv_usec = 0;
    #endif
    }

    CTimer::~CTimer()
    {
    }

    void CTimer::Start()
    {
    #ifdef _WIN32
        QueryPerformanceCounter(&m_startTime);
    #else
        gettimeofday(&m_startTime, nullptr);
    #endif
        m_running = true;
    }

    void CTimer::Stop()
    {
    #ifdef _WIN32
        QueryPerformanceCounter(&m_endTime);
    #else
        gettimeofday(&m_endTime, nullptr);
    #endif
        m_running = false;
    }

    double CTimer::GetElapsedTime()
    {
    #ifdef _WIN32
        if(m_running)
            QueryPerformanceCounter(&m_endTime);
        
        return double(m_endTime.QuadPart - m_startTime.QuadPart) * m_recipFrequency;
    #else
        if(m_running)
            gettimeofday(&m_endTime, nullptr);
        
        return (m_endTime.tv_sec - m_startTime.tv_sec) + double(m_endTime.tv_usec - m_startTime.tv_usec) * 0.0000001;
    #endif
    }
};
