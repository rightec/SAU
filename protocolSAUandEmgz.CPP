/**
@file		protocolSauAndEmgz.cpp
@brief		Class to manage the serial communication between the board SAU7000 and SAU7003.

@author		Nicola
@date		18/01/2011
@version	01.1
*/

#include "protocolSauAndEmgz.h"

//---------------------------------------------------------------------------------------
//------------------------------ sauAndEmgzProtocol -------------------------------------
//---------------------------------------------------------------------------------------
/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
SauAndEmgzProtocol::SauAndEmgzProtocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize) :
	stdUartProtocolAbstraction(__pUartDevice)
{
	int i;
	
	__pUartDevice->SetBaudRate(UartDeviceAbstract::BR230400, true);
	stdUartProtocolAbstraction::setReceiverState(stdUart_waitStx);
	stdUartProtocolAbstraction::setLastDataReceivedBuffer(__rxProtoBufSize);
	setMultiMessagesAnalisyPerLoop(true);
	rstBytesReceived();
	

	m_numImpPacketsReceived = 0;
	m_numEmgPacketsReceived = 0;
		
	m_lastHwError = Uart_Hw_NoError;

	m_testTimer.Preset(500);

	for(i = 0; i < 30; i++)
	{
		m_appRevision[i] = '\0';
		m_appDate[i] = '\0';
		m_hwVersion[i] = '\0';
	}

	for(i = 0; i < _NUM_VOLTAGE_MEASURE_EMGZ_; i++)
	{
		m_voltageMeasure[i] = 0;
	}

	for(i = 0; i <  _NUM_HR_EMG_SAMPLES_PER_PACKET_; i++)
	{
		m_emgSampleDummy[i] = 0xFFFF;
	}
	
	m_currentMeasure = 0;

	m_emgzStatus = 5;
	m_emgzNumErrorOccurred = 0;
	m_overcurrent = 0;
	m_readElectrodeImpedanceActive = false;

	m_sampleContactImpEnable = 0;

	m_packetLostImp = 0;
	m_packetLostEmg = 0;
	
	__pUartDevice->TurnOn();

	m_opCodeDataAnalyse = 0;
	m_lastRemoteIDDataAnalyse = 0;
	m_numBypassByteTransmittedDataAnalyse = 0;
	m_pFClearAllBuffers = NULL;
}

/**
Class destructor
*/
SauAndEmgzProtocol::~SauAndEmgzProtocol()
{

}

/**
protocol decoding's function
*/
bool SauAndEmgzProtocol::DataAnalyse()
{
bool newdata;
char dataBuff;

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

		//newByteReceived();			// un nuovo byte arrivato
		
		switch( getReceiverState() )
		{
			case stdUart_waitStx:
				if (dataBuff == __STX_EMGZ)
				{
					stopTxTimeoutTimer();
					setTimeoutRx(100);
					startRxTimeoutTimer();
					setReceiverState(stdUart_waitOpCode1);
				}else
				{
					if(getLastError() != stdUart_stxNotReceived)
					{
						setError((int)stdUart_stxNotReceived);
					}
					return false;
				}
				break;
				
			case stdUart_waitDeviceID:
				
				break;
				
			case stdUart_waitNumPacketTransmit:
				
				break;
				
			case stdUart_waitOpCode1:
					if ( checkOpCode(dataBuff) )
					{
						m_opCodeDataAnalyse = (int)dataBuff;
						setReceiverState(stdUart_waitDataLength);	
					}else
					{			
						setError(stdUart_UnknowOpcode);
						stopRxTimeoutTimer();
						setReceiverState(stdUart_waitStx);
						return false;
					}
				break;
			case stdUart_waitOpCode2:
				
				break;
				
			case stdUart_waitDataLength:
				setPacketlength((int)dataBuff);
				rstBytesReceived();
				
				if(dataBuff != 0)
				{
					setReceiverState(stdUart_receivingData);	
				}else
				{
					setReceiverState(stdUart_waitChecksum);	
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
				if (getNumBytesReceived() >= getPacketLength())
				{
					setReceiverState(stdUart_waitChecksum);	
				}
				
				break;
				
			case stdUart_waitChecksum:
				stopRxTimeoutTimer();
				setLastRemoteNodeID(m_lastRemoteIDDataAnalyse);	
				
				//protocol->setPacketsTransmitted(numPacketTransmitted);
				setNumDataReceived(getPacketLength());
				
				if( checksum((byte)dataBuff, (byte)m_opCodeDataAnalyse) )
				{
					setReceivedOpCode(m_opCodeDataAnalyse);
					newPacketReceived();
					
					if( getMultiMessagesAnalisyPerLoop() )
					{
						pushMultiMessages(bufferLastDataReceived(), getNumDataReceived() );
					}
				}
				else
				{
					setError(stdUart_ChecksumError);
				}
				
				setReceiverState(stdUart_waitStx);
				return true;
				break;
				
			case stdUart_waitEtx:
				break;
			default:
				
				break;
		}

		
	}
	
	return false;
}

