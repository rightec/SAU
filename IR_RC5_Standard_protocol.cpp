/**
@file		IR_RC5_Standard_protocol.h
@brief		Class that manages RC5 standard protocol
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/

#include "IR_RC5_Standard_protocol.h"


RC5_Standard_IR_protocol::RC5_Standard_IR_protocol(pwMeasurementAbstract *_pwMeasure): 
				IR_protocolAbstract()
{
	m_pwMeasure = _pwMeasure;
	m_stxState = stxs_waitLowPulse;
	m_dataDcdState = dcdData_bit00;
	m_timeOut.Stop();
	m_timerWait.Stop();
	
	m_curretData = 0;
	m_lastIRdata.error= IRe_none;
	m_lastIRdata.customCode = 0xFFFF;
	m_lastIRdata.dataCode = 0xFF;
	m_lastStxHeader = 0;
	m_pulseDecodingState = pdec_waiting_1_after_1;
}

RC5_Standard_IR_protocol::~RC5_Standard_IR_protocol()
{

}

void RC5_Standard_IR_protocol::manager()
{
	bool excWhile;
	int whileCycle;
	
	if(m_pwMeasure == NULL)
		return;

	whileCycle = 0;
	excWhile = m_pwMeasure->pulseMeasureAvailableHigh() || m_pwMeasure->pulseMeasureAvailableLow();
	while (excWhile && whileCycle < 100)
	{
		switch (getDecodingStatus())
		{
			case IRd_waitingStx:
				IRd_waitingStx_handler();
				break;
			case IRd_decodingData:
				IRd_decodingData_handler();
				break;
			case IRd_waitAfterError:
				if(m_timerWait.Match())
				{
					m_timerWait.Stop();
					
					m_stxState = stxs_waitLowPulse;
					m_dataDcdState = dcdData_bit00;
					m_lastStxHeader = 0;
					setState(IRd_waitingStx);
					m_pulseDecodingState = pdec_waiting_1_after_1;
					m_pwMeasure->clear();
				}
				break;
		}
	
		if(m_timeOut.Match())
		{
			m_timeOut.Stop();
			m_stxState = stxs_waitLowPulse;
			m_dataDcdState = dcdData_bit00;
			pushError(IRe_timeout);
			setState(IRd_waitingStx);
			m_pulseDecodingState = pdec_waiting_1_after_1;
			/*setcolor(BLACK);
			charset_select("pc6x8");
			gputsxy(70,50, "        ");
			gputsxy(70, 50, "timeout");*/
		}
		
		whileCycle++;
		excWhile = m_pwMeasure->pulseMeasureAvailableHigh() || m_pwMeasure->pulseMeasureAvailableLow();
	}

	/*IR_data_t data;
	if(pop_IR_Data(data))
	{
		char str[50];
		setcolor(BLACK);
		charset_select("pc6x8");
		gputsxy(30, 00, "          ");
		sprintf(str, "cc:%d", data.customCode);
		gputsxy(30, 00, str);


		gputsxy(30, 10, "          ");
		sprintf(str, "dc:%d", data.dataCode);
		gputsxy(30, 10, str);
		
		gputsxy(30, 20, "          ");
		sprintf(str, "err:%d", data.error);
		gputsxy(30, 20, str);

	}*/
}

