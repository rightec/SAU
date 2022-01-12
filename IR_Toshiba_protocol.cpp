/**
@file		IR_Toshiba_protocol.cpp
@brief		Class that manages toshiba's protocol
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/

#include "IR_Toshiba_protocol.h"

Toshiba_IR_protocol::Toshiba_IR_protocol(pwMeasurementAbstract *_pwMeasure): 
				IR_protocolAbstract()
{
	m_pwMeasure = _pwMeasure;
	m_stxState = stxs_waitLowPulse;
	m_dataDcdState = dcdData_bit00;
	m_timeOut.Stop();

	m_curretData = 0;
	m_lastIRdata.error= IRe_none;
	m_lastIRdata.customCode = 0xFFFF;
	m_lastIRdata.dataCode = 0xFF;
}

Toshiba_IR_protocol::~Toshiba_IR_protocol()
{

}

void Toshiba_IR_protocol::manager()
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
			case IRd_waitingEtx:
				IRd_waitingEtx_handler();
				break;
		}
	
		if(m_timeOut.Match())
		{
			m_timeOut.Stop();
			m_stxState = stxs_waitLowPulse;
			m_dataDcdState = dcdData_bit00;
			pushError(IRe_timeout);
			setState(IRd_waitingStx);
			
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

void Toshiba_IR_protocol::IRd_waitingStx_handler()
{
	dword high, low;
	char str[20];
	//static int t = 0;
	bool val;
	int whileCycle;
	
	switch(m_stxState)
	{
		case stxs_waitLowPulse:
			if(m_pwMeasure->getPulseMeasurementLow(low))
			{
				if (low > 4000 && low < 15000)
				{
					m_stxState = stxs_waitHighPulse;
					m_timeOut.Preset(200);

					high = 7000;
					val = m_pwMeasure->getPulseMeasurementHigh(high);
					
					whileCycle = 0;
					while (val && high > 6000 && whileCycle < 10)
					{
						val = m_pwMeasure->getPulseMeasurementHigh(high);	// throws away all the long high pulses: syncronizing.....
						whileCycle++;
					}
					
					if (high > 1500 && high <= 6000)
					{
						m_stxState = stxs_waitLowPulse;
						setState(IRd_decodingData);
						m_dataDcdState = dcdData_bit00;
						m_curretData = 0;

					}else if (high <= 1500)
					{
						m_stxState = stxs_waitLowPulse;
						pushError(IRe_wrongStx);
						m_timeOut.Stop();

						/*charset_select("pc6x8");
						gputsxy(10, 50, "                ");
						sprintf(str, "stx12 er:%7d:%d", high, t);
						gputsxy(10, 50, str);
						t++;*/
					}
						
				}else
				{
					m_stxState = stxs_waitLowPulse;
					pushError(IRe_wrongStx);
					m_timeOut.Stop();

					/*charset_select("pc6x8");
					gputsxy(10, 40, "               ");
					sprintf(str, "stx1 er:%7d:%d", low,t);
					gputsxy(10, 40, str);
					t++;*/
				}
			}else if(m_pwMeasure->getPulseMeasurementHigh(high))
			{
					/*charset_select("pc6x8");
					gputsxy(30, 0, "                  ");
					sprintf(str, "high:%d", high);
					gputsxy(30, 0, str);*/
			}
			break;
		case stxs_waitHighPulse:
			if (m_pwMeasure->getPulseMeasurementHigh(high))
			{
				if (high > 1500 && high < 6000)
				{
					m_stxState = stxs_waitLowPulse;
					setState(IRd_decodingData);
					m_dataDcdState = dcdData_bit00;
					m_curretData = 0;

					/*charset_select("pc6x8");
					gputsxy(30, 20, "               ");
					gputsxy(30, 20, "stx2 ok");*/
				}else
				{
					m_stxState = stxs_waitLowPulse;
					pushError(IRe_wrongStx);
					m_timeOut.Stop();

					/*charset_select("pc6x8");
					gputsxy(10, 50, "                ");
					sprintf(str, "stx22 er:%7d:%d", high, t);
					gputsxy(10, 50, str);
					t++;*/
				}
			}
			break;
	}
}

