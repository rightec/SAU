/**
@file		TMC262_drv.CPP
@brief		Class to manage the stepper driver TM262

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/

#include "TMC262_drv.h"

#define SPI_CS_LOW() (PinSPI_M1_CS = 0)
#define SPI_CS_HIGH() (PinSPI_M1_CS = 1)


TMC262_StepperDriver::TMC262_StepperDriver(SPI_Abstract *__spi)
{
	m_spi = NULL;

	SPI_CS_HIGH();
	
	if (__spi != NULL)
	{
		m_spi = __spi;

		m_spi->setBitTransferLenght(SPI_Abstract::btl_8, true);
		m_spi->setCommunicationClock(SPI_Abstract::cc_fxx_32, true);
		m_spi->setCommunicationType(SPI_Abstract::ct_CLK_high_sampleOnRisingEdge, true);
		m_spi->setTransferModeSpecification(SPI_Abstract::tms_SINGLE_transfer, true);
		m_spi->setTransferModeDirection(SPI_Abstract::tmd_MSB_first, true);
		m_spi->setTransmissionEnable(true);
		m_spi->setReceptionEnable(true);	
		m_spi->setPowerEnable(true);

		Init();
	
		setEnable(false);
		
	}
		
}

void TMC262_StepperDriver::Init()
{
	//----- DRVCONF ----
	setDrvConfSlopeControlHighSide(sch_min);

	setDrvConfSlopeControlLowSide(scl_min);

	setDrvConfShortToGndProtection(stgndp_enable);

	setDrvConfShortToGndDetectionTimer(stgndt_1_6us);

	setDrvConfStepDirInput (sdi_enableStepAndDirectionMode);

	setDrvConfSenseResistorVoltage (srv_fullScaleIs_305mV);
	
	setDrvConfValueToRead (rv_stallGuardAndCurrentLevel);
	
	//--- DRVCTRL ----
	setPolarityA(polarity_0);
	setPolarityB(polarity_0);
	setCurrentA(0);
	setCurrentB(0);
	
	setStepInterpolationEnable(false);
	setDoubleEdgePulsesEnable(false);
	setMicroStepResolution(uSr_256);
	
	//--- CHOPCONF ---
	setChopperBlankTime(bt_36_clk);

	setChopperRandomToffTime(rndtf_fixed);
	
		
	setChopperConstantToffMode(0,	0, ccmp_enable);

	setChopperNormalMode(hdec_32_clk, hlow_0, hsv_hysteresisLow_plush_1);

	setChopperDrvEnableAndToff(drv_disable);
	
	//---- SAMRTEN ---
	setSmartEnMinimumCurrent(mc_half);

	setSmartEnCurrentDownStepSpeed(cds_8);

	setSmartEnStallGuardHysteresis(sgh_8);

	setSmartEnCurrentUpStepWidth(cusw_4);

	setSamrtEnStallGuardMinimum(sgm_controlOff);

	//----- SGCSCONF ----
	setSGCSConfStallGuardFilterEnable(sgfe_standardMode);

	setSGCSConfStallGuardThreasholdValue(64);
		
	setSGCSConfCurrentScale(12);

	//--- Read value ---
	m_microStepPosition = 0xFFFF;
	m_stallGuardResult = 0xFFFF;
	m_currentControlScaling = 0xFFFF;
	m_standStillStep = rb_unknown;
	m_openLoopIndicatorMotorA = rb_unknown;
	m_openLoopIndicatorMotorB = rb_unknown;
	m_shortToGndDetectedOnMotorA = rb_unknown;
	m_shortToGndDetectedOnMotorB = rb_unknown;
	m_overTemperaturePreWarning = rb_unknown;
	m_overTemperature = rb_unknown;
	m_stallGuardStatus = rb_unknown;

}
void TMC262_StepperDriver::manager()
{
	int count;
	if(m_spi == NULL)
		return;

	count = 0;
	if(!m_reg_CHOPCONF_updated)
	{
		write_CHOPCONF();
		count++;
	}
	
	if(!m_reg_SGCFCONF_updated)
	{
		write_SGSFCONF();
		count++;
	}
	
	if(!m_reg_DRVCONF_updated)
	{
		write_DRVCONF();
		count++;
	}

	if(!m_reg_DRVCTRL_updated)
	{
		write_DRVCTRL();
		count++;
	}
	
	if(!m_reg_SMARTEN_updated)
	{
		write_SMARTEN();
		count++;
	}

	if(count == 5)
	{
		//goBuzzer(KEY_LIKE);
		count = 0;
	}
}	

void TMC262_StepperDriver::setEnable(bool en)
{
	m_enable = en;
}

dword TMC262_StepperDriver::transmitData(dword reg)
{
	dword retVal;
	byte *data;
	int wait;
	SPI_CS_LOW();

	for (wait = 0; wait < 10; wait++)
	{
		asm("nop");
	}
	
	//reg = reg << 4;
	
	data = (byte*)&reg;

	retVal = (m_spi->getc(data[2])) << 16;
	retVal += (m_spi->getc(data[1])) << 8;
	retVal += (m_spi->getc(data[0]));

	/*char str[30];
	gputsxy(10, 00, "                  ");
	sprintf(str, "%08X", reg);
	gputsxy(10, 00, str);
	gputsxy(10, 10, "                  ");
	sprintf(str, "%02X:%02X:%02X", data[2], data[1], data[0]);
	gputsxy(10, 10, str);*/

	retVal = retVal >> 4;

	for (wait = 0; wait < 10; wait++)
	{
		asm("nop");
	}
	
	SPI_CS_HIGH();

	for (wait = 0; wait < 10; wait++)
	{
		asm("nop");
	}
	
	return retVal;
}

