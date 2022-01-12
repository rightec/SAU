/**
@file		MotorAbstract.h
@brief		Motor Abstract

@author		Nicola Molinazzi
@date		27/09/2011
@version	01.00
*/


#ifndef __MOTOR_ABSTRACT_H__
#define __MOTOR_ABSTRACT_H__

#include "global.h"

class MotorAbstract
{
	public:
		MotorAbstract(){};
		~MotorAbstract(){};

		virtual void manager(){};

		virtual double  setVelocity(double v, bool forceDisable = false){m_velocity = v; return v;}
		virtual double getVelocity(){return m_velocity;}
		
		virtual void setEnable(bool enable){m_enable = enable;}
		virtual bool isEnable(){return m_enable;}

		virtual bool isMoving(){return (m_enable && (m_velocity != 0) && m_powered);}
		
		virtual void power(bool pwr){m_powered = pwr;}
		virtual bool isPowered(){ return m_powered;}
		
	protected:

	private:
		bool m_enable;
	
		double m_velocity;
		bool m_powered;
};

#endif













































