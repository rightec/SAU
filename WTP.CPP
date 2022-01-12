/**
@file		WTP.CPP
@brief		Classes to manage wtp's motor

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#include "wtp.h"

#define pin_WTP_DIRECTION 	PinDIR_M1
#define pin_WTP_PWR			PinEN_M1


WtpMenfis::WtpMenfis(pwmAbstract *__pwm, TMC262_StepperDriver *__driver, Timer_NEC *__timer)
{
	m_pwm = NULL;
	m_driver = NULL;
	m_accRampTimer = NULL;
	m_wtpIsCalibrate = false;
	m_calibrating = false;
	
	if(__pwm != NULL && __driver != NULL)
	{
		m_pwm = __pwm;
		m_driver = __driver;
		m_accRampTimer = __timer;

		m_accRampTimer->setEnable(false);
		
		
		m_pwm->setPwmTonDuty(50);
		m_pwm->setPwmFrq(0.0);
		m_pwm->setEnable(false);

		driverInit();
		power(true);

		m_pulseStep = 0;
		 
		m_accRampTimer->setFrequency(ACCELERATION_FRQ, Timer_NEC::_CCR0);
		
		m_pulse = 0;
		m_targetPulse = 0;
		m_numTimeStep = 0;
		m_pulseDelta = 0;
		m_initialPulse = 0;
		m_v = 0.0;
		m_enableAccTimer = false;

		m_statusTimer.Preset(2000);
		m_currentTimer.Stop();
		m_driverStatusChanged = false;

		readCalibrationValue();
		pin_WTP_DIRECTION = 1;
		
	}
}

void WtpMenfis::driverInit()
{
	//----- DRVCONF ----
	m_driver->setDrvConfSlopeControlHighSide(TMC262_StepperDriver::sch_max);

	m_driver->setDrvConfSlopeControlLowSide(TMC262_StepperDriver::scl_max);

	m_driver->setDrvConfShortToGndProtection(TMC262_StepperDriver::stgndp_enable);

	m_driver->setDrvConfShortToGndDetectionTimer(TMC262_StepperDriver::stgndt_3_2us);

	m_driver->setDrvConfStepDirInput (TMC262_StepperDriver::sdi_enableStepAndDirectionMode);

	m_driver->setDrvConfSenseResistorVoltage (TMC262_StepperDriver::srv_fullScaleIs_165mV);
	
	m_driver->setDrvConfValueToRead (TMC262_StepperDriver::rv_stallGuardLevel);
	
	//--- DRVCTRL ----
	m_driver->setStepInterpolationEnable(true);
	
	m_driver->setDoubleEdgePulsesEnable(false);
	
	m_driver->setMicroStepResolution(TMC262_StepperDriver::uSr_256);
	
	//--- CHOPCONF ---
	m_driver->setChopperBlankTime(TMC262_StepperDriver::bt_16_clk);

	m_driver->setChopperRandomToffTime(TMC262_StepperDriver::rndtf_random);
	
	m_driver->setChopperNormalMode(TMC262_StepperDriver::hdec_16_clk, TMC262_StepperDriver::hlow_3, TMC262_StepperDriver::hsv_hysteresisLow_plush_6);

	m_driver->setChopperDrvEnableAndToff(TMC262_StepperDriver::drv_disable);
	
	//---- SAMRTEN ---
	m_driver->setSmartEnMinimumCurrent(TMC262_StepperDriver::mc_quarter);

	m_driver->setSmartEnCurrentDownStepSpeed(TMC262_StepperDriver::cds_32);

	m_driver->setSmartEnStallGuardHysteresis(TMC262_StepperDriver::sgh_2);

	m_driver->setSmartEnCurrentUpStepWidth(TMC262_StepperDriver::cusw_2);

	m_driver->setSamrtEnStallGuardMinimum(TMC262_StepperDriver::sgm_3);

	//----- SGCSCONF ----
	m_driver->setSGCSConfStallGuardFilterEnable(TMC262_StepperDriver::sgfe_filterMode);

	m_driver->setSGCSConfStallGuardThreasholdValue(0);
		
	m_driver->setSGCSConfCurrentScale(22);	
}



void WtpMenfis::manager()
{
	dword status;
	if(m_driver == NULL)
		return;
	
	if(m_pwm == NULL)
		return;

	m_driver->manager();
	m_pwm->manager();

	if(m_currentTimer.Match())
	{
		m_currentTimer.Stop();
		m_driver->setSGCSConfCurrentScale(22);
		m_driver->write_SGSFCONF();
	}

	if(m_statusTimer.Match())
	{
		status = m_driver->read_DRVSTARTUS(TMC262_StepperDriver::rv_stallGuardAndCurrentLevel);
		m_statusTimer.Preset(500);

		if(m_driverStatus != status)
			m_driverStatusChanged = true;
		
		m_driverStatus = status;
		
		/*char str[20];
		setcolor(BLACK);
		charset_select("pc6x8");
		gputsxy(0 , 0, "                    ");
		sprintf(str, "%08X", m_driverStatus);
		gputsxy(0, 0, str);*/
	}
}

