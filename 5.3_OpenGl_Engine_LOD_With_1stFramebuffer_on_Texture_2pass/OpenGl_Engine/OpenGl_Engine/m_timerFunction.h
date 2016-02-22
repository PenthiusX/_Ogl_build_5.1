#pragma once

#ifndef _Timer_
#define _Timer_

class m_timerFunction
{
public:
	m_timerFunction();
	~m_timerFunction();

	void Start(void);
	void Stop(void);
	void Reset(void);
	float GetElapsedSeconds(void);	// Can also use __int64 (win32 specific)
	unsigned long long GetFrequency(void);
private:
	unsigned long long m_elapsedTime;	// Can also use __int64 (win32 specific)
	unsigned long long m_startTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_stopTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_frequency;		// Can also use __int64 (win32 specific)

};

#endif // _CHRTimer_HG_