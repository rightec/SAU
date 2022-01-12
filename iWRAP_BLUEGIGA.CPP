/**
@file		iWRAP_BLUEGIGA.CPP
@brief		Class to manage iWRAP bluegiga protocol
					
@author		Nicola
@date		13/11/2011
@version	01.00
*/



#include "iWRAP_BLUEGIGA.h"

iWRAP_BLUEGIGA_Protocol::iWRAP_BLUEGIGA_Protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	stdUartProtocolAbstraction(__pUartDevice)
{

	m_ringBuffer = new RingBuffer(__rxProtoBufSize);
	
	setReceiverState(stdUart_waitStx);
	setLastDataReceivedBuffer(__rxProtoBufSize);
	setMultiMessagesAnalisyPerLoop(true);
	rstBytesReceived();
	
	m_lastCmdSent = cmd_noCmd;
	m_message.clear();

	m_waitSyntaxErrorTimer.Stop();
	m_numBypassByteTransmittedDataAnalyse = 0;
}

iWRAP_BLUEGIGA_Protocol::~iWRAP_BLUEGIGA_Protocol()
{
	delete m_ringBuffer;
}

void iWRAP_BLUEGIGA_Protocol::manager()
{

	int error, msg;
	iWRAP_msg_struct msg_struct;
	iWRAP_msg_struct msg_struct_waiting;
	
	if (m_pUart == NULL)
		return;

	stdUartProtocolAbstraction::manager();

	//---- Remapping stdUartProtocolAbstraction error ----
	error = popError();
	
	while (error != stdUart_NoError)
	{
		switch(error)
		{
			case stdUart_Hw_ReceptionError:
				m_lastHwError |=  Uart_Hw_ReceptionError;

				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: reception error");
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_Hw_ParityError:
				m_lastHwError |=  Uart_Hw_ParityError;
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: parity error");
				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_Hw_FrameError:
				m_lastHwError |=  Uart_Hw_FrameError;
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: frame error");
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_Hw_OverrunError:
				m_lastHwError |=  Uart_Hw_OverrunError;
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: overrun error");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_Hw_RxFifoOverrun:
			case stdUart_Hw_TxFifoOverrun:
				m_lastHwError |=  Uart_Hw_OverrunFpgaFifo;
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: fpga fifo overrun");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
			case stdUart_Hw_NoDevice:
				m_lastHwError |=  Uart_Hw_NoDevice;
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: no device");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_Hw_ErrorUnknown:
				m_lastHwError |= Uart_Hw_ErrorUnknown;
				
				msg_struct.code = msg_HwError;
				strcpy(msg_struct.par, "Hw: unknown");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			//  + da Queue.h -> errori protocollo
			case stdUart_TimeOutErrorR:
				msg_struct.code = msg_timoutRx;
				strcpy(msg_struct.par, "Timeout RX");
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_TimeOutErrorT:
				msg_struct.code = msg_timoutTx;
				strcpy(msg_struct.par, "Timeout TX");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_TransmitBufferOverflowError:
				msg_struct.code = msg_transmitBufferOverflowError;
				strcpy(msg_struct.par, "Transmit buffer overflow");
				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_ReceptionBufferOverflowError:
				msg_struct.code = msg_receptionBufferOverflowError;
				strcpy(msg_struct.par, "Reception buffer overflow");				
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_DataReceivedBufferOverflow:
				msg_struct.code = msg_dataReceivedBufferOverflow;
				strcpy(msg_struct.par, "Data received buffer overflow");			
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;
			case stdUart_ModuleResponseError:
			default:
				msg_struct.code = msg_unknownError;
				strcpy(msg_struct.par, "Unknown Error");			
				msg_struct.len = strlen(msg_struct.par);
				sendMsg(msg_struct);
				break;

		}
		error = popError();
	}

	//------- Checking for packets receiving ----------------
	int whileCycles = 0;
	while(popPacketReceived(msg))
	{	
		popMultiMessage(m_messageReceived, m_messageReceivedLenght);
	
		switch (msg)
		{
			case rsp_OK:
				strcpy((char*)m_messageReceived, str_OK);
				m_messageReceivedLenght = str_READY_lenght + 1;
				break;
			case evt_READY:
				strcpy((char*)m_messageReceived, str_READY);
				m_messageReceivedLenght = str_READY_lenght+ 1;
				break;
			case evt_SYNTAX_ERROR:
				strcpy((char*)m_messageReceived, str_SYNTAX_ERROR);
				m_messageReceivedLenght = str_SYNTAX_ERROR_lenght + 1;
				break;
			
		}

		if(m_messageReceivedLenght > 1)
			m_messageReceived[m_messageReceivedLenght - 1] = '\0';
		else
			m_messageReceived[0] = '\0';


		if (m_waitingMessage.empty()) // wating for sytax error.. the command will be send when an other valid command will be received or when the time _WAIT_SYNTAX_ERROR_TIME_ will be elapsed.
		{
			m_waitSyntaxErrorTimer.Preset(_WAIT_SYNTAX_ERROR_TIME_);
			msg_struct.code = msg;
			msg_struct.len = m_messageReceivedLenght;
			memcpy(msg_struct.par, m_messageReceived, msg_struct.len);
			if(!m_waitingMessage.full())
			{
				m_waitingMessage.push_back(msg_struct);
			}else
			{
				//-- error !!!!
				waitSyntaxErrorSyncroError(1);
			}
		}else
		{
			if (msg == evt_SYNTAX_ERROR) // syntax error received....
			{
				if(!m_waitingMessage.empty())
				{
					m_waitingMessage.pop_front(msg_struct_waiting); // this message is not valid...
				}
					
				m_waitSyntaxErrorTimer.Stop();
				
				msg_struct.code = msg;
				msg_struct.len = m_messageReceivedLenght;
				memcpy(msg_struct.par, m_messageReceived, msg_struct.len);
				sendMsg(msg_struct);
			}else // ...
			{
				m_waitingMessage.pop_front(msg_struct); // this message is valid...
				sendMsg(msg_struct);	
				
				m_waitSyntaxErrorTimer.Preset(_WAIT_SYNTAX_ERROR_TIME_);
				msg_struct.code = msg;
				msg_struct.len = m_messageReceivedLenght;
				memcpy(msg_struct.par, m_messageReceived, msg_struct.len);
				if(!m_waitingMessage.full())
				{
					m_waitingMessage.push_back(msg_struct);
				}else
				{
					//-- error !!!!
					waitSyntaxErrorSyncroError(2);
				}
			}
					
		}
		
		whileCycles++;
		checkSyntaxErrorTimer();
	}
	
	checkSyntaxErrorTimer();
}

