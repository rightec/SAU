/**
@file		IR_Toshiba_protocol.h
@brief		Class that manages toshiba's protocol
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/

#ifndef __IR_TOSHIBA_PROTOCOL_H__
#define __IR_TOSHIBA_PROTOCOL_H__


#include "global.h"
#include "IR_protocolAbstarct.h"
#include "pwmAbstract.h"
#include "swtim.h"


class Toshiba_IR_protocol: public IR_protocolAbstract
{
public:
	Toshiba_IR_protocol(pwMeasurementAbstract *_pwMeasure);
	~Toshiba_IR_protocol();

	virtual void manager();

	virtual bool pop_IR_Data(IR_data_t &data);
	
protected:
	enum stxDecodingState
	{
		stxs_waitLowPulse = 0,
		stxs_waitHighPulse,
		stxs_numStates,
	};

	enum dataDecodingStates
	{
		dcdData_bit00 = 0,
		dcdData_bit01,
		dcdData_bit02,
		dcdData_bit03,
		dcdData_bit04,
		dcdData_bit05,
		dcdData_bit06,
		dcdData_bit07,
		dcdData_bit08,
		dcdData_bit09,
		dcdData_bit10,
		dcdData_bit11,
		dcdData_bit12,
		dcdData_bit13,
		dcdData_bit14,
		dcdData_bit15,
		dcdData_bit16,
		dcdData_bit17,
		dcdData_bit18,
		dcdData_bit19,
		dcdData_bit20,
		dcdData_bit21,
		dcdData_bit22,
		dcdData_bit23,
		dcdData_bit24,
		dcdData_bit25,
		dcdData_bit26,
		dcdData_bit27,
		dcdData_bit28,
		dcdData_bit29,
		dcdData_bit30,
		dcdData_bit31,
		dcdData_numStates,
	};
	
	bool pushError(IR_protocolErrors error);
	bool pushData(IR_data_t data);
	
	int decodeBit(pwMeasurementAbstract::PulseWidthMeasurement pw);
	
	bool bitError();

	int dcdData_bit_hadler(pwMeasurementAbstract::PulseWidthMeasurement pw);
	
private:
	void IRd_waitingStx_handler();
	void IRd_decodingData_handler();
	void IRd_waitingEtx_handler();
	
	pwMeasurementAbstract* m_pwMeasure;
	pwMeasurementAbstract::PulseWidthMeasurement m_pw;

	stxDecodingState m_stxState;
	dataDecodingStates m_dataDcdState;
	DecTimer m_timeOut;

	dword m_curretData;

	IR_data_t m_lastIRdata;
	
};


#endif































