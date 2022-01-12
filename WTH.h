/**
@file		WTH.h
@brief		Classes to manage the puller (WTH)

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#ifndef __WTH_H__
#define __WTH_H__

#include "global.h"
#include "MotorAbstract.h" 
#include "pwmAbstract.h" 
#include "sound.h"


class WthAbstract: public MotorAbstract
{
public:
	enum LimitSwitchStatus
	{
		lmtSw_unknown = 0,
		lmtSw_reached,
		lmtSw_notReached,
		lmtSw_error,
	};
	
	WthAbstract():MotorAbstract(){};
	~WthAbstract(){};

	virtual void manger(){};

	virtual double setLinearVelocity(double v, bool forceDisable = false){ return MotorAbstract::setVelocity(v);}
	virtual double getLinearVelocity(){return MotorAbstract::getVelocity();}
	
	virtual void setEnable(bool enable){MotorAbstract::setEnable(enable);}
	virtual bool isEnable(){return MotorAbstract::isEnable();}

	virtual bool isMoving(){return MotorAbstract::isMoving();}
	
	virtual void setStep(double step){m_step = step;}
	virtual double getStep(){return m_step;}
	
	virtual LimitSwitchStatus getLmtSwLowStatus(){return m_lmtSwLow;}	
	virtual LimitSwitchStatus getLmtSwHighStatus(){return m_lmtSwHigh;}

	virtual void power(bool pwr){MotorAbstract::power(pwr);}
	virtual bool isPowered(){ return MotorAbstract::isPowered();}
	
protected:
	virtual void setLmtSwLowStatus(LimitSwitchStatus lmtSw){m_lmtSwLow = lmtSw;}
	virtual void setLmtSwHighStatus(LimitSwitchStatus lmtSw){m_lmtSwHigh = lmtSw;}
	
private:
	
	LimitSwitchStatus m_lmtSwLow; //closest to the motor's body
	LimitSwitchStatus m_lmtSwHigh;

	double m_step;
};



class WthMenfis: public WthAbstract
{
public:
	WthMenfis(pwmAbstract* pwm, pwmAbstract* pwmStep);
	~WthMenfis();

	virtual void manger();

	virtual double setLinearVelocity(double v, bool forceDisable = false);
	
	virtual void setEnable(bool enable, bool stopTimer = true);

	virtual void setStep(double step);

	virtual void power(bool pwr);
	
protected:
	virtual void setLmtSwLowStatus(LimitSwitchStatus lmtSw);
	virtual void setLmtSwHighStatus(LimitSwitchStatus lmtSw);

	void checkLimitSwitchesStatus();
	
private:
	pwmAbstract* m_pwm;
	pwmAbstract* m_pwmStep;
	
	double m_motorStep;
	double m_motoruStep;
	double m_threadScrew;		// mm
	double m_totalMotorStepPerRevolution;
	DecTimer m_waitLmtSwTimer;
	DecTimer m_tim;
	bool m_limitSwitchTimerError;
};












#endif