bool iWRAP_BLUEGIGA_Protocol::DataAnalyse()
{
	
	bool newdata;
	char dataBuff;
	iWRAP_msg msg;
	byte* data;

						// prima la gestione errori
	stdUartProtocolAbstraction::DataAnalyse();

	if(m_bypassMode) // by pass mode....
	{
		m_numBypassByteTransmittedDataAnalyse = 0;
		while(!m_pBypassUart->TransmissionBufferIsFull() && m_numBypassByteTransmittedDataAnalyse < 300)
		{
			newdata = m_pUart->ReceiveData(dataBuff);
			if(newdata)
			{
				m_pBypassUart->TransmitData((byte)dataBuff);
			}else
			{
				break;
			}

			m_numBypassByteTransmittedDataAnalyse++;
		}
		return true;
	}			
									// succhia dalla uart dati in arrivo 
	while( (newdata = m_pUart->ReceiveData(dataBuff)) == true )
	{
		m_ringBuffer->Insert(dataBuff);
		switch( getReceiverState() )
		{
			case stdUart_waitStx:
				msg = msg_noMsg;
				//----- events ---
				if (m_ringBuffer->Equals(str_CONNECT, str_CONNECT_lenght))
				{
					msg = evt_CONNECT;
				}else if (m_ringBuffer->Equals(str_INQUIRY_PARTIAL, str_INQUIRY_PARTIAL_lenght))
				{
					msg = evt_INQUIRY_PARTIAL;
				}else if (m_ringBuffer->Equals(str_NO_CARRIER, str_NO_CARRIER_lenght))
				{
					msg = evt_NO_CARRIER;
				}else if (m_ringBuffer->Equals(str_READY, str_READY_lenght))
				{
					msg = evt_READY;					
				}else if (m_ringBuffer->Equals(str_NAME, str_NAME_lenght))
				{
					if (m_ringBuffer->Equals(str_SET_BT_NAME, str_SET_BT_NAME_lenght))
					{
						msg = rsp_SET_BT_NAME;
					}
					else
					{
						msg = evt_NAME;
					}
				}else if (m_ringBuffer->Equals(str_NAME_ERROR, str_NAME_ERROR_lenght))
				{
					msg = evt_NAME_ERROR;
				}else if (m_ringBuffer->Equals(str_PAIR, str_PAIR_lenght))
				{
					msg = evt_PAIR;
				}else if (m_ringBuffer->Equals(str_RING, str_RING_lenght))
				{
					msg = evt_RING;
				}else if (m_ringBuffer->Equals(str_SYNTAX_ERROR, str_SYNTAX_ERROR_lenght))
				{
					msg = evt_SYNTAX_ERROR;
				}else if (m_ringBuffer->Equals(str_AUTH, str_AUTH_lenght))
				{
					if (m_ringBuffer->Equals(str_SET_BT_AUTH, str_SET_BT_AUTH_lenght))
					{
						msg = rsp_SET_BT_AUTH;
					}
					else
					{
						msg = evt_AUTH;
					}
				}//----- Responses ---
				else if (m_ringBuffer->Equals(str_CALL, str_CALL_lenght)) 
				{
					msg = rsp_CALL;
				}else if (m_ringBuffer->Equals(str_CLOSE, str_CLOSE_lenght)) 
				{
					msg = rsp_CLOSE;
				}else if (m_ringBuffer->Equals(str_LIST, str_LIST_lenght))
				{
					msg = rsp_LIST;
				}else if (m_ringBuffer->Equals(str_SET_PROFILE, str_SET_PROFILE_lenght))
				{
					msg = rsp_SET_PROFILE;
				}else if (m_ringBuffer->Equals(str_SET_BT_BADDR, str_SET_BT_BADDR_lenght))
				{
					msg = rsp_SET_BT_BADDR;
				}else if (m_ringBuffer->Equals(str_SET_BT_CLASS, str_SET_BT_CLASS_lenght))
				{
					msg = rsp_SET_BT_CLASS;
				}else if (m_ringBuffer->Equals(str_SET_BT_LAP, str_SET_BT_LAP_lenght))
				{
					msg = rsp_SET_BT_LAP;
				}else if (m_ringBuffer->Equals(str_SET_BT_PAIR, str_SET_BT_PAIR_lenght))
				{
					msg = rsp_SET_BT_PAIR;
				}else if (m_ringBuffer->Equals(str_SET_BT_ROLE, str_SET_BT_ROLE_lenght))
				{
					msg = rsp_SET_BT_ROLE;
				}else if (m_ringBuffer->Equals(str_SET_BT_SNIFF, str_SET_BT_SNIFF_lenght))
				{
					msg = rsp_SET_BT_SNIFF;
				}else if (m_ringBuffer->Equals(str_SET_BT_SSP, str_SET_BT_SSP_lenght))
				{
					msg = rsp_SET_BT_SSP;
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_BAUD, str_SET_CONTROL_BAUD_lenght))
				{
					msg = rsp_SET_CONTROL_BAUD;
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_CD, str_SET_CONTROL_CD_lenght))
				{
					msg = rsp_SET_CONTROL_CD;	
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_ECHO, str_SET_CONTROL_ECHO_lenght))
				{
					msg = rsp_CONTROL_ECHO;
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_ESCAPE, str_SET_CONTROL_ESCAPE_lenght))
				{
					msg = rsp_SET_CONTROL_ESCAPE;
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_BIND, str_SET_CONTROL_BIND_lenght))
				{
					msg = rsp_SET_CONTROL_BIND;
				}else if (m_ringBuffer->Equals(str_SET_CONTROL_MSC, str_SET_CONTROL_MSC_lenght))
				{
					msg = rsp_SET_CONTROL_MSC;
				}else if (m_ringBuffer->Equals(str_TEST, str_TEST_lenght))
				{
					msg = rsp_TEST;
				}else if (m_ringBuffer->Equals(str_BER, str_BER_lenght))
				{
					msg = rsp_BER;
				}else if (m_ringBuffer->Equals(str_RSSI, str_RSSI_lenght))
				{
					msg = rsp_RSSI;
				}else if (m_ringBuffer->Equals(str_TXPOWER, str_TXPOWER_lenght))
				{
					msg = rsp_TXPOWER;
				}else if (m_ringBuffer->Equals(str_SDP, str_SDP_lenght)) // also SDP ADD
				{
					msg = rsp_SDP;
				}else if (m_ringBuffer->Equals(str_PING, str_PING_lenght))
				{
					msg = rsp_PING;		
				}else if (m_ringBuffer->Equals(str_OK, str_OK_lenght))
				{
					msg = rsp_OK;
				}else if (m_ringBuffer->Equals("\r\n", 1))
				{
					msg = rsp_UNKNOWN;
					
					setReceivedOpCode((int)msg);
					data = (byte*)(m_ringBuffer->bufferData());
					pushMultiMessages(data , m_ringBuffer->getNumData());

					m_ringBuffer->Clear();
				}

				if(msg != msg_noMsg && msg != rsp_UNKNOWN)
				{
					m_decodingMsg = msg;
					stopTxTimeoutTimer();
					setTimeoutRx(300);
					startRxTimeoutTimer();
					rstBytesReceived();
					m_ringBuffer->Clear();
					setReceiverState(stdUart_receivingData);
				}
				
				break;
			
			case stdUart_receivingData:	
				if (!addDataToReceivingBuffer( (byte)dataBuff, getNumBytesReceived()))
				{
					setReceiverState(stdUart_waitStx);
					stopRxTimeoutTimer();
					return true;
				}
				newByteReceived();
				if (dataBuff == '\n')
				{
					stopRxTimeoutTimer();
					m_ringBuffer->Clear();
					setReceiverState(stdUart_waitStx);

					setReceivedOpCode((int)m_decodingMsg);
					
					if( getMultiMessagesAnalisyPerLoop() )
					{
						if (getNumBytesReceived() > 1)
						{
							data = (byte*)(&bufferLastDataReceived()[1]);
							pushMultiMessages(data , getNumBytesReceived() - 1);
						}else
						{
							pushMultiMessages(bufferLastDataReceived(), getNumBytesReceived());
						}
					}

					m_decodingMsg = msg_noMsg;
					return true;
				}
				break;
		}
	}
	return false;
}


