/**
@file		WTP.h
@brief		Classes to manage wtp's motor

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#ifndef __WTP_H__
#define __WTP_H__


#include "global.h"
#include "pwmAbstract.h"
#include "Timer_NEC.h"
#include "TMC262_drv.h"
#include "MotorAbstract.h"
#include "sound.h"
#include "EEPROM_SPI.h"


#define WTP_CALIBRATION_BOUNDS	3

struct wtpCalibration_t
{
	byte upperBound;
	float gain  __attribute__((packed));
	long offset __attribute__((packed));

 	bool operator ==( const wtpCalibration_t &__val)
    {
        for( word i = 0; i < sizeof(wtpCalibration_t); i++)
            if( *((byte*)(((byte*)this)+i)) != *((byte*)((( byte*)&__val)+i)))
                return false;
        return true;
    }

    bool operator !=(const wtpCalibration_t &__val)
    {
        for( word i = 0; i < sizeof(wtpCalibration_t); i++)
            if( *((byte*)(((byte*)this)+i)) != *((byte*)(((byte*)&__val)+i)))
                return true;
        return false;
    }

    wtpCalibration_t()
    {
        for( word i = 0; i < sizeof(wtpCalibration_t); i++)
            *((byte*)(((byte*)this)+i)) = 0;
    }

    wtpCalibration_t(const wtpCalibration_t &__val)
    {
        for( word i = 0; i < sizeof(wtpCalibration_t); i++)
            *((byte*)(((byte*)this)+i)) = *((byte*)(((byte*)&__val)+i));
    }

	wtpCalibration_t& operator= (const wtpCalibration_t &__cal)
	{
        for( word i = 0; i < sizeof(wtpCalibration_t); i++)
            *((byte*)(((byte*)this)+i)) = *((byte*)(((byte*)&__cal)+i));
		return *this;
	}

	void reset()
	{
		for( word i = 0; i < sizeof(wtpCalibration_t); i++)
			*((byte*)(((byte*)this)+i)) = 0;
	}
};
#ifdef __cplusplus
#else
typedef struct wtpCalibration_t 	wtpCalibration_t;
#endif


class WtpAbstract : public MotorAbstract
{
public:
	WtpAbstract():MotorAbstract(){};
	~WtpAbstract(){};	

	virtual void manager(){};

	virtual double setVelocity(double v, bool forceDisable = false){return MotorAbstract::setVelocity(v, forceDisable);}
	virtual double getVelocity(){return MotorAbstract::getVelocity();}
	
	virtual void setEnable(bool enable){MotorAbstract::setEnable(enable);}
	virtual bool isEnable(){return MotorAbstract::isEnable();}

	virtual bool isMoving(){return MotorAbstract::isMoving();}
	
	virtual void power(bool pwr){MotorAbstract::power(pwr);}
	virtual bool isPowered(){ return MotorAbstract::isPowered();}

protected:

private:
	
};

#define ACCELERATION_FRQ  	100.0
		
class WtpMenfis : public WtpAbstract
{
public:
	WtpMenfis(pwmAbstract *__pwm, TMC262_StepperDriver *__driver, Timer_NEC *__timer);
	~WtpMenfis(){};

	void driverInit();

	virtual void manager();

	virtual double setVelocity(double v, bool forceDisable = false);
	
	virtual void setEnable(bool enable);
	
	virtual void power(bool pwr);

	void accelerationRamp();
	
	bool isCalibrate(){return (m_wtpIsCalibrate || m_calibrating);}

	dword getDriverStatus(){return m_driverStatus;}

	bool driverStatusChanged(){bool retVal = m_driverStatusChanged; m_driverStatusChanged = false; return retVal;}

	int writeCalibrationValue(wtpCalibration_t *__wtpCalValues);	
	int readCalibrationValue();

	wtpCalibration_t *getCalibration(){return m_aWtpCalParameters;}

	void setVelocityPulse(dword __pulse);
	long getTargetPulse(){return m_targetPulse;}
	
protected:
	void accRampInitialParameters();
	
private:
	
	pwmAbstract *m_pwm;
	TMC262_StepperDriver *m_driver;

	Timer_NEC *m_accRampTimer;
	double m_v;
	
	long m_pulseStep;
	long m_pulseDelta;
	long m_targetPulse;
	long m_pulse;
	long m_initialPulse;
	long m_numTimeStep;
	long m_targetTimeStep;
	bool m_enableAccTimer;

	DecTimer m_currentTimer;
	DecTimer m_statusTimer;

	dword m_driverStatus;
	bool m_driverStatusChanged;

	wtpCalibration_t m_aWtpCalParameters[WTP_CALIBRATION_BOUNDS];
	bool m_wtpIsCalibrate;
	bool m_calibrating;
};










#endif













