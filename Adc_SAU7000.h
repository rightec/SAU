/**
@file		Adc_SAU7000.h

@brief		specific implementation of the adc for the board sau7000
		
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#ifndef _ADC_SAU7000_H_
#define _ADC_SAU7000_H_


#include "Adc_NEC.h"



class Adc_NEC_SAU7000: public Adc_NEC
{
	public:
		Adc_NEC_SAU7000(volatile adc_nec_t *_reg, volatile adcConv_nec_t *_convReg, volatile adc_nec_interrupt_t* _intReg);
		~Adc_NEC_SAU7000(){};

		void manager();
		
		bool getVoltageAdcData(word *data);
		bool getCurrentAdcData(word *data);

		void pushVoltageAdcDataToChan(int chan, word adc);
		void pushCurrentAdcDataToChan(int chan, word adc);
	protected:

	private:
		VoltageHwChanManager<word, _PWR_VOLTAGE_BUFFER_LENGTH_> *m_powerVoltageChan;
		CurrentHwChanManager <word, _PWR_CURRENT_BUFFER_LENGTH_> *m_powerCurrentChan;

		VoltageHwChanManager<word, 16> *m_powerVoltageChanAdc;
		CurrentHwChanManager <word, 16> *m_powerCurrentChanAdc;
};















#endif


