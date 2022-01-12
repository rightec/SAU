/**
@file		IR_RC5_Standard_protocol.h
@brief		Class that manages RC5 standard protocol
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/

#ifndef __IR_RC5_STADARD_PROTOCOL_H__
#define __IR_RC5_STADARD_PROTOCOL_H__


#include "global.h"
#include "IR_protocolAbstarct.h"
#include "pwmAbstract.h"
#include "swtim.h"
#include "graph.h"


class RC5_Standard_IR_protocol: public IR_protocolAbstract
{
public:
	RC5_Standard_IR_protocol(pwMeasurementAbstract *_pwMeasure);
	~RC5_Standard_IR_protocol();

	virtual void manager();

	virtual bool pop_IR_Data(IR_data_t &data);

protected:
	enum stxDecodingState
	{
		stxs_waitLowPulse = 0,
		stxs_decodeBit01_readFullPulse,
		stxs_decodeBit01_readLowPulse,
		stxs_decodeBit02,
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
		dcdData_numStates,
	};

	enum pulseDecoding
	{
		pdec_waiting_1_after_1 = 0,
		pdec_waiting_1_after_0,
		pdec_waiting_0_after_0,
		pdec_waiting_0_after_1,
	};
	
	bool pushError(IR_protocolErrors error);
	bool pushData(IR_data_t data);
	
	int decodeBit();
	
	bool bitError();

	int dcdData_bit_hadler();
	
private:
	void IRd_waitingStx_handler();
	void IRd_decodingData_handler();
	
	pwMeasurementAbstract* m_pwMeasure;
	pwMeasurementAbstract::PulseWidthMeasurement m_pw;

	stxDecodingState m_stxState;
	dataDecodingStates m_dataDcdState;
	DecTimer m_timeOut;
	DecTimer m_timerWait;
	dword m_curretData;

	IR_data_t m_lastIRdata;
	byte m_lastStxHeader;
	pulseDecoding m_pulseDecodingState;
};




#endif





































