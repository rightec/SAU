/**
@file		Adc_NEC.h

@brief		NEC V850 JG2 ADC
		
@author		Nicola Molinazzi
@date		05/10/2011
@version	01.00
*/



#ifndef _ADC_NEC_H_
#define _ADC_NEC_H_

#include "global.h"
#include "AdcAbstract.h"
#include "channels.h"


#define _INTERNAL_ADC_REF_mV_	3300

struct adc_nec_t{
	byte	M0;
	byte	M1;
	byte    S;
	byte    M2;
	byte 	PFM;
	byte 	PFT;
};

struct adc_nec_interrupt_t
{
	byte IC;
};
struct adcConv_nec_t
{
	word    CR0;
	word    CR1;
	word    CR2;
	word    CR3;
	word    CR4;
	word    CR5;
	word    CR6;
	word    CR7;
	word    CR8;
	word    CR9;
	word    CR10;
	word    CR11;
};


class Adc_NEC: public AdcAbstract
{
public:
	enum ConversionMode
	{
		cm_continuousSelected = 0,
		cm_continuousScan,
		cm_oneShotSelected,
		cm_oneShotScan,
	};

	enum SpecOfExtTrigger
	{
		extTr_noEdge = 0,
		extTr_fallingEdge,
		extTr_risingEdge,
		extTr_bothEdges,
	};

	enum TriggerModeSpec
	{
		tm_swTrigger = 0,
		tm_extTrigger,
	};

	enum ConversionSpeed
	{
		cs_normal = 0,
		cs_highSpeed,
	};

	enum ConversionTime
	{
		convTime_00 = 0,
		convTime_01,
		convTime_02,
		convTime_03,
		convTime_04,
		convTime_05,
		convTime_06,
		convTime_07,
	};

	enum HwTriggerSource
	{
		ts_ADTRG = 0,
		ts_triggerMode_00, //when INTTP2CC0 interrupt request generated
		ts_triggerMode_01,	//(when INTTP2CC0 interrupt request generated
	};

	enum AnalogInput
	{
		_ANI_00 = 0,
		_ANI_01,
		_ANI_02,
		_ANI_03,
		_ANI_04,
		_ANI_05,
		_ANI_06,
		_ANI_07,
		_ANI_08,
		_ANI_09,
		_ANI_10,
		_ANI_11,
	};
	
	Adc_NEC(volatile adc_nec_t *_reg, volatile adcConv_nec_t *_convReg, volatile adc_nec_interrupt_t* _intReg);
	~Adc_NEC(){};

	void setPower(bool pw);
	bool isPowered(){return AdcAbstract::isPowered();}
	
	void setEnable(bool en);
	bool isEnable(){return AdcAbstract::isEnable();}

	bool setConversionMode(ConversionMode cm, bool forceModify = false);
	ConversionMode getConversionMode(){return m_conversionMode;}

	bool setSpecificationOnExternalTrigger(SpecOfExtTrigger ext, bool forceModify = false);
	SpecOfExtTrigger getSpecificationOnExternalTrigger(){ return m_specOfExtTrigger;}

	bool setTriggerModeSpecification(TriggerModeSpec tm, bool forceModify = false);
	TriggerModeSpec getTriggerModeSpecification(){return m_triggerModeSpec;};

	bool setHwTriggerSource(HwTriggerSource source, bool forceModify = false);
	HwTriggerSource getHwTriggerSource(){return m_triggerSource;}
	
	bool conversionInProgress(){return ( (m_reg->M0 & 0x01) == 0x01 );}

	bool setConversionSpeed(ConversionSpeed cs, bool forceModify = false);
	ConversionSpeed getConversionSpeed(){ return m_conversionSpeed;}

	bool setConversionTime(ConversionTime time, bool forceModify = false);
	ConversionTime getConversionTime(){return m_conversionTime;}

	bool setInputChannel(AnalogInput an, bool forceModify = false);
	AnalogInput getInputChannel(){return m_ANI;}

	void setADIC_interrupt(bool en, byte priority);
	
protected:
	volatile adc_nec_t *m_reg;
	volatile adcConv_nec_t *m_convReg;
	volatile adc_nec_interrupt_t* m_pIntReg;
private:
	ConversionMode m_conversionMode;
	SpecOfExtTrigger m_specOfExtTrigger;
	TriggerModeSpec m_triggerModeSpec;
	HwTriggerSource m_triggerSource;
	
	ConversionSpeed m_conversionSpeed;
	ConversionTime m_conversionTime;

	AnalogInput m_ANI;
};









#endif























