/**
@file		WTH.cpp
@brief		Classes to manage the puller (WTH)

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#include "wth.h"


#define pin_WTH_DIRECTION 	PinDIR_M2
#define pin_WTH_PWR		PinEN_M2

#define pin_LMTSW_LOW		PinWTH_FCL		
#define pin_LMTSW_HIGH		PinWTH_FCH

#define LMTSW_REACHED		0
#define LMTSW_NOT_REACHED	1

#define WTH_DIRECTION_PULL	0
#define WTH_DIRECTION_BACK	1


WthMenfis::WthMenfis(pwmAbstract* pwm, pwmAbstract* pwmStep)
{
	m_pwm = pwm;
	m_pwmStep = pwmStep;

	m_pwmStep->setPwmFrq(1000);
	m_pwmStep->setPwmTonDuty(50);
	
	power(false);
	checkLimitSwitchesStatus();
	setEnable(false);
	setLinearVelocity(0);
	setStep(0);
	m_limitSwitchTimerError = false;
		
	m_motorStep = 200.0;
	m_motoruStep = 256.0;
	m_threadScrew = 1.588 * 4.0;
	m_totalMotorStepPerRevolution = m_motorStep* m_motoruStep / 10.0; // the speed is given in 1/10 of mm
	m_waitLmtSwTimer.Stop();
	m_tim.Preset(2000);
	
}

WthMenfis::~WthMenfis()
{

}

void WthMenfis::manger()
{	
	if (!isPowered())
		return;
	
	checkLimitSwitchesStatus();
	if (getLmtSwHighStatus() == lmtSw_error|| getLmtSwLowStatus() == lmtSw_error )
	{
		if(isEnable())
			setEnable(false);
	}else
	{
		if (getLmtSwHighStatus() == lmtSw_reached && getLinearVelocity() < 0)
		{
				if(isEnable())
					setEnable(false);
		}
		if (getLmtSwLowStatus() == lmtSw_reached && getLinearVelocity() > 0)
		{
				if(isEnable())
					setEnable(false);
		}
	}

	if(m_pwm->pulseCountingIsEnded())
	{
		setEnable(false);
	}
	if(m_tim.Match()){
		m_tim.Preset(500);
		double v = getLinearVelocity();
		if (v  >= 0){
			pin_WTH_DIRECTION = WTH_DIRECTION_BACK;
			pin_WTH_DIRECTION  = WTH_DIRECTION_PULL;
		}
		else{
			pin_WTH_DIRECTION  = WTH_DIRECTION_PULL;
			pin_WTH_DIRECTION  = WTH_DIRECTION_BACK;
		}
	}
}

double WthMenfis::setLinearVelocity(double v, bool forceDisable)
{
	bool enable;
	
	if (!m_pwmStep->pulseCountIsEnabled())
	{ 
		WthAbstract::setLinearVelocity(v);
		enable = isEnable();
		setEnable(false, forceDisable);

		if (v  >= 0)
		{
			pin_WTH_DIRECTION  = WTH_DIRECTION_PULL;
		}
		else
		{
			pin_WTH_DIRECTION  = WTH_DIRECTION_BACK;
			v = v * -1.0;
		}
		m_pwm->setPwmFrq((v * m_totalMotorStepPerRevolution) / m_threadScrew);

		if (enable && !forceDisable)
		{	
			setEnable(true);
		}
	}
	return getLinearVelocity();
}

void WthMenfis::setEnable(bool enable, bool stopTimer)
{
	WthAbstract::setEnable(enable && isPowered());
	if (WthAbstract::isEnable())
	{
		if (getLmtSwHighStatus() == lmtSw_reached && getLinearVelocity() < 0 )
		{
				m_pwm->setEnable(false);
				m_pwmStep->setEnable(false);
				WthAbstract::setEnable(false);
		}else if (getLmtSwLowStatus() == lmtSw_reached && getLinearVelocity() > 0 )
		{
				m_pwm->setEnable(false);
				m_pwmStep->setEnable(false);
				WthAbstract::setEnable(false);
		}else
		{
			if(getLinearVelocity() != 0)
			{
				m_pwm->setEnable(true);
				if(WthAbstract::getStep() > 0)
					m_pwmStep->setEnable(true);
				else
					m_pwmStep->setEnable(false);
				
				if (!m_waitLmtSwTimer.getStatus())
				{
					m_waitLmtSwTimer.Preset(30/__ABS(getLinearVelocity())*1000 + 1000);
					
					m_limitSwitchTimerError = false;
				}
			}else
			{
				//m_waitLmtSwTimer.Stop();
				m_pwm->setEnable(false);
				m_pwmStep->setEnable(false);
			}
		}
		
	}else
	{
		m_pwm->setEnable(false);
		m_pwmStep->setEnable(false);
		if (stopTimer)
		{
			m_waitLmtSwTimer.Stop();
		}
	}
}

void WthMenfis::power(bool pwr)
{
	WthAbstract::power(pwr);
	if (pwr)
	{
		pin_WTH_PWR = 1;
	}else
	{
		pin_WTH_PWR = 0;
		setEnable(false);
		m_waitLmtSwTimer.Stop();
	}
}

void WthMenfis::setStep(double step)
{
	unsigned long num;
	bool enable;
	double v;
	enable = isEnable();
	setEnable(false);

	WthAbstract::setStep(step);
	//setLinearVelocity(v, true);
	
	if(step > 0)
	{
		//num = (((double)step / v) * m_pwm->getPwmFrq());
		//num = (unsigned long)((step * m_totalMotorStepPerRevolution) / m_threadScrew);
		v = WthAbstract::getLinearVelocity();
		if (v < 0)
			v = v *(-1.0);
		
		num = (unsigned long)((step*m_pwmStep->getPwmFrq())/(v));
		asm("di");
		m_pwmStep->rstPulse();
		m_pwmStep->setNumPulse(num);
		m_pwmStep->setPulseCountEnable(true);
		asm("ei");
		
		setEnable(enable);
	}else
	{
		m_pwmStep->setPulseCountEnable(false);
		setEnable(enable);
		
	}
}

void WthMenfis::setLmtSwLowStatus(LimitSwitchStatus lmtSw)
{
	WthAbstract::setLmtSwLowStatus(lmtSw);
	
}

void WthMenfis::setLmtSwHighStatus(LimitSwitchStatus lmtSw)
{
	WthAbstract::setLmtSwHighStatus(lmtSw);
}

void WthMenfis::checkLimitSwitchesStatus()
{
	
	if ((pin_LMTSW_LOW == LMTSW_REACHED) && (pin_LMTSW_HIGH == LMTSW_REACHED))
	{
		setLmtSwLowStatus(lmtSw_error);
		setLmtSwHighStatus(lmtSw_error);
	}else
	{
		if (pin_LMTSW_LOW == LMTSW_NOT_REACHED)
		{
			setLmtSwLowStatus(lmtSw_notReached);
		}
		else
		{
			setLmtSwLowStatus(lmtSw_reached);
		}
		
		if (pin_LMTSW_HIGH == LMTSW_NOT_REACHED)
		{
			setLmtSwHighStatus(lmtSw_notReached);
		}
		else
		{
			setLmtSwHighStatus(lmtSw_reached);

		}
	}
	
	
	if (getLmtSwHighStatus() == lmtSw_reached && getLinearVelocity() > 0 && (m_waitLmtSwTimer.Match() || m_limitSwitchTimerError))
	{
			m_limitSwitchTimerError = true;
			setEnable(false);
			setLmtSwHighStatus(lmtSw_error);
	}
	else if (getLmtSwLowStatus() == lmtSw_reached && getLinearVelocity() < 0 && (m_waitLmtSwTimer.Match() || m_limitSwitchTimerError))
	{
			m_limitSwitchTimerError = true;
			setEnable(false);
			setLmtSwLowStatus(lmtSw_error);
	}
	else if (getLmtSwHighStatus() == lmtSw_notReached && getLinearVelocity() > 0 && m_limitSwitchTimerError)
	{
		m_limitSwitchTimerError = false;
	}
	else if (getLmtSwLowStatus() == lmtSw_notReached && getLinearVelocity() < 0 && m_limitSwitchTimerError)
	{
		m_limitSwitchTimerError = false;
	}
	
}






