double WtpMenfis::setVelocity(double v, bool forceDisable)
{
	m_calibrating = false;
	if(!m_wtpIsCalibrate)
	{
		m_targetPulse = 0;
		m_v = 0.0;
		return m_v;
	}
	if(v != getVelocity())
	{
		//m_targetPulse = getVelocity()*1080; // target pulse previous velocity multiplied by gain to get ml/min.
		if(v < m_aWtpCalParameters[0].upperBound)
			m_targetPulse = (v*m_aWtpCalParameters[0].gain) - m_aWtpCalParameters[0].offset;
		else if(v < m_aWtpCalParameters[1].upperBound)
			m_targetPulse = (v*m_aWtpCalParameters[1].gain) - m_aWtpCalParameters[1].offset;
		else
			m_targetPulse = (v*m_aWtpCalParameters[2].gain) - m_aWtpCalParameters[2].offset;
		
		WtpAbstract::setVelocity(v, forceDisable);
		accRampInitialParameters();
	
		if(m_enableAccTimer && isEnable())
		{
			m_accRampTimer->setEnable(true);	
		}
	}
	if(isEnable())
	{
		if(v > 0)
		{	
			m_driver->setSGCSConfCurrentScale(31);	
			m_driver->write_SGSFCONF();
			m_currentTimer.Preset(500);
		}
	}

	/*if( m_targetPulse < 5000)
		m_driver->setSGCSConfStallGuardThreasholdValue(18);
	else if (m_targetPulse >= 5000 && m_targetPulse < 10000)
		m_driver->setSGCSConfStallGuardThreasholdValue(8);
	else if (m_targetPulse >= 10000 && m_targetPulse < 20000)
		m_driver->setSGCSConfStallGuardThreasholdValue(5);
	else if (m_targetPulse >= 20000 && m_targetPulse < 35000)
		m_driver->setSGCSConfStallGuardThreasholdValue(3);
	else if (m_targetPulse >= 35000 && m_targetPulse < 50000)
		m_driver->setSGCSConfStallGuardThreasholdValue(2);
	else if (m_targetPulse >= 50000 && m_targetPulse < 75000)
		m_driver->setSGCSConfStallGuardThreasholdValue(2);
	else if (m_targetPulse >= 75000 && m_targetPulse < 100000)
		m_driver->setSGCSConfStallGuardThreasholdValue(1);
	else if (m_targetPulse >= 100000 && m_targetPulse <= 250000)
		m_driver->setSGCSConfStallGuardThreasholdValue(-1);
	else if ( m_targetPulse > 250000)
		m_driver->setSGCSConfStallGuardThreasholdValue(-5);
	m_driver->write_SGSFCONF();
	*/
	
	m_v = v;
	return m_v;
}

void WtpMenfis::setVelocityPulse(dword __pulse)
{
	m_targetPulse = __pulse;
	m_calibrating = true;
	accRampInitialParameters();

	if(m_enableAccTimer && isEnable())
	{
		m_accRampTimer->setEnable(true);	
	}
	
	if(isEnable())
	{
		if(m_targetPulse > 0)
		{	
			m_driver->setSGCSConfCurrentScale(31);	
			m_driver->write_SGSFCONF();
			m_currentTimer.Preset(500);
		}
	}
}
/* ok
*/

void WtpMenfis::setEnable(bool enable)
{
	if(!isCalibrate())
		return;
	
	accRampInitialParameters();
	
	WtpAbstract::setEnable(enable);
	if (enable)
	{
		m_driver->setChopperDrvEnableAndToff(TMC262_StepperDriver::drv_T0ff_02);
		if(m_targetPulse > 0)
		{
			m_driver->setSGCSConfCurrentScale(31);	
			m_driver->write_SGSFCONF();
			m_currentTimer.Preset(500);
		}
	}else
	{
		m_driver->setChopperDrvEnableAndToff(TMC262_StepperDriver::drv_disable);
		m_calibrating = false;
	}
	m_driver->write_CHOPCONF();
	
	
	m_pwm->setEnable(enable);
	
	if(m_enableAccTimer && isEnable())
	{
		m_accRampTimer->setEnable(true);
	}
	
}

void WtpMenfis::power(bool pwr)
{
	WtpAbstract::power(pwr);

	pin_WTP_PWR = pwr ? 1:0;
}