/**
This funtion has to be called in the mail loop cycle becuase it manage all the class proprieties.
*/
void SauAndEmgzProtocol::manager()
{
	int error, opCode;
	byte packet[20];
	int i;

	
	stdUartProtocolAbstraction::manager();
	/*if(m_testTimer.Match())
	{
		m_testTimer.Preset(500);
		for(i = 0; i < 20; i++)
			packet[i] = 0xAA;

		stdUartProtocolAbstraction::SendCommand(packet, 20, 0);
		stdUartProtocolAbstraction::SendCommand(packet, 20, 0);
	} */
	//---- Remapping stdUartProtocolAbstraction error ----
	
	error = popError();
	while (error != stdUart_NoError)
	{
		switch(error)
		{
			case stdUartProtocolAbstraction::stdUart_Hw_ReceptionError:
				m_lastHwError |=  Uart_Hw_ReceptionError;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_ParityError:
				m_lastHwError |=  Uart_Hw_ParityError;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_FrameError:
				m_lastHwError |=  Uart_Hw_FrameError;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_OverrunError:
				m_lastHwError |=  Uart_Hw_OverrunError;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_RxFifoOverrun:
			case stdUartProtocolAbstraction::stdUart_Hw_TxFifoOverrun:
				m_lastHwError |=  Uart_Hw_OverrunFpgaFifo;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_NoDevice:
				m_lastHwError |=  Uart_Hw_NoDevice;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_ErrorUnknown:
				m_lastHwError |= Uart_Hw_ErrorUnknown;
				sendMsg(Msg_HwError);
				break;
			case stdUartProtocolAbstraction::stdUart_Hw_OverflowRxBuff:
				m_lastHwError |= Uart_Hw_OverflowRxBuff;
				sendMsg(Msg_HwError);
				break;
			//  + da Queue.h -> errori protocollo
			case stdUartProtocolAbstraction::stdUart_UnknowOpcode:
				sendMsg(Msg_opcodeUnknown);
				break;
			case stdUartProtocolAbstraction::stdUart_stxNotReceived:
				sendMsg(Msg_wrongStx);
				break;
			case stdUartProtocolAbstraction::stdUart_TimeOutErrorR:
				sendMsg(Msg_timoutRx);
				break;
			case stdUartProtocolAbstraction::stdUart_TimeOutErrorT:
				m_emgzStatus = EMGZstatus_unknown;
				sendMsg(Msg_timoutTx);
				break;
			case stdUartProtocolAbstraction::stdUart_ChecksumError:
				sendMsg(Msg_checkSumError);
				break;
			case stdUartProtocolAbstraction::stdUart_TransmitBufferOverflowError:
				sendMsg(Msg_transmitBufferOverflowError);
				break;
			case stdUartProtocolAbstraction::stdUart_ReceptionBufferOverflowError:
				sendMsg(Msg_receptionBufferOverflowError);
				break;
			case stdUartProtocolAbstraction::stdUart_DataReceivedBufferOverflow:
				sendMsg(Msg_dataReceivedBufferOverflow);
				break;
			//case stdUartProtocolAbstraction::stdUart_ModuleResponseError:
			defaluts:
				sendMsg(Msg_unknownError);	
				break;

		}
		error = popError();
	}
	//------- Checking for packets receiving ----------------
	while(popPacketReceived(opCode))
	{
		popMultiMessage(m_messageReceived, m_messageReceivedLenght);
		switch (opCode)
		{
			case INFO:
				_infoCmdDecode();
				break;
            case CAL_EMG:
				_calEmgCmdDecode();
				break;
            case CAL_IMP:
				_calImpCmdDecode();
				break;
			case ACKNOWLEDGE:
				_acknowledgeCmdDecode();
				break;
            case STATUS:
				_statusCmdDecode();
				break;
            case ACQUISITION_STARTED:
				_acquisitionStartedCmdDecode();
				break;
            case ACQUISITION_STOPPED:
				_acquisitionStoppedCmdDecode();
				break;
			case ERRORS:
				_errorsCmdDecode();
				break;
			case IMP_SAMPLES:
				_impSampleDecode();
				break;
			case EMG_SAMPLES:
				_emgSampleDecode();
				break;
			case JUMPED_TO_LOADER:
				_jumpToLoaderCmdDecode();
				break;
		}
	}
}

