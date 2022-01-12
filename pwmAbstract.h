/**
@file		pwmAbstract.h
@brief		pwm classes

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.0

*/


#ifndef _PWM_ABSTRACT_H_
#define _PWM_ABSTRACT_H_

#include "global.h"
#include "TimerAbstract.h"

class pwmAbstract
{
public:
	pwmAbstract(TimerAbstract *__timer);
	~pwmAbstract();

	virtual void manager(){};
	
	virtual void setPwmFrq(double frq);
	virtual double getPwmFrq(){return m_timer->getFrequency();}

	virtual void setPwmPeriod(double T);
	virtual double getPwmPeriod(){return m_timer->getPeriod();}

	virtual void setPwmTonDuty(word Ton);
	virtual word getPwmTonDuty(){return m_pwmTonDuty;}

	virtual void setPwmToffDuty(word Toff);
	virtual word getPwmToffDuty(){return m_pwmToffDuty;}

	virtual void setEnable(bool en){m_timer->setEnable(en);}
	virtual bool isEnabled(){return m_timer->isEnable();}
	
	inline void rstNumPulse(){m_numPulse = 0;}
	inline void setNumPulse(unsigned long np){m_numPulse = np;}
	inline unsigned long  getNumPulse(){return m_numPulse;}

	inline void newPulse(){m_pulseCount++;}
	inline void rstPulse(){m_pulseCount = 0;}
	inline unsigned long getPulseCount(){return m_pulseCount;}
	
	virtual bool pulseCountIsEnabled(){return m_pulseCountEnable;} 
	virtual void setPulseCountEnable(bool en){m_pulseCountEnable = en; m_countingEnded = false;}
	
	virtual bool pulseCountingIsEnded(){return m_countingEnded;} 
	virtual void setPulseCountingEnded(bool val){m_countingEnded = val;} 

	virtual void pulseCountingEnded_ISR(){m_countingEnded = true;} 
	
protected:
	void setBaseFrequency(double frq){m_timer->setBaseFrequency(frq);}		
	double getBaseFrequency(){return m_timer->getBaseFrequency();}

	void setPwmDuty(word duty){m_pwmDuty = duty;}
	word getPwmDuty(){return m_pwmDuty;}

	TimerAbstract * m_timer;
private:
	
	word m_pwmTonDuty;
	word m_pwmToffDuty;
	word m_pwmDuty;

	dword m_pulseCount;
	dword m_numPulse;

	bool m_pulseCountEnable;
	bool m_countingEnded;
};



class pwMeasurementAbstract
{
public:

	enum TimeUnit 
	{
		tu_sec = 0,
		tu_msec,
		tu_usec,
		tu_nsec,
	};
	struct PulseWidthMeasurement
	{
		dword timeHigh;
		dword timeLow;
	};

	enum MeasurementState
	{
		measurement_low = 0,
		measurement_high,
	};
	
	pwMeasurementAbstract(TimerAbstract *__timer){m_timer = __timer;};
	~pwMeasurementAbstract(){};

	virtual void setEnable(bool en){m_timer->setEnable(en);}
	virtual bool isEnable(){return m_timer->isEnable();}

	virtual void setTimeBase(dword time, TimeUnit unit){m_baseTime = time; m_timeUnit = unit;};
	virtual dword getBaseTime(){return m_baseTime;}
	virtual TimeUnit getTimeUnit(){return m_timeUnit;}
	
	virtual bool getPulseMeasurement(PulseWidthMeasurement &pm){ return false;};
	virtual bool getPulseMeasurementLow(dword &low){return false;};
	virtual bool getPulseMeasurementHigh(dword &High){return false;};

	virtual bool pulseMeasureAvailable(){return false;}
	virtual bool pulseMeasureAvailableLow(){return false;}
	virtual bool pulseMeasureAvailableHigh(){return false;}

	virtual void clear(){};
	
	virtual void manager(){};

	virtual double timeUnitToSecond(TimeUnit t)
		{
			switch (t)
			{
				case tu_sec:
					return 1.0;
				case tu_msec:
					return 1.0e-3;
				case tu_usec:
					return 1.0e-6;
				case tu_nsec:
					return 1.0e-9;
			}
		};
protected:
	
	virtual bool popCountHigh(dword &count){return false;};
	virtual bool pushCountHigh(dword count){return false;};

	virtual bool popCountLow(dword &count){return false;};
	virtual bool pushCountLow(dword count){return false;};

	virtual MeasurementState whatMeasuring(){return m_whatMeasuring;}
	virtual void setMeasurementState(MeasurementState measure){m_whatMeasuring = measure;}

	TimerAbstract * m_timer;
	
private:
	
	dword m_baseTime;
	TimeUnit m_timeUnit;
	MeasurementState m_whatMeasuring;

	
};



#endif