void WtpMenfis::accRampInitialParameters()
{
	char str[50];

	if (isEnable())
		m_initialPulse = m_pulse;
	else
		m_initialPulse = 0;
	
	m_pulse = m_initialPulse;
	
	m_pulseDelta = m_targetPulse - m_initialPulse;

	if (m_pulseDelta < 0)
	{
		m_pulseStep = -20000;
	}else
	{
		m_pulseStep = 10000;
	}
	
	m_targetTimeStep = m_pulseDelta / m_pulseStep;
	m_numTimeStep = 0;
	
	if(m_targetTimeStep == 0)
	{
		m_pulse = m_targetPulse;
	}
	
	m_pwm->setPwmFrq(m_pulse);

	if(m_targetTimeStep != 0)
	{
		m_enableAccTimer = true;
	}else
	{
		m_enableAccTimer = false;
	}

	/*setcolor(BLACK);
	charset_select("pc6x8");
	gputsxy(0 , 0, "                    ");
	sprintf(str, "%d,%d,%d,%d", m_targetTimeStep, m_initialPulse, m_velocity, m_targetPulse);
	gputsxy(0, 0, str);*/
	
}

void WtpMenfis::accelerationRamp()
{	
	if(m_numTimeStep == (m_targetTimeStep -1))
	{
		m_pulse = m_targetPulse;
		m_pwm->setPwmFrq(m_pulse);
		//-- stop timer
		m_accRampTimer->setEnable(false);
		m_enableAccTimer = false;
	}else
	{
		m_pulse += m_pulseStep;
		m_pwm->setPwmFrq(m_pulse);	
		m_numTimeStep++;
	}
}



int WtpMenfis::writeCalibrationValue(wtpCalibration_t *__wtpCalValues)
{
	int i;
	byte crc;
	byte error = 0;
	byte ee_writeContent[EE_SPI_WTP_GAIN_OFFSET_LENGTH];

	crc = 0xFF;
	for(i = 0; i < EE_SPI_WTP_GAIN_OFFSET_LENGTH-1; i++)
	{
		ee_writeContent[i] = *((byte*)((byte*)__wtpCalValues + i));
		crc += ee_writeContent[i];
	}

	ee_writeContent[i++] = crc;

	ee_spi_write(EE_SPI_ADD_WTP_GAIN_OFFSET, ee_writeContent, EE_SPI_WTP_GAIN_OFFSET_LENGTH);

	ee_writeContent[0] = EE_SPI_VALID_CAL_STORED;
	ee_spi_write(EE_SPI_ADD_VALID_WTP_CAL_STORED, ee_writeContent, 1);

	return readCalibrationValue();

}

int WtpMenfis::readCalibrationValue()
{
	int i;
	byte crc;
	byte error = 0;
	byte ee_readContent[EE_SPI_WTP_GAIN_OFFSET_LENGTH];

	m_wtpIsCalibrate = false;
	
	ee_spi_read(EE_SPI_ADD_VALID_WTP_CAL_STORED, ee_readContent, 1);
	if(ee_readContent[0] != EE_SPI_VALID_CAL_STORED)
		return 100;
	
	ee_spi_read(EE_SPI_ADD_WTP_GAIN_OFFSET, ee_readContent, EE_SPI_WTP_GAIN_OFFSET_LENGTH);
	crc = 0xFF;
	for(i = 0; i < EE_SPI_WTP_GAIN_OFFSET_LENGTH-1; i++)
	{
		crc += ee_readContent[i];
	}
	
	if(crc != ee_readContent[i++])
	{
		error++;
	}
	if(error == 0)
	{		
		m_aWtpCalParameters[0] = *((wtpCalibration_t*)(&(ee_readContent[0]))); 
		m_aWtpCalParameters[1] = *((wtpCalibration_t*)(&ee_readContent[sizeof(wtpCalibration_t)])); 
		m_aWtpCalParameters[2] = *((wtpCalibration_t*)(&ee_readContent[sizeof(wtpCalibration_t)*2])); 

		//---- Bounds verify ---
		if(m_aWtpCalParameters[0].upperBound > 15 || m_aWtpCalParameters[0].upperBound < 7)
			error++;
		if(m_aWtpCalParameters[1].upperBound > 65 || m_aWtpCalParameters[1].upperBound < 40)
			error++;
		if(m_aWtpCalParameters[2].upperBound > 90 || m_aWtpCalParameters[2].upperBound < 60)
			error++;

		//---- Gain Verify ----
		if(m_aWtpCalParameters[0].gain > 2000 || m_aWtpCalParameters[0].gain < 500)
			error++;
		if(m_aWtpCalParameters[1].gain > 2000 || m_aWtpCalParameters[1].gain < 500)
			error++;
		if(m_aWtpCalParameters[2].gain > 2000 || m_aWtpCalParameters[2].gain < 500)
			error++;
		
		if(error == 0)
			m_wtpIsCalibrate = true;
	}
	return error;
}