/**
Decodes the command "GET_INFO"
@return always 1
*/
int SauAndEmgzProtocol::_infoCmdDecode()
{

	int i, k;
	char c;
	i = 0;
	k = 0;
	c = m_messageReceived[i];
	while (c != '\t' && k < 29)
	{
		m_appRevision[k] = c;
		i++;
		k++;
		c = m_messageReceived[i];
	}
	m_appRevision[k] = '\0';

	i++;
	k = 0;
	c = m_messageReceived[i];
	while (c != '\t' && k < 29)
	{
		m_appDate[k] = c;
		i++;
		k++;
		c = m_messageReceived[i];
	}
	m_appDate[k] = '\0';

	i++;
	k = 0;
	c = m_messageReceived[i];
	while (c != '\t' && k < 29)
	{
		m_hwVersion[k] = c;
		i++;
		k++;
		c = m_messageReceived[i];
	}
	m_hwVersion[k] = '\0';
	
	sendMsg(Msg_infoReceived);
	return 1;
}

/**
Not used
*/
int SauAndEmgzProtocol::_calEmgCmdDecode()
{
	sendMsg(Msg_emgCalReceived);
	return 1;
}

/**
Not used
*/
int SauAndEmgzProtocol::_calImpCmdDecode()
{
	sendMsg(Msg_impCalReceived);
	return 1;
}


/**
Decodes the command "GET_STATUS"
@return always 1
*/
int SauAndEmgzProtocol::_statusCmdDecode()
{
	int i = 0;

	m_currentMeasure = *((word*)&m_messageReceived[i]);
	i += 2;
	
	m_voltageMeasure[_VCC_EMGZ_] = *((word*)&m_messageReceived[i]);
	i += 2;
	m_voltageMeasure[_5VDC_EMGZ_] = *((word*)&m_messageReceived[i]);
	i += 2;
	m_voltageMeasure[_VREF_EMGZ_] = *((word*)&m_messageReceived[i]);
	i += 2;
	
	m_emgzStatus = m_messageReceived[i];
	i++;
	
	m_emgzNumErrorOccurred = *((word*)&m_messageReceived[i]);
	i += 2;
	
	m_overcurrent = m_messageReceived[i] & 0x0F;
	m_sampleContactImpEnable = (m_messageReceived[i] & 0xF0) >> 4;
	i++;

	m_LPF_type = m_messageReceived[i];
	i++;
	m_NOTCHF_type = m_messageReceived[i];
	i++;
	m_HPF_type = m_messageReceived[i];
	i++;
	
	sendMsg(Msg_statusReceived);
	return 1;
}