void Toshiba_IR_protocol::IRd_decodingData_handler()
{
	if(m_pwMeasure->getPulseMeasurement(m_pw))
	{
		switch (m_dataDcdState)
		{
			case dcdData_bit00:
				if (m_pw.timeHigh > 4000) // verify we are not receiveing etx
				{
					if ((m_pw.timeLow > 400) && (m_pw.timeLow < 800)) // ok it is a etx
					{
						setState(IRd_waitingStx);
						m_stxState = stxs_waitLowPulse;
						m_dataDcdState = dcdData_bit00;
						m_timeOut.Stop();
						
						//if(m_lastIRdata.customCode != 0xFFFF)
						//{
							pushData(m_lastIRdata);
						//}else
						//{
						//	pushError(IRe_dataError);
						//}
						
						/*setcolor(BLACK);
						charset_select("pc6x8");
						gputsxy(50, 30, "       ");
						gputsxy(50, 30, "same cmd");*/
					}else // error...
					{
						bitError();
						
						/*setcolor(BLACK);
						charset_select("pc6x8");
						gputsxy(50, 30, "       ");
						gputsxy(50, 30, "bit er: %d");*/
					}
				}else // we are lookinig for a bit...
				{
					/*charset_select("pc6x8");
					gputsxy(50, 30, "       ");*/
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
					
				}
				break;
			case dcdData_bit01:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit02:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit03:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit04:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit05:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit06:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit07:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit08:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit09:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit10:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit11:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit12:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit13:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit14:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit15:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit16:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit17:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit18:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit19:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit20:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit21:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit22:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit23:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit24:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit25:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit26:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit27:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit28:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit29:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit30:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
				break;
			case dcdData_bit31:
					m_dataDcdState = (dataDecodingStates)dcdData_bit_hadler(m_pw);
					if(m_dataDcdState != dcdData_bit00)
					{
						
						m_dataDcdState = dcdData_bit00;
						setState(IRd_waitingEtx);
						/*setcolor(BLACK);
						charset_select("pc6x8");
						gputsxy(50, 30, "       ");
						gputsxy(50, 30, "bit er: %d");*/
					}
				break;
		}
	}
}

void Toshiba_IR_protocol::IRd_waitingEtx_handler()
{
	char str[50];
	byte* data;
	IR_data_t IRdata;
	
	if(m_pwMeasure->getPulseMeasurement(m_pw))
	{
		if(m_pw.timeLow > 400 && m_pw.timeLow < 700 && m_pw.timeHigh > 4000)
		{
			setState(IRd_waitingStx);
			m_dataDcdState = dcdData_bit00;
			m_timeOut.Stop();

			data = (byte*)(&m_curretData);

			if ( (data[2] == 255 - data[3]) )
			{
				IRdata.dataCode=  data[2];
				IRdata.customCode = ((word)data[1] << 8) + data[0];
				IRdata.error = IRe_none;
				
				m_lastIRdata = IRdata;
				pushData(IRdata);
				
			}else
			{
				pushError(IRe_dataError);
				
				/*setcolor(BLACK);
				charset_select("pc6x8");
				gputsxy(50, 30, "       ");
				gputsxy(50, 30, "dataErr");*/
			}
			
					
					/*setcolor(BLACK);
					charset_select("pc6x8");
					gputsxy(30, 50, "                  ");
					gputsxy(30, 50, "ext ok");*/
		}else
		{
			setState(IRd_waitingStx);
			m_dataDcdState = dcdData_bit00;
			m_timeOut.Stop();

			pushError(IRe_wrongEtx);

			/*setcolor(BLACK);
						charset_select("pc6x8");
					gputsxy(30, 50, "                  ");
					sprintf(str, "etx er:%d", m_pw.timeLow);
					gputsxy(30, 50, str);*/
		}
	}
}


int Toshiba_IR_protocol::decodeBit(pwMeasurementAbstract::PulseWidthMeasurement pw)
{
	if (pw.timeLow > 400 && pw.timeLow < 700)
	{
	
		if(pw.timeHigh > 400 && pw.timeHigh < 700)
			return 0;
		else if (pw.timeHigh > 1200 && pw.timeHigh < 2500)
			return 1;
		
	}
	return -1; 
}


bool Toshiba_IR_protocol::bitError()
{
	setState(IRd_waitingStx);
	m_stxState = stxs_waitLowPulse;
	m_dataDcdState = dcdData_bit00;
	m_timeOut.Stop();

	return pushError(IRe_bitError);
}

int Toshiba_IR_protocol::dcdData_bit_hadler(pwMeasurementAbstract::PulseWidthMeasurement pw)
{
	int decode;
	char str[30];
	
	decode = decodeBit(pw);
	if (decode == -1)
	{
		bitError();
		/*charset_select("pc6x8");
		gputsxy(30, 40, "                  ");
		sprintf(str, "bit err:%d", m_dataDcdState);
		gputsxy(30, 40, str);*/
		
		return (int)dcdData_bit00;
	}else
	{
		m_curretData |= decode << m_dataDcdState;
	}
	
	/*charset_select("pc6x8");
	gputsxy(30, 40, "                  ");
	sprintf(str, "dec bit:%d", m_dataDcdState);
	gputsxy(30, 40, str);*/
	
	return (int)m_dataDcdState + 1;
}

bool Toshiba_IR_protocol::pop_IR_Data(IR_data_t &data)
{
	if (m_fifoIR_data.empty())
		return false;

	m_fifoIR_data.pop_front(data);
	return true;
}

bool Toshiba_IR_protocol::pushError(IR_protocolErrors error)
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

bool Toshiba_IR_protocol::pushData(IR_data_t data)
{
	if(m_fifoIR_data.full())
		return false;

	m_fifoIR_data.push_back(data);
	return true;
}





























