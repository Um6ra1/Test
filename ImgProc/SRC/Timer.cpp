#include "Timer.h"

void CTimer::Start()
{
	::gettimeofday(&m_tvStart, NULL);
}

void CTimer::End()
{
	::gettimeofday(&m_tvEnd, NULL);
}

double CTimer::GetElapsedTime()
{
	return((double)(m_tvEnd.tv_usec - m_tvStart.tv_usec) * 1.0e-6 + (double)(m_tvEnd.tv_sec - m_tvStart.tv_sec));
}