void iWRAP_BLUEGIGA_Protocol::waitSyntaxErrorSyncroError(int i)
{
	iWRAP_msg_struct msg_struct;
	
	m_waitSyntaxErrorTimer.Stop();
	m_waitingMessage.clear();

	msg_struct.code = msg_syntaxErrorSyncroError;
	sprintf(msg_struct.par, "Syntax Error syncro err:%d", i);
	msg_struct.len = strlen(msg_struct.par);
	sendMsg(msg_struct);
		
}

void iWRAP_BLUEGIGA_Protocol::checkSyntaxErrorTimer()
{
	if(m_waitSyntaxErrorTimer.Match())
	{
		iWRAP_msg_struct msg_struct;
		m_waitSyntaxErrorTimer.Stop();
		
		m_waitingMessage.pop_front(msg_struct); // this message is valid...
		sendMsg(msg_struct);
	}
}

/**
push a message in the messages' fifo.

@param i message
@return 0 if the message was pushed, -1 otherwise(full fifo).
@see Msg
*/
int iWRAP_BLUEGIGA_Protocol::sendMsg(iWRAP_msg_struct msg)
{
	if(!m_message.full())
	{
		m_message.push_back(msg);
		return 0;
	}	
	return -1;
}

int iWRAP_BLUEGIGA_Protocol::sendMsg(int i)
{
	iWRAP_msg_struct msg;

	if(!m_message.full())
	{
		msg.code = i;
		msg.len = 0;
		msg.par[0] = '\0';
		m_message.push_back(msg);
		return 0;
	}	
	return -1;
}