/**

*/
int SauAndEmgzProtocol::_impSampleDecode()
{
	word dataw;
	int i,k;
	byte lost
		;
	i = 0;
	lost = checkPacketsLost(m_messageReceived[i], m_numImpPacketsReceived);
	if(lost != 0)
	{
		m_packetLostImp = lost;
		m_numImpPacketsReceived = m_messageReceived[i];
		sendMsg(Msg_packetsLostImp);
	}
	i++;

	m_sauPcProtocol->sendSauPcEmgContactImpSamples(((word*)&m_messageReceived[i]), 3);

	sendMsg(Msg_contactImpSamplesReceived);
	m_numImpPacketsReceived++;
	
	return 1;
}

/**

*/
int SauAndEmgzProtocol::_emgSampleDecode()
{
	word dataw;
	int i, k, j;
	byte lost;
	
	i = 0;
	lost = checkPacketsLost(m_messageReceived[i], m_numEmgPacketsReceived);
	if(lost != 0)
	{
		m_packetLostEmg = lost;

		if(m_pFClearAllBuffers != NULL)
			(*m_pFClearAllBuffers)();
		/*if(lost > 10)
			lost = 10;
		
		for(j = 0; j < lost; j++)
		{
			m_sauPcProtocol->sendSauPcEmgSamples(m_emgSampleDummy, _NUM_HR_EMG_SAMPLES_PER_PACKET_);
		}*/
			
		m_numEmgPacketsReceived = m_messageReceived[i];
		sendMsg(Msg_packetsLostEmg);
	}
	i++;

	m_sauPcProtocol->sendSauPcEmgSamples(((word*)&m_messageReceived[i]), _NUM_HR_EMG_SAMPLES_PER_PACKET_);
	
	sendMsg(Msg_emgSamplesReceived);
	m_numEmgPacketsReceived++;
	return 1;
}

/**
Decodes the command "START_ACQUISITION"
@return always 1
*/
int SauAndEmgzProtocol::_acquisitionStartedCmdDecode()
{
	sendMsg(Msg_acquisitionStarted);
	return 1;
}

/**
Decodes the command "STOP_ACQUISITION"
@return always 1
*/
int SauAndEmgzProtocol::_acquisitionStoppedCmdDecode()
{
	sendMsg(Msg_acquisitionStopped);
	return 1;
}

int SauAndEmgzProtocol::_errorsCmdDecode()
{
	int i;

	i = 0;

	m_emgzBoardLastComErrors = *((word*)(&m_messageReceived[i]));
	i += 2;
	
	m_emgzBoardLastHwError = m_messageReceived[i];
	i++;
	
	m_emgzBoardLastGenericError = *((word*)(&m_messageReceived[i]));
	
	sendMsg(Msg_errorPacketReceived);
	return 1;
}

/**
Decodes commmand "ACKNOWLEDGE"
*/		
int SauAndEmgzProtocol::_acknowledgeCmdDecode()
{
	if (m_messageReceived[0] == 1) 
		sendMsg(Msg_acknowledge);
	else
		sendMsg(Msg_notAcknowledge);
	return 1;
}

/**
Decodes the command "JUMP_TO_LOADER"
@return always 1
*/
int SauAndEmgzProtocol::_jumpToLoaderCmdDecode()
{
	sendMsg(Msg_jumpedToLoader);
	return 1;
}

int SauAndEmgzProtocol::sendInfoRequest()
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)GET_INFO;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = GET_INFO;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 100);
}

int SauAndEmgzProtocol::sendStartAcq()
{	
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)START_ACQUISITION;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_numEmgPacketsReceived = 0;
	m_lastCmdSent = START_ACQUISITION;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 200);
}

