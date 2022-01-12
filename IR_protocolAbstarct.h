/**
@file		IR_protocolAbstract.h
@brief		IR remote abstarct protocol
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/

#ifndef __IR_PROTOCOL_ABSTARCT_H__
#define __IR_PROTOCOL_ABSTARCT_H__

#include "global.h"

class IR_protocolAbstract
{
public:
	enum IR_protocolErrors
	{
		IRe_none = 0,
		IRe_wrongStx,
		IRe_wrongEtx,
		IRe_bitError,
		IRe_dataError,
		IRe_timeout,
		IRe_numErrors,
	};
	
	struct IR_data_t
	{
		word customCode;
		byte dataCode;
		IR_protocolAbstract::IR_protocolErrors error;
	};
	
	IR_protocolAbstract(){m_state = IRd_waitingStx; m_previousState = IRd_waitingStx; m_fifoIR_data.clear();};
	~IR_protocolAbstract(){};

	virtual void manager(){};

	virtual bool pop_IR_Data(IR_data_t &data){return false;};
	virtual bool isDataEmpry(){return m_fifoIR_data.empty();};
	
protected:
	enum IR_decondingStates
	{
		IRd_waitingStx = 0,
		IRd_decodingData,
		IRd_waitingEtx,
		IRd_waitAfterError,
		IRd_numStates,
	};

	virtual void setState(IR_decondingStates state)
		{
			if(state != m_state)
			{
				m_previousState = m_state;
				m_state = state;
			}
		}
	
	virtual IR_decondingStates getDecodingStatus(){return m_state;}
	virtual IR_decondingStates getPreviousDecodingStatus(){return m_previousState;}

	CSmallRingBuf <IR_data_t, 20> m_fifoIR_data;
	
private:
	IR_decondingStates m_state;
	IR_decondingStates m_previousState;

};

#endif








