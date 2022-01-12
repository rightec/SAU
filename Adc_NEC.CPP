/**
@file		Adc_NEC.cpp

@brief		NEC V850 JG2 ADC
		
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/



#include "Adc_NEC.h"


Adc_NEC::Adc_NEC(volatile adc_nec_t *_reg, volatile adcConv_nec_t *_convReg, volatile adc_nec_interrupt_t* _intReg): AdcAbstract()
{
	if((_reg != NULL) && (_convReg != NULL) && (_intReg != NULL))
	{
		m_reg = _reg;
		m_convReg = _convReg;
		m_pIntReg = _intReg;
		
		setEnable(false);
		setConversionMode(cm_continuousScan, true);
		setSpecificationOnExternalTrigger(extTr_noEdge, true);
		setTriggerModeSpecification(tm_swTrigger, true);
		setHwTriggerSource(ts_ADTRG, true);
		
		setConversionSpeed(cs_normal, true);
		setConversionTime(convTime_05, true);
		setInputChannel(_ANI_00, true);
	}
}


void Adc_NEC::setPower(bool pw)
{
	AdcAbstract::setPower(pw);
	setEnable(pw);
}

void Adc_NEC::setEnable(bool en)
{
	AdcAbstract::setEnable(en);
	
	if (en)
		m_reg->M0 |= 0x80;
	else
		m_reg->M0 &= ~0x80;	
}

bool Adc_NEC::setConversionMode(ConversionMode cm, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (cm != m_conversionMode || forceModify)
	{
		m_conversionMode = cm;
		m_reg->M0 &= ~0x30;
		switch(cm)
		{
			case cm_continuousSelected:
				break;
			case cm_continuousScan:
				m_reg->M0 |= 0x10;
				break;
			case cm_oneShotSelected:
				m_reg->M0 |= 0x20;
				break;
			case cm_oneShotScan:
				m_reg->M0 |= 0x30;
				break;
		}
		return true;
	}
	return false;
}

bool Adc_NEC::setSpecificationOnExternalTrigger(SpecOfExtTrigger ext, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (ext != m_specOfExtTrigger || forceModify)
	{
		m_specOfExtTrigger = ext;
		m_reg->M0 &= ~0x0C;
		switch(ext)
		{
			case extTr_noEdge:
				break;
			case extTr_fallingEdge:
				m_reg->M0 |= 0x04;
				break;
			case extTr_risingEdge:
				m_reg->M0 |= 0x08;
				break;
			case extTr_bothEdges:
				m_reg->M0 |= 0x0C;
				break;
		}
		return true;
	}
	return false;
}


bool Adc_NEC::setTriggerModeSpecification(TriggerModeSpec tm, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (tm != m_triggerModeSpec || forceModify)
	{
		m_triggerModeSpec = tm;
		m_reg->M0 &= ~0x02;
		switch(tm)
		{
			case tm_swTrigger:
				break;
			case tm_extTrigger:
				m_reg->M0 |= 0x02;
				break;
		}
		return true;
	}
	return false;
}

bool Adc_NEC::setHwTriggerSource(HwTriggerSource source, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (source != m_triggerSource|| forceModify)
	{
		m_triggerSource = source;
		m_reg->M2 &= ~0x03;
		switch(source)
		{
			case ts_ADTRG:
				break;
			case ts_triggerMode_00:
				m_reg->M2 |= 0x01;
				break;
			case ts_triggerMode_01:
				m_reg->M2 |= 0x02;
		}
		return true;
	}
	return false;
}

bool Adc_NEC::setConversionSpeed(ConversionSpeed cs, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (cs != m_conversionSpeed || forceModify)
	{
		m_conversionSpeed = cs;
		m_reg->M1 &= ~0x80;
		switch(cs)
		{
			case cs_normal:
				break;
			case cs_highSpeed:
				m_reg->M1 |= 0x08;
				break;
		}
		return true;
	}
	return false;
}

bool Adc_NEC::setConversionTime(ConversionTime time, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (time != m_conversionTime|| forceModify)
	{
		m_conversionTime = time;
		m_reg->M1 &= ~0x07;
		switch(time)
		{
			case convTime_00:
			case convTime_01:
				m_reg->M1 |= 0x01;
				break;
			case convTime_02:
				m_reg->M1 |= 0x02;
				break;
			case convTime_03:
				m_reg->M1 |= 0x03;
				break;
			case convTime_04:
				m_reg->M1 |= 0x04;
				break;
			case convTime_05:
				m_reg->M1 |= 0x05;
				break;
			case convTime_06:
				m_reg->M1 |= 0x06;
				break;
			case convTime_07:
				m_reg->M1 |= 0x07;
				break;
				
		}
		return true;
	}
	return false;

}


bool Adc_NEC::setInputChannel(AnalogInput an, bool forceModify)
{
	if(isEnable())
		return false;
	
	if (an != m_ANI || forceModify)
	{
		m_ANI = an;
		m_reg->S &= ~0x0F;
		
		switch(an)
		{
			case _ANI_00:
				m_reg->S |= 0x00;
				break;
			case _ANI_01:
				m_reg->S |= 0x01;
				break;
			case _ANI_02:
				m_reg->S |= 0x02;
				break;
			case _ANI_03:
				m_reg->S |= 0x03;
				break;
			case _ANI_04:
				m_reg->S |= 0x04;
				break;
			case _ANI_05:
				m_reg->S |= 0x05;
				break;
			case _ANI_06:
				m_reg->S |= 0x06;
				break;
			case _ANI_07:
				m_reg->S |= 0x07;
				break;
			case _ANI_08:
				m_reg->S |= 0x08;
				break;
			case _ANI_09:
				m_reg->S |= 0x09;
				break;
			case _ANI_10:
				m_reg->S |= 0x0A;
				break;
			case _ANI_11:
				m_reg->S |= 0x0B;
				break;
			
		}
		return true;
	}
	return false;
}

void Adc_NEC::setADIC_interrupt(bool en, byte priority)
{
	if(priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->IC |= priority;
		m_pIntReg->IC &= ~0x40;
		
	}else
	{
		m_pIntReg->IC |= 0x40;
	}
		
}

