void RC5_Standard_IR_protocol::IRd_waitingStx_handler()
{
	dword high, low;
	char str[20];
	bool val;
	int whileCycle;
	int decode;
	
	switch(m_stxState)
	{
		case stxs_waitLowPulse:
			if(m_pwMeasure->getPulseMeasurementLow(low))
			{
				if (low > 600 && low < 1200)
				{
					high = 3000;
					val = m_pwMeasure->getPulseMeasurementHigh(high);
					
					whileCycle = 0;
					while (val && high > 3000 && whileCycle < 10)
					{
						val = m_pwMeasure->getPulseMeasurementHigh(high);	// throws away all the long high pulses: syncronizing.....
						whileCycle++;
					}
					
					if (high > 600 && high < 1200 && val)
					{
						m_stxState = stxs_decodeBit01_readLowPulse;
					}else
					{
						m_stxState = stxs_decodeBit01_readFullPulse;
					}
					
					m_lastStxHeader = (0x01 << 2);
					m_pulseDecodingState = pdec_waiting_1_after_1;
					m_timeOut.Preset(200);
						
				}else
				{
					setState(IRd_waitAfterError);
					m_timerWait.Preset(1200);
					pushError(IRe_wrongStx);
					m_timeOut.Stop();

					/*setcolor(BLACK);
					charset_select("pc6x8");
					gputsxy(0, 10, "                  ");
					gputsxy(0, 10, "err: dec stx");*/
				}
			}else if(m_pwMeasure->getPulseMeasurementHigh(high))
			{
					/*charset_select("pc6x8");
					gputsxy(30, 0, "                  ");
					sprintf(str, "high:%d", high);
					gputsxy(30, 0, str);*/
			}
			break;
			
		case stxs_decodeBit01_readFullPulse:
			decode = decodeBit();
			switch (decode)
			{
				case -1:
					setState(IRd_waitAfterError);
					m_timerWait.Preset(1200);
					m_timeOut.Stop();

					/*setcolor(BLACK);
					charset_select("pc6x8");
					gputsxy(0, 10, "                  ");
					gputsxy(0, 10, "err: dec stx full");*/
					break;
				case 2:
					break;
				default:
					m_stxState = stxs_decodeBit02;
					m_lastStxHeader |= decode << 1;
					break;
			}				
			break;
			
		case stxs_decodeBit01_readLowPulse:
			if(m_pwMeasure->getPulseMeasurementLow(low))
			{
				m_lastStxHeader |= 0x01 << 1;
				m_stxState = stxs_decodeBit02;
				if(low > 400 && low < 1000)
				{
					m_pulseDecodingState = pdec_waiting_1_after_1;
				}else if(low > 1000 && low < 2000)
				{
					m_pulseDecodingState = pdec_waiting_0_after_1;
				}else
				{
					setState(IRd_waitAfterError);
					m_timerWait.Preset(1200);
					pushError(IRe_wrongStx);
					m_timeOut.Stop();

					/*setcolor(BLACK);
					charset_select("pc6x8");
					gputsxy(0, 10, "                  ");
					gputsxy(0, 10, "err: dec stx low");*/
				}
				
			}
			break;	
		case stxs_decodeBit02:
			decode = decodeBit();
			switch (decode)
			{
				case -1:
					setState(IRd_waitAfterError);
					m_timerWait.Preset(1200);
					pushError(IRe_wrongStx);
					m_timeOut.Stop();

					/*setcolor(BLACK);
					charset_select("pc6x8");
					gputsxy(0, 10, "                  ");
					gputsxy(0, 10, "err: dec stx2");*/
					break;
				case 2:
					break;
				default:
					m_lastStxHeader |= decode;
					
					m_stxState = stxs_waitLowPulse;
					setState(IRd_decodingData);
					m_dataDcdState = dcdData_bit00;
					m_curretData = 0;
					/*charset_select("pc6x8");
					gputsxy(30, 0, "                  ");
					sprintf(str, "header:%d", m_lastStxHeader);
					gputsxy(30, 0, str);*/
					break;
			}		
			break;
	}
}

void RC5_Standard_IR_protocol::IRd_decodingData_handler()
{
	IR_data_t IRdata;
	char str[30];

	/*setcolor(BLACK);
	charset_select("pc6x8");
	gputsxy(30, 40, "                  ");
	sprintf(str, "dec:%d, %d", m_dataDcdState, m_pulseDecodingState);
	gputsxy(30, 40, str);*/
				
	switch (m_dataDcdState)
	{
	case dcdData_bit00:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit01:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit02:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit03:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit04:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit05:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit06:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit07:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit08:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit09:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit10:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		break;
	case dcdData_bit11:
		m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler();
		if(m_dataDcdState != dcdData_bit00)
		{
			
			m_dataDcdState = dcdData_bit00;
			m_stxState = stxs_waitLowPulse;
			setState(IRd_waitingStx);
			
			m_timeOut.Stop();

			
			IRdata.dataCode =   m_curretData & 0x000FF;
			IRdata.customCode = 0x01;
			IRdata.error = IRe_none;
			
			m_lastIRdata = IRdata;
			pushData(IRdata);
				
			/*setcolor(BLACK);
			charset_select("pc6x8");
			gputsxy(30, 50, "                  ");
			sprintf(str, "data:%d", m_curretData);
			gputsxy(30, 50, str);*/
		}
		break;
	}
}