int SauAndEmgzProtocol::sendStopAcq()
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)STOP_ACQUISITION;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = STOP_ACQUISITION;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 200);
}

int SauAndEmgzProtocol::sendStatusRequest()
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)GET_STATUS;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = GET_STATUS;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 500);
}

int SauAndEmgzProtocol::sendFilteringSetting(byte __LPF, byte __NOTCHF, byte __HPF)
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)SET_FILTERING;
	i++;
	packet[i] = 0;
	i++;

	packet[i] = __LPF;
	i++;
	packet[i] = __NOTCHF;
	i++;
	packet[i] = __HPF;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SET_FILTERING;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 200);
}

int SauAndEmgzProtocol::sendCheckContactImpedances(bool val)
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)CHECK_CONTACT_IMP;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = (val ? 1:0);
	i++;
	
	m_readElectrodeImpedanceActive = val;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = CHECK_CONTACT_IMP;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 100);
}


int SauAndEmgzProtocol::sendJumpedToLoaderRequest()
{
	byte packet[20];
	int i,k;

	i = 0;	
	packet[i] = __STX_EMGZ;
	i++;
	packet[i] = (byte)JUMP_TO_LOADER;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = stdUartProtocolAbstraction::calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = JUMP_TO_LOADER;
	return stdUartProtocolAbstraction::SendCommand(packet, i, 100);
}


/**
checks if the opcode received is valid

@param data opcode to check
@return true if the opcode is recognized, false otherwise 
@see Cmd_ToBoard
*/
bool SauAndEmgzProtocol::checkOpCode (byte data)
{
	if( data != INFO)	
		if( data != CAL_IMP)
			if( data != CAL_EMG)
				if( data != ACKNOWLEDGE)
					if( data != ACQUISITION_STARTED)
						if( data != ACQUISITION_STOPPED)
							if( data != STATUS)
								if( data != ERRORS)
									if( data !=  IMP_SAMPLES)
										if( data != EMG_SAMPLES)
											if( data != JUMPED_TO_LOADER)
												return false;
	return true;											
}

/**
Calculates the checksum of the receiving packet and verify if it has the same value of the received one.

@param receivedChk received checksum
@param opcode opcode of the command received
@return true if the two opcode are the same, flase otherwise
*/
bool SauAndEmgzProtocol::checksum(byte receivedChk, byte opCode)
{
	int i;
	byte myChk = 0;
	myChk += opCode;
	myChk += stdUartProtocolAbstraction::getNumDataReceived();

	for ( i = 0; i < stdUartProtocolAbstraction::getNumDataReceived(); i++)
	{
		myChk += stdUartProtocolAbstraction::bufferLastDataReceived()[i];
	}
	if (myChk == receivedChk)
	{
		return true;
	}
	return false;
}

/**
push a message in the messages' fifo.

@param i message
@return 0 if the message was pushed, -1 otherwise(full fifo).
@see Msg
*/
int SauAndEmgzProtocol::sendMsg(int i)
{
	if(!m_message.full())
	{
		m_message.push_back(i);
		return 0;
	}	
	return -1;
}

/**
@return the message within the fifo. if the fifo is empty return AcqHrMsg_noMsg
@see Msg
*/
int SauAndEmgzProtocol::getMsg()
{
	int msg = Msg_noMsg;
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
	}
	return msg;
}

byte SauAndEmgzProtocol::checkPacketsLost(byte numPacketsTx, byte numPacketsRx)
{
	int dif;
	//---- Checking for packets lost ----
	dif = (int)numPacketsTx - (int)numPacketsRx;
	if( dif != 0)
	{
		if (dif < 0)
		{
			dif += 256;
		}
	}
	return ((byte)dif);
}

void SauAndEmgzProtocol::getLastEmgzBoardErrors(word &comErr, byte &hwErr, word &genErr)
{
	comErr = m_emgzBoardLastComErrors;
	hwErr = m_emgzBoardLastHwError;
	genErr = m_emgzBoardLastGenericError;
}



