dword TMC262_StepperDriver::decodeDRVSTATUS(dword reg)
{
	switch(getDrvConfValueToRead())
	{
		case rv_microstepPosition:
			m_microStepPosition = (reg & 0x0FFC00) >> 10;
			break;
		case rv_stallGuardLevel:
			m_stallGuardResult = (reg & 0x0FFC00) >> 10;
			break;
		case rv_stallGuardAndCurrentLevel:
			m_stallGuardResult = (reg & 0x0F8000) >> 10;
			m_currentControlScaling = (reg & 0x007C00) >> 10;
			break;
	}

	m_standStillStep = ((reg & 0x000080) == 0x000080) ? rb_true : rb_false;

	m_openLoopIndicatorMotorA = ((reg & 0x000040) == 0x000040) ? rb_true : rb_false;
	m_openLoopIndicatorMotorB = ((reg & 0x000020) == 0x000020) ? rb_true : rb_false;

	m_shortToGndDetectedOnMotorA = ((reg & 0x000010) == 0x000010) ? rb_true : rb_false;
	m_shortToGndDetectedOnMotorB = ((reg & 0x000008) == 0x000008) ? rb_true : rb_false;

	m_overTemperaturePreWarning = ((reg & 0x000004) == 0x000004) ? rb_true : rb_false;
	m_overTemperature = ((reg & 0x000002) == 0x000002) ? rb_true : rb_false;
	
	m_stallGuardStatus = ((reg & 0x000001) == 0x000001) ? rb_true : rb_false;
	
	return reg;
}

dword TMC262_StepperDriver::write_DRVCONF()
{
 	dword reg;
	dword retVal;
	
	reg = (0b1110) << 16;
	reg += m_drvConfSplopeControlHighSide << 14;
	reg += m_drvConfSlopeControlLowSide << 12;
	reg += m_drvConfShortToGndProtection << 10;
	reg += m_drvConfShortToGndDetectionTimer << 8;
	reg += m_drvConfStepDirInput << 7;
	reg += m_drvConfSenseResistorVoltage << 6;
	reg += m_drvConfValueToRead << 4;

	retVal = transmitData(reg);
	decodeDRVSTATUS(retVal);

	m_reg_DRVCONF_updated = true;
		
	return retVal;
}

