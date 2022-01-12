/**
@file		pwm.cpp
@brief		pwm classes

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/

#include "pwm_NEC.h"




NECpwmTimer::NECpwmTimer(Timer_NEC *__timer, word pwmDutyMax): pwmAbstract(__timer)
{		
	((Timer_NEC*)m_timer)->setTimerMode(Timer_NEC::tm_pwmOutputMode, true);
	
	setBaseFrequency(20.0e+6);
	setPwmDuty(pwmDutyMax);
	setPwmFrq(1.0);
	setPwmTonDuty(0);
	setPulseCountEnable(false);
	setEnable(false);
}

NECpwmTimer::~NECpwmTimer()
{
	
}

void NECpwmTimer::setPwmFrq(double frq)
{
	((Timer_NEC*)m_timer)->setFrequency(frq, Timer_NEC::_CCR0);
	
	setPwmTonDuty(getPwmTonDuty());
}

void NECpwmTimer::setPwmPeriod(double T)
{
	((Timer_NEC*)m_timer)->setPeriod(T, Timer_NEC::_CCR0);
	
	setPwmTonDuty(getPwmTonDuty());
}

void NECpwmTimer::setPwmTonDuty(word Ton)
{
	word CCR1;
	
	pwmAbstract::setPwmTonDuty(Ton);
	
	CCR1 = (getPwmToffDuty() * ((Timer_NEC*)m_timer)->getCCR0()) / getPwmDuty();
	((Timer_NEC*)m_timer)->setCCR1(CCR1);
}

void NECpwmTimer::setPwmToffDuty(word Toff)
{
	word CCR1;
	
	pwmAbstract::setPwmToffDuty(Toff);
	
	CCR1 = (getPwmToffDuty() * ((Timer_NEC*)m_timer)->getCCR0()) / getPwmDuty();
	((Timer_NEC*)m_timer)->setCCR1(CCR1);
	
}

void NECpwmTimer::setEnable(bool en,  bool rstPulseCount)
{
	pwmAbstract::setEnable(en);
	m_timer->setEnable(en);
	if (!en && rstPulseCount)
	{
		setPulseCountEnable(false);
		setPulseCountingEnded(false);
		rstPulse();
		rstNumPulse();
	}
}


void NECpwmTimer::setPulseCountEnable(bool en)
{
	pwmAbstract::setPulseCountEnable(en);
	((Timer_NEC*)m_timer)->setCCIC0_interrupt(en, 4);
}





NECpwMeasurentTimer::NECpwMeasurentTimer(Timer_NEC *__timer, Timer_NEC::CCRn _ccrn ): pwMeasurementAbstract(__timer)
{
	m_ccrn = _ccrn;
	((Timer_NEC*)m_timer)->setTimerMode(Timer_NEC::tm_pulseWidthMeasurementMode, true);
	m_timer->setBaseFrequency(20e+6);
	m_fifoCountLow.clear();
	m_fifoCountHigh.clear();
	setMeasurementState(measurement_high);
	setEnable(false);
	setTimeBase(100, tu_usec);
	m_timerOverflow = 0;
	
}


NECpwMeasurentTimer::~NECpwMeasurentTimer()
{

}

dword NECpwMeasurentTimer::getTime(dword count)
{
	dword time;
	double convFact;

	convFact = (m_calculatedBaseTime * timeUnitToSecond(m_calcutetedTimeUnit)) / (timeUnitToSecond(getTimeUnit()) * (double)getBaseTime());
	time = (dword)((double)count * convFact); 
	
	return (dword)time;
}

void NECpwMeasurentTimer::rstTimerOverflow()
{
	m_timerOverflow = 0;
}

void NECpwMeasurentTimer::setEnable(bool en)
{
	pwMeasurementAbstract::setEnable(en);
}

void NECpwMeasurentTimer::setTimeBase(dword time, TimeUnit unit)
{
	double dtime[tu_nsec + 2];
	double dprescaler;
	word ccr;
	double frq;
	
	pwMeasurementAbstract::setTimeBase(time, unit);
	
	switch(getTimeUnit())
	{
		case tu_sec:
			frq = 1.0 / (double)getBaseTime();
			break;
		case tu_msec:
			frq = 1.0e+3 / (double)getBaseTime();
			break;
		case tu_usec:
			frq = 1.0e+6 / (double)getBaseTime();
			break;
		case tu_nsec:
			frq = 1.0e+9 / (double)getBaseTime();
			break;
	}
	
	((Timer_NEC*)m_timer)->setFrequency(frq, m_ccrn);
	
	dtime[tu_nsec + 1] = 1.0 / ((Timer_NEC*)m_timer)->calculateFrequency(m_ccrn);
	
	dtime[tu_sec] = dtime[tu_nsec + 1];
	dtime[tu_msec] = dtime[tu_nsec + 1] * 1.0e+3;
	dtime[tu_usec] = dtime[tu_nsec + 1] * 1.0e+6;
	dtime[tu_nsec] = dtime[tu_nsec + 1] * 1.0e+9;
	
	while (( ( (dword)dtime[unit] ) == 0) && (unit < tu_nsec))
	{
		unit = (TimeUnit)(unit + 1);
	}
	
	switch(getTimeUnit())
	{
		case tu_sec:
			frq = 1.0 / dtime[tu_nsec + 1] ;
			break;
		case tu_msec:
			frq = 1.0e+3 / dtime[tu_nsec + 1] ;
			break;
		case tu_usec:
			frq = 1.0e+6 / dtime[tu_nsec + 1] ;
			break;
		case tu_nsec:
			frq = 1.0e+9 / dtime[tu_nsec + 1] ;
			break;
	}
	m_calculatedBaseTime = dtime[unit];
	m_calcutetedTimeUnit = unit;
}

bool NECpwMeasurentTimer::getPulseMeasurement(PulseWidthMeasurement &pm)
{
	dword high;
	dword low;
	dword countHigh;
	dword countLow;
	
	if(m_fifoCountHigh.empty() || m_fifoCountLow.empty())
		return false;

	asm("di");
	m_fifoCountHigh.pop_front(high);
	m_fifoCountHigh.pop_front(countHigh);
	m_fifoCountLow.pop_front(low);
	m_fifoCountLow.pop_front(countLow);
	asm("ei");
	
	pm.timeHigh = getTime((dword)high + (dword)countHigh * (dword)0x10000); 
	pm.timeLow = getTime((dword)low + (dword)countLow * (dword)0x10000);
	return true;
}

bool NECpwMeasurentTimer::getPulseMeasurementLow(dword &low)
{
	dword countLow;
	dword lowPulse;
	
	if(m_fifoCountLow.empty())
		return false;

	asm("di");
	m_fifoCountLow.pop_front(lowPulse);
	m_fifoCountLow.pop_front(countLow);
	asm("ei");
	
	low = getTime((dword)lowPulse + (dword)countLow * (dword)0x10000);
	
	return true;
}

bool NECpwMeasurentTimer::getPulseMeasurementHigh(dword &High)
{
	dword countHigh;
	dword highPulse;
	
	if(m_fifoCountHigh.empty())
		return false;

	asm("di");
	m_fifoCountHigh.pop_front(highPulse);
	m_fifoCountHigh.pop_front(countHigh);
	asm("ei");
	
	High = getTime((dword)highPulse + (dword)countHigh * (dword)0x10000);
	return true;
}

bool NECpwMeasurentTimer::pulseMeasureAvailable()
{
	return (!m_fifoCountHigh.empty() && !m_fifoCountLow.empty());
}

bool NECpwMeasurentTimer::pulseMeasureAvailableLow()
{
	return (!m_fifoCountLow.empty());
}

bool NECpwMeasurentTimer::pulseMeasureAvailableHigh()
{
	return (!m_fifoCountHigh.empty());
}
	

void NECpwMeasurentTimer::manager()
{

}

bool NECpwMeasurentTimer::popCountHigh(dword &count)
{
	 return false;
}

bool NECpwMeasurentTimer::pushCountHigh(dword count)
{
	if(!m_fifoCountHigh.full())
	{
		m_fifoCountHigh.push_back(count);
		return true;
	}
	m_fifoCountHigh.clear();
	m_fifoCountLow.clear();
	return false;
}

bool NECpwMeasurentTimer::popCountLow(dword &count)
{
	return false;
}

bool NECpwMeasurentTimer::pushCountLow(dword count)
{
	if(!m_fifoCountLow.full())
	{
		m_fifoCountLow.push_back(count);
		return true;
	}
	m_fifoCountHigh.clear();
	m_fifoCountLow.clear();
	return false;
}

pwMeasurementAbstract::MeasurementState NECpwMeasurentTimer::whatMeasuring()
{
	return pwMeasurementAbstract::whatMeasuring();
}

void NECpwMeasurentTimer::setMeasurementState(MeasurementState measure)
{
	pwMeasurementAbstract::setMeasurementState(measure);
}




































