/**
@file		pwm.h
@brief		pwm classes

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/

#ifndef __PWM_NEC_H__
#define __PWM_NEC_H__



#include "global.h"
#include "pwmAbstract.h"
#include "Timer_NEC.h"


class NECpwmTimer: public pwmAbstract
{
public:
	
	NECpwmTimer(Timer_NEC *__timer, word pwmDutyMax = 100);
	~NECpwmTimer();

	void setOutput_00(Timer_NEC::TxnOE0 e, Timer_NEC::TxnOL0 l){ ((Timer_NEC*)m_timer)->setTxnOE0(e); ((Timer_NEC*)m_timer)->setTxnOL0(l);};
	void setOutput_01(Timer_NEC::TxnOE1 e, Timer_NEC::TxnOL1 l){ ((Timer_NEC*)m_timer)->setTxnOE1(e); ((Timer_NEC*)m_timer)->setTxnOL1(l);};
	
	virtual void manager(){};
	
	virtual void setPwmFrq(double frq);

	virtual void setPwmPeriod(double T);

	virtual void setPwmTonDuty(word Ton);

	virtual void setPwmToffDuty(word Toff);

	virtual void setEnable(bool en, bool rstPulseCount = true);

	virtual void setPulseCountEnable(bool en);
	
protected:
	
private:
	
};


class NECpwMeasurentTimer: public pwMeasurementAbstract
{
public:
	
	NECpwMeasurentTimer(Timer_NEC *__timer, Timer_NEC::CCRn _ccrn);
	~NECpwMeasurentTimer();

	virtual void setEnable(bool en);
	
	virtual void setTimeBase(dword time, TimeUnit unit);
	
	virtual bool getPulseMeasurement(PulseWidthMeasurement &pm);
	virtual bool getPulseMeasurementLow(dword &low);
	virtual bool getPulseMeasurementHigh(dword &High);

	virtual bool pulseMeasureAvailable();
	virtual bool pulseMeasureAvailableLow();
	virtual bool pulseMeasureAvailableHigh();
	
	virtual void manager();
	
	virtual bool pushCountHigh(dword count);
	
	virtual bool pushCountLow(dword count);

	virtual MeasurementState whatMeasuring();
	virtual void setMeasurementState(MeasurementState measure);

	virtual void newTimerOverFlow(){m_timerOverflow++;};
	virtual word getTimerOverFlow(){ return m_timerOverflow;};
	virtual void rstTimerOverflow();

	virtual void clear(){asm("di"); m_fifoCountLow.clear(); m_fifoCountHigh.clear(); asm("ei");};
	
protected:

	virtual bool popCountHigh(dword &count);
	virtual bool popCountLow(dword &count);

	dword getTime(dword count);

	Timer_NEC::CCRn m_ccrn;
private:
	CSmallRingBuf <dword, 50> m_fifoCountLow;
	CSmallRingBuf <dword, 50> m_fifoCountHigh;
	
	double m_calculatedBaseTime;
	TimeUnit m_calcutetedTimeUnit;
	
	word m_timerOverflow;
};






#endif


