/**
@return the message within the fifo. if the fifo is empty return AcqHrMsg_noMsg
@see Msg
*/
bool iWRAP_BLUEGIGA_Protocol::getMsg(iWRAP_msg_struct &msg)
{
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
		return true;
	}
	return false;
}



int iWRAP_BLUEGIGA_Protocol::CALL(const char *_address, const char* _target, conenct_mode _cm, int MTU, int _uart_timeout)
{
	char str[70];
	char str2[20];
	int len;
	
	strcpy(str, str_CALL);
	strcat(str, " ");
	strcat(str, _address);
	strcat(str, " ");
	strcat(str, _target);
	switch (_cm)
	{
		case cm_RFCOMM:
			strcat(str, " RFCOMM");
			break;
		case cm_HFP_AG:
			strcat(str, " HFP");
			break;
 		case cm_HFP:
 			strcat(str, " HFP_AG");
			break;
		case cm_A2DP:
			strcat(str, " A2DP");
			break;
		case cm_HID:
			strcat(str, " HID");
			break;
		case cm_L2CAP:
			strcat(str, " L2CAP");
			break;
	}

	if (MTU >= 21 && MTU <= 1008)
	{
		sprintf(str2, " MTU %d", MTU);
		strcat(str, str2);
	}
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_CALL;

	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::CLOSE(int _link_id, int _uart_timeout)
{
	char str[20];
	char str2[20];
	int len;

	strcpy(str, str_CLOSE);
	
	sprintf(str2, " %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_CLOSE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::INQUIRY(int _timeout, bool _name, inquiryAccessCode_LAP _lap, int _uart_timeout )
{
	char str[70];
	char str2[20];
	
	int len;

	strcpy(str, str_INQUIRY);
	
	sprintf(str2, " %d", _timeout);
	strcat(str, str2);
	
	if (_name)
	{
		strcat(str, " NAME");
	}

	switch(_lap)
	{
		case lap_none:
			break;
		case lap_GIAC:
		case lap_LIAC:
			sprintf(str2, " LAP %x", _lap);
			strcat(str, str2);
			break;
	}
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_INQUIRY;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::IC(int _uart_timeout)
{
	char str[10];
	int len;

	strcpy(str, str_IC);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_IC;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::LIST(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_LIST);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_LIST;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::NAME(const char* _address, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_NAME);

	strcat(str, " ");
	strcat(str, _address);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_NAME;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::PAIR(const char* _address , int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_PAIR);

	strcat(str, " ");
	strcat(str, _address);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_PAIR;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}


int iWRAP_BLUEGIGA_Protocol::RESET(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_RESET);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_RESET;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SELECT(int _link_id, int _uart_timeout)
{
	char str[20];
	char str2[20];
	int len;

	strcpy(str, str_SELECT);

	sprintf(str2, " %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SELECT;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::INFO(int _uart_timeout)
{
	char str[10];
	int len;

	strcpy(str, str_INFO);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_INFO;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::AUTH(const char* _bt_address, int _uart_timeout)
{
	char str[40];
	int len;

	strcpy(str, str_AUTH);

	strcat(str, " ");
	strcat(str, _bt_address);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_PROFILE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_SET);
	
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_PROFILE(profile_name _pn, const char* _name, int _uart_timeout)
{
	char str[70];
	int len;

	strcpy(str, str_SET_PROFILE);

	switch (_pn)
	{
		case pn_HFP:
			strcat(str, " HFP ");
			break;
		case pn_HFP_AG:
			strcat(str, " HFP AG ");
			break;
		case pn_SPP:
			strcat(str, " SPP ");
			break;
		case pn_OPP:
			strcat(str, " OPP ");
			break;
	}
	
	strcat(str, _name);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_PROFILE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_BDADDR(int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_BT_BADDR);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_BADDR;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_NAME(const char* _name, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_BT_NAME);

	strcat(str, " ");
	strcat(str, _name);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_NAME;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_CLASS(const char* _class_of_device, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_BT_CLASS);

	strcat(str, " ");
	strcat(str, _class_of_device);
	
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_CLASS;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_LAP(inquiryAccessCode_LAP _lap, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_LAP);

	if(_lap != lap_none)
	{
		sprintf(str2, " %x" ,_lap);
		strcat(str, str2);
	}
	
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_LAP;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_AUTH(const char* _pin_code, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_BT_AUTH);

	strcat(str, " * ");
	strcat(str, _pin_code);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_AUTH;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_PAIR(const char *_bd_addr, const char* _link_key, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_BT_PAIR);

	strcat(str, " ");
	strcat(str, _bd_addr);
	strcat(str, " ");
	strcat(str, _link_key);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_PAIR;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_PAGEMODE(page_mode _pm, int _page_timeout, page_scan_mode _scan_mode, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_PAGEMODE);

	sprintf(str2," %d %x %d\r\n", _pm, _page_timeout, _scan_mode);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_PAGEMODE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_ROLE(master_slave_policy _ms_policy, link_policy _lk_policy, int _supervision_timeout, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_ROLE);
	
	sprintf(str2," %d %x %x\r\n", _ms_policy, _lk_policy, _supervision_timeout);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_ROLE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_SNIFF(int _max, int _min, int _attempt, int _timeout, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_SNIFF);

	sprintf(str2," %d %d %x %x\r\n", _max, _min, _attempt, _timeout);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_SNIFF;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_SNIFF(int _avg, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_SNIFF);

	sprintf(str2," %d\r\n", _avg);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_SNIFF;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_SSP(spp_capabilities _capability, spp_mitm _mitm, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_BT_SSP);

	sprintf(str2," %d %d\r\n", _capability, _mitm);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_SSP;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_BT_POWER(int _default, int _maximum, bool _reset, int _uart_timeout )
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_PROFILE);

	if (_reset)
	{
		strcat(str, " RESET");
	}
	
	sprintf(str2," %d %d\r\n", _default, _maximum);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_BT_POWER;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}


int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_AUTOCALL(const char* _target, int _timeout, const char *_profile, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_AUTOCALL);

	strcat(str, " ");

	strcat(str, _target);

	sprintf(str2," %d ", _timeout);
	strcat(str, str2);
	strcat(str, _profile);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_AUTOCALL;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_BAUD(baud_rate _br, parity _parity, stop_bits _st_bit, int _uart_timeout)
{
	char str[70];
	char str2[40];
	int baud;
	int len;

	strcpy(str, str_SET_CONTROL_BAUD);

	switch(_br)
	{
		case br_9600:
			baud = 9600;
			break;
		case br_19200:
			baud = 19200;
			break;
		case br_38400:
			baud = 38400;
			break;
		case br_115200:
			baud = 115200;
			break;
		case br_230400: 
			baud = 230400;
			break;
		case br_460800:
			baud = 460800;
			break;
	}
	
	sprintf(str2," %d,8", baud);
	strcat(str, str2);

	switch(_parity)
	{
		case parity_none:
			strcat(str, "N");
			break;
		case parity_even:
			strcat(str, "E");
			break;
		case parity_odd:
			strcat(str, "O");
			break;
	}

	sprintf(str2,"%d\r\n", _st_bit);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_BAUD;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_CD(byte _cd_mask, bool _data_mode, int _uart_timeout)
{
	char str[40];
	char str2[20];
	int len;
	int mode;
	
	strcpy(str, str_SET_CONTROL_CD);

	mode = _data_mode ? 1:0;
	sprintf(str2," %x %d\r\n", _cd_mask, mode);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_CD;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_CONFIG(configuration_values _config, int _uart_timeout)
{
	char str[30];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_CONFIG);

	sprintf(str2," %x\r\n", _config);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_CONFIG;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_ECHO(echo_mask _echo, int _uart_timeout)
{
	char str[30];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_ECHO);

	sprintf(str2," %d\r\n", _echo);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_ECHO;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_ESCAPE(char _esc_char, word _dtr_mask, dtr_mode _mode, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_ESCAPE);

	sprintf(str2," %c %x %d\r\n", _esc_char, _dtr_mask, _mode);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_ESCAPE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_INIT(const char *_command, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SET_CONTROL_INIT);

	strcat(str, " ");

	strcat(str, _command);
	
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_INIT;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_MUX(bool enable, int _uart_timeout)
{
	char str[20];
	char str2[20];
	int mode;
	int len;

	strcpy(str, str_SET_CONTROL_MUX);

	mode = enable ? 1:0;
	sprintf(str2," %d\r\n", mode);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_MUX;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_BIND(byte _priority, word _io_mask, direction _dir, const char *_command, int _uart_timeout)
{
	char str[70];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_BIND);

	sprintf(str2," %d %x ", _priority, _io_mask);
	strcat(str, str2);

	switch (_dir)
	{
		case dir_RISE:
			strcat(str, "RISE ");
			break;
		case dir_FALL:
			strcat(str, "FALL ");
			break;
		case dir_CHANGE:
			strcat(str, "CHANGE ");
			break;
	}

	strcat(str, _command);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_BIND;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_CONTROL_MSC(msc_mode _mode, word _DSR_mask, word _DTR_mask, word _RTS_mask, word _CTS_mask, word _RI_mask, word _DCD_mask, int _uart_timeout)
{
	char str[70];
	char str2[20];
	int len;

	strcpy(str, str_SET_CONTROL_MSC);

	switch(_mode)
	{
		case msc_DTE:
			strcat(str, " DTE ");
			break;
		case msc_nDTE:
			strcat(str, " nDTE ");
			break;
		case msc_DCE:
			strcat(str, " DCE ");
			break;
		case msc_nDCE:
			strcat(str, " nDCE ");
			break;
	}

	sprintf(str2,"%x %x %x %x %x %x\r\n", _DSR_mask, _DTR_mask, _RTS_mask, _CTS_mask, _RI_mask, _DCD_mask);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_CONTROL_MSC;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_ACTIVE(int _link_id, int _uart_timeout)
{
	char str[30];
	
	int len;

	sprintf(str, "SET %d ACTIVE\r\n", _link_id);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_ACTIVE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_MASTER(int _link_id, int _uart_timeout)
{
	char str[30];
	int len;

	sprintf(str, "SET %d MASTER\r\n", _link_id);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_MASTER;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_SLAVE(int _link_id, int _uart_timeout)
{
	char str[30];
	int len;

	sprintf(str, "SET %d SLAVE\r\n", _link_id);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_SLAVE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_PARK(int _link_id, int _max, int _min, int _uart_timeout)
{
	char str[50];
	int len;

	sprintf(str, "SET %d PARK %d %d\r\n", _link_id, _max, _min);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_PARK;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_PARK(int _link_id, int _avg, int _uart_timeout)
{
	char str[50];
	int len;

	sprintf(str, "SET %d PARK %d\r\n", _link_id, _avg);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_PARK;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_SNIFF(int _link_id, int _max, int _min, int _attempt, int _timeout, int _uart_timeout)
{
	char str[50];
	int len;

	sprintf(str, "SET %d SNIFF %d %d %d %d\r\n", _link_id, _max, _min, _attempt, _timeout);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_SNIFF;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_SNIFF(int _link_id, int _avg, int _uart_timeout)
{
	char str[50];
	int len;

	sprintf(str, "SET %d SNIFF %d \r\n", _link_id, _avg);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_SNIFF;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SET_LINK_ID_MSC(int _link_id, const char* _status, int _uart_timeout)
{
	char str[50];
	int len;

	sprintf(str, "SET %d MSC %s\r\n", _link_id, _status);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SET_LINK_ID_MSC;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::TESTMODE(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_TESTMODE);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_TESTMODE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::BER(int _link_id, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_BER);

	sprintf(str2," %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_BER;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::RSSI(int _link_id, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_RSSI);

	sprintf(str2," %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_RSSI;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::TXPOWER(int _link_id, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_TXPOWER);

	sprintf(str2," %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_TXPOWER;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SDP(const char *_bd_addr, const char *_Uuid, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_SDP);

	strcat(str, " ");

	strcat(str, _bd_addr);

	strcat(str, _Uuid);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SDP;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SDP_ADD(const char *_Uuid, const char* _name, int _uart_timeout)
{
	char str[70];
	int len;

	strcpy(str, str_SDP_ADD);

	strcat(str, " ");

	strcat(str, _Uuid);

	strcat(str, _name);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SDP_ADD;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SLEEP(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_SLEEP);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SLEEP;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SCO_ENABLE(int _uart_timeout)
{
	char str[20];
	int len;

	strcpy(str, str_SCO_ENABLE);

	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_SCO_ENABLE;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::SCO_OPEN(int _link_id, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_SCO_OPEN);

	sprintf(str2," %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_SCO_OPEN;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::BOOT(boot_mode _mode, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;
	
	strcpy(str, str_BOOT);

	sprintf(str2," %d\r\n", _mode);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_BOOT;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::ECHO(int _link_id, const char* _string, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	
	strcpy(str, str_ECHO);

	sprintf(str2," %d ", _link_id);
	strcat(str, str2);

	strcat(str, _string);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_ECHO;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::PING(int _link_id, int _uart_timeout)
{
	char str[50];
	char str2[20];
	int len;

	strcpy(str, str_PING);
	
	sprintf(str2," %d\r\n", _link_id);
	strcat(str, str2);
	
	len = strlen(str);

	m_lastCmdSent = cmd_PING;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::TEST(const char* _test_cmd, int _uart_timeout)
{
	char str[50];
	int len;

	strcpy(str, str_TEST);

	strcat(str, " ");
	strcat(str, _test_cmd);
	strcat(str, "\r\n");
	
	len = strlen(str);

	m_lastCmdSent = cmd_TEST;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}


int iWRAP_BLUEGIGA_Protocol::AT(int _uart_timeout)
{
	char str[10];
	int len;

	strcpy(str, "\nAT\r\n");

	len = strlen(str);

	m_lastCmdSent = cmd_AT;
	return SendCommand((unsigned char*)str , len, _uart_timeout);
}

int iWRAP_BLUEGIGA_Protocol::ESCAPE_SEQUENCE(int _uart_timeout)
{
	char str[10];
	int len;
	DecTimer wait;

	wait.Preset(1500);
	while (!wait.Match());
	
	strcpy(str, "+++");

	len = strlen(str);

	m_lastCmdSent = cmd_ESCAPE_SEQUENCE;
	SendCommand((unsigned char*)str , len, _uart_timeout);

	wait.Preset(1100);
	while (!wait.Match());

	return 1;
}





















