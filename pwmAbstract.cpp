/**
@file		pwmAbstract.cpp
@brief		pwm classes

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.0
*/

#include "pwmAbstract.h"


pwmAbstract::pwmAbstract(TimerAbstract *__timer)
{
	m_timer = __timer;
	setBaseFrequency(20.0e+6);
	setPwmDuty(100);
	setPwmFrq(1.0);
	setPwmTonDuty(0);
	setPulseCountEnable(false);
	rstPulse();
	rstNumPulse();
}

pwmAbstract::~pwmAbstract()
{

}

void pwmAbstract::setPwmFrq(double frq)
{
	m_timer->setFrequency(frq);
}

void pwmAbstract::setPwmPeriod(double T)
{
	m_timer->setPeriod(T);
}


void pwmAbstract::setPwmTonDuty(word Ton)
{
	double dTon;
	if (Ton > m_pwmDuty)
		Ton = m_pwmDuty;

	m_pwmTonDuty = Ton;
	m_pwmToffDuty = m_pwmDuty - m_pwmTonDuty;
}

void pwmAbstract::setPwmToffDuty(word Toff)
{
	double dToff;
	if (Toff > m_pwmDuty)
		Toff = m_pwmDuty;

	m_pwmToffDuty = Toff;
	m_pwmTonDuty = m_pwmDuty - m_pwmToffDuty;
}



































