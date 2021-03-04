#include <windows.h>
#include "Timer.h"

Timer::Timer()
	: m_SecondsPerCount(0.0), m_DeltaTime(0.0)
	, m_BaseTime(0LL), m_PausedTime(0LL), m_StopTime(0LL), m_PrevTime(0LL), m_CurrentTime(0LL)
	, m_bIsStop(false)
{
	__int64 countsPerSecond;

	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);

	m_SecondsPerCount = 1.0 / static_cast<double>(countsPerSecond);
}

Timer::~Timer()
{

}

float Timer::TotalTime(void) const
{
	if (m_bIsStop)
	{
		return static_cast<float>(static_cast<double>((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	else
	{
		return static_cast<float>(static_cast<double>((m_CurrentTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}

float Timer::DeltaTime(void) const
{
	return static_cast<float>(m_DeltaTime);
}

void Timer::Reset(void)
{
	__int64 currentTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	m_BaseTime = currentTime;
	m_PrevTime = currentTime;
	m_StopTime = 0;
	m_bIsStop = false;
}

void Timer::Start(void)
{
	__int64 startTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_bIsStop)
	{
		m_PausedTime += (startTime - m_StopTime);
		m_PrevTime = startTime;
		m_StopTime = 0;
		m_bIsStop  = false;
	}
}

void Timer::Stop(void)
{
	if (!m_bIsStop)
	{
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		m_StopTime = currentTime;
		m_bIsStop = true;
	}
}

void Timer::Tick(void)
{
	if (m_bIsStop)
	{
		m_DeltaTime = 0.0;
	}
	else
	{
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		m_CurrentTime = currentTime;
		m_DeltaTime = (m_CurrentTime - m_PrevTime) * m_SecondsPerCount;
		m_PrevTime = m_CurrentTime;

		if (m_DeltaTime < 0.0)
		{
			m_DeltaTime = 0.0;
		}
	}
}