#include "m_timerFunction.h"
#include "windows.h"	


m_timerFunction::m_timerFunction()
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;

	LARGE_INTEGER tempTime;

	QueryPerformanceFrequency(&tempTime);   // 8 MHz

	this->m_frequency = static_cast<unsigned long long>(tempTime.QuadPart);
}
m_timerFunction::~m_timerFunction()
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter(&tempTime);
	this->m_startTime = static_cast<unsigned long long>(tempTime.QuadPart);
}


void m_timerFunction::Start(void)
{
	
}

void m_timerFunction::Stop(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter(&tempTime);
	this->m_stopTime = static_cast<unsigned long long>(tempTime.QuadPart);
}

void m_timerFunction::Reset(void)
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;
}

float m_timerFunction::GetElapsedSeconds(void)
{
	// Update current time
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter(&tempTime);
	this->m_stopTime = static_cast<unsigned long long>(tempTime.QuadPart);

	// Calculate elapsed time
	this->m_elapsedTime = this->m_stopTime - this->m_startTime;
	float elapsedTimeSec = static_cast<float>(this->m_elapsedTime) / this->m_frequency;
	return elapsedTimeSec;
}

unsigned long long m_timerFunction::GetFrequency(void)
{
	return this->m_frequency;
}