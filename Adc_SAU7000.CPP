/**
@file		Adc_SAU7000.cpp

@brief		specific implementation of the adc for the board sau7000
		
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#include "Adc_SAU7000.h"


Adc_NEC_SAU7000::Adc_NEC_SAU7000(volatile adc_nec_t *_reg, volatile adcConv_nec_t *_convReg, volatile adc_nec_interrupt_t* _intReg)
	:Adc_NEC(_reg, _convReg, _intReg)
{
	int i;
	setEnable(false);
	setConversionMode(cm_continuousScan, true);
	setSpecificationOnExternalTrigger(extTr_noEdge, true);
	setTriggerModeSpecification(tm_swTrigger, true);
	setHwTriggerSource(ts_ADTRG, true);
	
	setConversionSpeed(cs_normal, true);
	setConversionTime(convTime_03, true);
	setInputChannel(_ANI_11, true);

	//--- power Voltage channel ---
	m_powerVoltageChan = new VoltageHwChanManager <word, _PWR_VOLTAGE_BUFFER_LENGTH_> (_NUM_VOLTAGE_MEASURE_, _10_BITS_RESOLUTION_, 1);
	m_powerVoltageChan->setVoltageRefernce_mV(_INTERNAL_ADC_REF_mV_);
	for(i = 0; i < _NUM_VOLTAGE_MEASURE_; i++)
	{
		m_powerVoltageChan->setIsZeroable(i, false);
		m_powerVoltageChan->setOffset(i, 0);
		m_powerVoltageChan->setGain(i, (m_powerVoltageChan->getVoltageRefence_mV()/m_powerVoltageChan->getAdcResolution()) * (5.0/3.0));
		m_powerVoltageChan->setEnabled(i, true);
	}	
	//--- power Current channel  ---
	m_powerCurrentChan = new CurrentHwChanManager <word, _PWR_CURRENT_BUFFER_LENGTH_> (1, _10_BITS_RESOLUTION_, 1);
	m_powerCurrentChan->setVoltageRefernce_mV(_INTERNAL_ADC_REF_mV_);
	m_powerCurrentChan->setIsZeroable(0, false);
	m_powerCurrentChan->setOffset(0, 0);
	m_powerCurrentChan->setGain(0, (m_powerCurrentChan->getVoltageRefence_mV()/m_powerCurrentChan->getAdcResolution()) * (20.0/604.0)/0.12);
	m_powerCurrentChan->setEnabled(0, true);

	m_powerVoltageChanAdc = new VoltageHwChanManager <word, 16> (_NUM_VOLTAGE_MEASURE_, _10_BITS_RESOLUTION_, 1);
	for(i = 0; i < _NUM_VOLTAGE_MEASURE_; i++)
	{
		m_powerVoltageChanAdc->setEnabled(i, true);
	}
	m_powerCurrentChanAdc = new CurrentHwChanManager <word, 16> (1, _10_BITS_RESOLUTION_, 1);
	m_powerCurrentChanAdc->setEnabled(0, true);
}


void Adc_NEC_SAU7000::manager()
{
	word adc[100];//[_NUM_VOLTAGE_MEASURE_*8];
	long sum;
	int i, k;
	word average;
	int baseIndex;
	
	if(m_powerVoltageChanAdc->popAdcData((word*)adc, 8))
	{
		for(i = 0; i < _NUM_VOLTAGE_MEASURE_; i++)
		{
			sum = 0;
			baseIndex = (i << 3);
			for(k = 0; k < 8; k++)
			{
				sum += adc[k + baseIndex];
			}
			average = sum >> 3;
			m_powerVoltageChan->pushAdcDataToChan_isr(i, average);
		}
	}

	if(m_powerCurrentChanAdc->popAdcData((word*)adc, 8))
	{
		i = 0;
		sum = 0;
		for(k = 0; k < 8; k++)
		{
			sum += adc[k];
		}
		average = sum >> 3;
		m_powerCurrentChan->pushAdcDataToChan_isr(i, average);
	}
	
}

bool Adc_NEC_SAU7000::getVoltageAdcData(word *data)
{
	return m_powerVoltageChan->popAdcData_isr(data);
}

bool Adc_NEC_SAU7000::getCurrentAdcData(word *data)
{
	return m_powerCurrentChan->popAdcData_isr(data);
}

void Adc_NEC_SAU7000::pushVoltageAdcDataToChan(int chan, word adc)
{
	m_powerVoltageChanAdc->pushAdcDataToChan_isr(chan, adc);
}

void Adc_NEC_SAU7000::pushCurrentAdcDataToChan(int chan, word adc)
{
	m_powerCurrentChanAdc->pushAdcDataToChan_isr(chan, adc);
}




























