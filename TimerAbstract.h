/**
@file		TimerAbstarct.h
@brief		timer classes

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.0

*/


#ifndef __TIMER_ABSTRACT_H__
#define __TIMER_ABSTRACT_H__

#include "global.h"
class TimerAbstract
{
public:
	TimerAbstract(){};
	~TimerAbstract();

	virtual void setEnable(bool en){m_enable = en;}
	virtual bool isEnable(){return m_enable;}

	virtual void setBaseFrequency(double fxx){m_fxx = fxx;} 
	virtual double getBaseFrequency(){return m_fxx;}
	
	virtual void setFrequency(double frq)
		{
			if (frq < 0)
				frq = 0.0;
			m_frq = frq;
			m_T = 1.0/m_frq;
		}
	virtual double getFrequency(){return m_frq;}

	virtual void setPeriod(double T)
	{
		if(T < 0)
			T = 0.0;

		m_T = T;
		m_frq = 1/m_T;
	}
	virtual double getPeriod(){return m_T;}
	
protected:
	

private:
	bool m_enable;
	double m_frq;
	double m_T;
	double m_fxx;
	
};

























#endif