dword TMC262_StepperDriver::write_DRVCTRL()
{
	dword reg = -1;
	dword retVal;
	
	if (getDrvConfStepDirInput() == sdi_enableStepAndDirectionMode)
	{
		reg = 0;
		reg += (m_stepInterpolationEnable ? 1:0 ) << 9;
		reg += (m_doubleEdgeStepPulsesEnable ? 1:0) << 8;
		reg += m_uStepResolution;
		
		retVal = transmitData(reg);
		decodeDRVSTATUS(retVal);

		m_reg_DRVCTRL_updated = true;

		return retVal;

	}else if (getDrvConfStepDirInput() == sdi_enableSpiMode)
	{
		reg = 0;
		reg += (m_polarityA ? 1:0) << 17;
		reg += m_currentA << 10;
		reg += (m_polarityB ? 1:0) << 8;
		reg += m_currentB;

		retVal = transmitData(reg);
		decodeDRVSTATUS(retVal);

		m_reg_DRVCTRL_updated = true;
				
		return retVal;

	}
	
	return reg;
}

dword TMC262_StepperDriver::write_CHOPCONF()
{
	dword reg = -1;
	dword retVal;
	
	if(getChopperMode() == cm_standartMode)
	{
		reg = 0b1000 << 16;
		reg += m_chopConfBlankTime << 15;
		reg += m_chopConfChopperMode << 14;
		reg += m_chopConfRandomToffTime << 13;
		
		reg += m_chopConfHystDecInt << 11;
		reg += m_chopConfLowHystInt << 7;
		reg += m_chopConfHystStartVal << 4;
		
		reg += m_chopConfDrvEnableAndToff;

		retVal = transmitData(reg);
		decodeDRVSTATUS(retVal);

		m_reg_CHOPCONF_updated = true;
		
		return retVal;

	}else if(getChopperMode() == cm_ConstToff)
	{
		
		reg = 0b1000 << 16;
		reg += m_chopConfBlankTime << 15;
		reg += m_chopConfChopperMode << 14;
		reg += m_chopConfRandomToffTime << 13;
		
		reg += m_chopConfCurrentComparator << 12;
		reg += (((m_chopConfFastDecTimeSetting & 0x08 ) == 0x08) ? 1:0) << 11;
		reg += m_chopConfSineWaveOffset << 7;
		reg += (m_chopConfFastDecTimeSetting & 0x07)<< 4;
		
		reg += m_chopConfDrvEnableAndToff;

		retVal = transmitData(reg);
		decodeDRVSTATUS(retVal);

		m_reg_CHOPCONF_updated = true;
		
		return retVal;
		
	}

	return reg;
}

dword TMC262_StepperDriver::write_SMARTEN()
{
	dword reg;
	dword retVal;
	
	reg = 0b1010 << 16;
	reg += m_smartEnMiminumCurrent << 15;
	reg += m_smartEnCurrentDownStepSpeed << 13;
	reg += m_smartEnStallGuardHysteresis << 8;
	reg += m_smartEnCurrentUpStepWidth << 5;
	reg += m_smartEnStallGuardMinimum;

	m_reg_SMARTEN_updated = true;

	retVal = transmitData(reg);
	decodeDRVSTATUS(retVal);
		
	return retVal;
}

dword TMC262_StepperDriver::write_SGSFCONF()
{
	dword reg;
	dword retVal;
	dword Thresh = m_sgcsConfStallThresholdVal << 8;
	Thresh &= 0x0000EF00;
		
	reg = 0b1100 << 16;
	reg |= m_sgcsConfStallGuardFilterEn << 16;
	reg |= Thresh;
	reg |= m_sgcsConfCurrentScale;
	/*
	reg = 0b1100 << 16;
	reg += m_sgcsConfStallGuardFilterEn << 16;
	reg += m_sgcsConfStallThresholdVal << 8;
	reg += m_sgcsConfCurrentScale;
	*/
	retVal = transmitData(reg);
	decodeDRVSTATUS(retVal);
	
	m_reg_SGCFCONF_updated = true;	

	return retVal;
}

dword TMC262_StepperDriver::read_DRVSTARTUS(DRVCONF_ValueToRead val)
{
	dword reg;
	
	setDrvConfValueToRead(val);

	write_DRVCONF();
	
	return write_DRVCONF();
}






