int RC5_Standard_IR_protocol::decodeBit()
{
	dword high, low;
	switch(m_pulseDecodingState)
	{
		case pdec_waiting_1_after_1:
			if(m_pwMeasure->getPulseMeasurement(m_pw))
			{
				if(m_pw.timeHigh > 600 && m_pw.timeHigh < 1200)
				{
					if(m_pw.timeLow > 600 && m_pw.timeLow < 1200)
					{
						m_pulseDecodingState = pdec_waiting_1_after_1;
						return 1;
					}else if(m_pw.timeLow > 1200 && m_pw.timeLow < 3000)
					{
						m_pulseDecodingState = pdec_waiting_0_after_1;
						return 1;
					}
				}
				return -1;
			}
			return 2;
			break;
			
		case pdec_waiting_1_after_0:
			if(m_pwMeasure->getPulseMeasurementLow(low))
			{
				if(low > 600 && low < 1200)
				{
					m_pulseDecodingState = pdec_waiting_1_after_1;
					return 1;
				}else if(low> 1200 && low < 3000)
				{
					m_pulseDecodingState = pdec_waiting_0_after_1;
					return 1;
				}
				return -1;
			}
			return 2;
			break;
			
		case pdec_waiting_0_after_0:
			if(m_pwMeasure->getPulseMeasurement(m_pw))
			{
				if(m_pw.timeLow > 600 && m_pw.timeLow < 1200)
				{
					if(m_pw.timeHigh> 600 && m_pw.timeHigh < 1200)
					{
						m_pulseDecodingState = pdec_waiting_0_after_0;
						return 0;
					}else if((m_pw.timeHigh > 1200 && m_pw.timeHigh < 3000 ) || (m_dataDcdState == dcdData_bit10))
					{
						m_pulseDecodingState = pdec_waiting_1_after_0;
						return 0;
					}
				}
				return -1;
			}
			return 2;
			break;
			
		case pdec_waiting_0_after_1:
			if(m_pwMeasure->getPulseMeasurementHigh(high))
			{
				if(high > 600 && high < 1200)
				{
					m_pulseDecodingState = pdec_waiting_0_after_0;
					return 0;
				}else if((high > 1200 && high < 3000 ) || (m_dataDcdState == dcdData_bit10))
				{
					m_pulseDecodingState = pdec_waiting_1_after_0;
					return 0;
				}
				return -1;
			}
			return 2;
			break;
	}
	return -1;
}


bool RC5_Standard_IR_protocol::bitError()
{
	setState(IRd_waitAfterError);
	m_timerWait.Preset(1200);
	m_timeOut.Stop();

	return pushError(IRe_bitError);
}

int RC5_Standard_IR_protocol::dcdData_bit_hadler()
{
	int decode;
	char str[30];
	//static int count = 0;
	
	decode = decodeBit();
	
	switch (decode)
	{
		case -1:
			bitError();
			//count = 0;
			return (int)dcdData_bit00;
			break;
		case 2:
			return m_dataDcdState;
			break;
		default:
			m_curretData |= decode << (dcdData_bit11 - m_dataDcdState - 1);

			/*setcolor(BLACK);
			charset_select("pc6x8");
			gputsxy(0 + count *6, 30, "                  ");
			sprintf(str, "%d", decode);
			gputsxy(0 + count  *6, 30, str);
			count++;*/
			
			break;
	}			

	//if(m_dataDcdState == dcdData_bit11)
		//count = 0;
	
	return (int)m_dataDcdState + 1;
}

bool RC5_Standard_IR_protocol::pop_IR_Data(IR_data_t &data)
{
	if (m_fifoIR_data.empty())
		return false;

	m_fifoIR_data.pop_front(data);
	return true;
}

bool RC5_Standard_IR_protocol::pushError(IR_protocolErrors error)
{
	IR_data_t data;	
	if (m_fifoIR_data.full())
		return false;

	data.customCode = 0xFFFF;
	data.dataCode = 0xFF;
	data.error = error;
	m_lastIRdata = data;
	
	m_fifoIR_data.push_back(data);
	return true;
}

bool RC5_Standard_IR_protocol::pushData(IR_data_t data)
{
	if(m_fifoIR_data.full())
		return false;

	m_fifoIR_data.push_back(data);
	return true;
}





































