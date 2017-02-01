#pragma once

#include <sys/time.h>

class CTimer
{
	timeval	m_tvStart;
	timeval	m_tvEnd;

public:
	void	Start();
	void	End();
	double	GetElapsedTime();
};
