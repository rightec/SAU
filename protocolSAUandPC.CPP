/**
@file		protocolSauPc.cpp
@brief		Class to manage the serial communication between the board HRC-7000 and the PC.

@author		Nicola
@date		18/01/2011
@version	01.1
*/

#include "protocolSAUandPC.h"

extern const char sAppName[];
extern const char sAppRevision[];
extern const char sAppDate[];
extern const char sHwVersion[];

extern const int sAppNameLength;
extern const int sAppRevisionLength;
extern const int sAppDateLength;
extern const int sHwVersionLength;

//extern ADuC_WEIGHT_protocol *VinfProtocol;

//---------------------------------------------------------------------------------------
//------------------------------ pcProtocol ---------------------------------------------
//---------------------------------------------------------------------------------------
/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
SauPcProtocol::SauPcProtocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize) :
	stdUartProtocolAbstraction(__pUartDevice)
{
	__pUartDevice->SetBaudRate(UartDeviceAbstract::BR460800, true);

	setReceiverState(stdUart_waitStx);
	setLastDataReceivedBuffer(__rxProtoBufSize);
	setMultiMessagesAnalisyPerLoop(true);
	rstBytesReceived();
	
	m_numPrsPacketsSent = 0;
	m_numEmgPacketsSent = 0;
	m_numGenPacketsSent = 0;
	m_numWeightPacketsSent = 0;
	m_numEmgContactImpPacketsSent = 0;
	
	m_lastWthSpeed = 0;
	m_lastWtpSpeed = 0;
		
	m_lastWthStatus = 0;
	m_lastWthPowered = 0;
	m_lastWtpStatus = 0;
	m_lastWthStep = 0;
	m_lastBubbleStatus = 0;
	
	m_lastHwError = sauPcUart_Hw_NoError;
	m_message.clear();
	m_lastCmdSent = SAUPC_NO_CMD_FROM_BOARD;
	
	m_messageReceivedLenght = 0;

	m_COMM_type = comm_FTDI_USBtoSERIAL;

	m_ringBuffer = new RingBuffer(30);

	m_watchdogIsEnable = true;
	m_watchdogTime = 3000;
	
	__pUartDevice->TurnOn();

	m_opCodeDataAnalyse = 0;
	m_lastRemoteIDDataAnalyse = 0;
	m_numBypassByteTransmittedDataAnalyse = 0;
	m_wtpSpeedCalibration = 0;
}

/**
Class destructor
*/
SauPcProtocol::~SauPcProtocol()
{
	delete m_ringBuffer;
}

/**
protocol decoding's function
*/
bool SauPcProtocol::DataAnalyse()
{

char dataBuff;

						// prima la gestione errori
	stdUartProtocolAbstraction::DataAnalyse();
									
	if(m_bypassMode) // by pass mode....
	{
		m_numBypassByteTransmittedDataAnalyse = 0;
		while(!m_pBypassUart->TransmissionBufferIsFull() && m_numBypassByteTransmittedDataAnalyse < 300)
		{
			if( m_pUart->ReceiveData(dataBuff))
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
	while( ( m_pUart->ReceiveData(dataBuff)) == true )
	{

		//newByteReceived();			// un nuovo byte arrivato
		
		if(m_COMM_type == comm_BT)
		{
			m_ringBuffer->Insert(dataBuff);
			if(m_ringBuffer->Equals(str_NO_CARRIER, str_NO_CARRIER_lenght) || m_ringBuffer->Equals(str_CLOSE, str_CLOSE_lenght))
			{
				sendMsg(SauPcMsg_BTcommunicationDropped);
			}
		}
		
		switch( getReceiverState() )
		{
			case stdUart_waitStx:
				if (dataBuff == SAUPC_STX)
				{
					stopTxTimeoutTimer();
					setTimeoutRx(_TIME_AFTER_STX);
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
				// setError(stdUart_ChecksumError);  // Set it as a generic error in case of umpredicted STOP: 21/01/2022 - It block the statrt up
				// setError((int)stdUart_stxNotReceived); // Set it as a generic error in case of umpredicted STOP: 21/01/2022 - It block the statrt up
				break;
		}	
	}
	
	return false;
}

/**
This funtion has to be called in the mail loop cycle becuase it manage all the class proprieties.
*/
void SauPcProtocol::manager()
{
	int error, opCode;

	stdUartProtocolAbstraction::manager();

	//---- Remapping stdUartProtocolAbstraction error ----
	error = popError();
	
	while (error != stdUart_NoError)
	{
		switch(error)
		{
			case stdUart_Hw_ReceptionError:
				m_lastHwError |=  sauPcUart_Hw_ReceptionError;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_ParityError:
				m_lastHwError |=  sauPcUart_Hw_ParityError;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_FrameError:
				m_lastHwError |=  sauPcUart_Hw_FrameError;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_OverrunError:
				m_lastHwError |=  sauPcUart_Hw_OverrunError;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_RxFifoOverrun:
			case stdUart_Hw_TxFifoOverrun:
				m_lastHwError |=  sauPcUart_Hw_OverrunFpgaFifo;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_NoDevice:
				m_lastHwError |=  sauPcUart_Hw_NoDevice;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_OverflowRxBuff:
				m_lastHwError |= sauPcUart_Hw_OverflowRxBuff;
				sendMsg(SauPcMsg_HwError);
				break;
			case stdUart_Hw_ErrorUnknown:
				m_lastHwError |= sauPcUart_Hw_ErrorUnknown;
				sendMsg(SauPcMsg_HwError);
				break;
			//  + da Queue.h -> errori protocollo
			case stdUart_UnknowOpcode:
				sendMsg(SauPcMsg_opcodeUnknown);
				break;
			case stdUart_stxNotReceived:
				sendMsg(SauPcMsg_wrongStx);
				break;
			case stdUart_TimeOutErrorR:
				sendMsg(SauPcMsg_timoutRx);
				break;
			case stdUart_TimeOutErrorT:
				sendMsg(SauPcMsg_timoutTx);
				break;
			case stdUart_ChecksumError:
				sendMsg(SauPcMsg_checkSumError);
				break;
			case stdUart_TransmitBufferOverflowError:
				sendMsg(SauPcMsg_transmitBufferOverflowError);
				break;
			case stdUart_ReceptionBufferOverflowError:
				sendMsg(SauPcMsg_receptionBufferOverflowError);
				break;
			case stdUart_DataReceivedBufferOverflow:
				sendMsg(SauPcMsg_dataReceivedBufferOverflow);
				break;
			case stdUart_ModuleResponseError:
			default:
				sendMsg(SauPcMsg_unknownError);	
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
			case SAUPC_GET_INFO:
				sauPc_getInfoCmdDecode();
				break;
			case SAUPC_GET_STATUS:
				sauPc_getStatusCmdDecode();
				break;
          	case SAUPC_START_ACQUISITION:
				sauPc_startAcquisitionCmdDecode();
				break;
            case SAUPC_STOP_ACQUISITION:
				sauPc_stopAcquisitionCmdDecode();
				break;
            case SAUPC_WTH_SET_SPEED:
				sauPc_setWthSpeedCmdDecode();
				break;
			case SAUPC_WTP_SET_SPEED:
				sauPc_setWtpSpeedCmdDecode();
				break;
			case SAUPC_WTH_SET_STATUS:
				sauPc_setWthStatusCmdDecode();
				break;
			case SAUPC_WTP_SET_STATUS:
				sauPc_setWtpStatusCmdDecode();
				break;
			case SAUPC_BUBBLE_SET_STATUS:
				sauPc_setBubbleStatusCmdDecode();
				break;
			case SAUPC_JUMP_TO_LOADER:
				sauPc_JumpToLoaderCmdDecode();
				break;
			case SAUPC_EMGZ_JUMP_TO_LOADER:
				sauPC_emgz_JumpToLoaderCmdDecode();
				break;
            case SAUPC_GET_CAL_WEIGHT:
				sauPc_getCalWeightCmdDecode();
				break;
            case SAUPC_GET_CAL_PRS:
				sauPc_getCalPrsCmdDecode();
				break;
            case SAUPC_GET_CAL_EMG:
				sauPc_getCalEmgCmdDecode();
				break;
            case SAUPC_GET_CAL_GENERIC:
				sauPc_getCalGenericCmdDecode();
				break;
            case SAUPC_SET_CAL_WEIGHT:
				sauPc_setCalWeightCmdDecode();
				break;
            case SAUPC_SET_CAL_PRS:
				sauPc_setCalPrs_CmdDecode();
				break;
            case SAUPC_SET_CAL_EMG:
				sauPc_setCalEmgCmdDecode();
				break;
            case SAUPC_SET_CAL_GENERIC:
				sauPc_setCalGeneric_CmdDecode();
				break;
            case SAUPC_ZERO_CHAN:
				sauPc_zeroPrsCmdDecode();
				break;
			case SAUPC_SET_DISPLAY:
				sauPc_setDisplayCmdDecode();
				break;
			case SAUPC_GET_DISPLAY:
				sauPc_getDisplayCmdDecode();
				break;
			case SAUPC_EMGZ_CONTAC_IMP:
				sauPc_emgzContactImpCmdDecode();
				break;
			case SAUPC_EMGZ_SET_FILTERING:
				sauPc_emgzSetFilteringCmdDecode();
				break;
			case SAUPC_SET_WATCHDOG:
				sauPc_setWatchdogCmdDecode();
				break;
			case SAU_PC_BT_BYPASS_MODE:
				sauPc_setBT_bypassModeCmdDecode();
				break;
			case SAUPC_SET_MOTOR_CAL:
				sauPc_setMotorCalCmdDecode();
				break;
			case SAUPC_GET_MOTOR_CAL:
				sauPc_getMotorCalCmdDecode();
				break;
			case SAUPC_SET_OVERPRESSURE:
				sauPc_setOverpressureCmdDecode();
				break;
			case SAUPC_WTP_SET_SPEED_CALIBRATION:
				sauPc_setWtpSpeedCalibrationCmdDecode();
				break;
		}
	}
}

/**
Decodes the command "SAUPC_GET_INFO"
@return always 1
*/
int SauPcProtocol::sauPc_getInfoCmdDecode()
{
	sendMsg(SauPcMsg_infoAsked);
	return 1;
}

/**
Decodes the command "SAUPC_GET_STATUS"
@return always 1
*/
int SauPcProtocol::sauPc_getStatusCmdDecode()
{ 
	sendMsg(SauPcMsg_statusAsked);
	return 1;
}

/**
Decodes the command "SAUPC_START_ACQUISITION"
@return always 1
*/
int SauPcProtocol::sauPc_startAcquisitionCmdDecode()
{
	sendMsg(SauPcMsg_startAcquisition);
	return 1;
}

/**
Decodes the command "SAUPC_STOP_ACQUISITION"
@return always 1
*/
int SauPcProtocol::sauPc_stopAcquisitionCmdDecode()
{
	sendMsg(SauPcMsg_stopAcquisition);
	return 1;
}

/**
Decodes the command "SAUPC_WTH_SET_SPEED"
@return always 1
*/
int SauPcProtocol::sauPc_setWthSpeedCmdDecode()
{
	m_lastWthSpeed = *((short*)m_messageReceived);
	sendMsg(SauPcMsg_wthSpeed);
	return 1;
}

/**
Decodes the command "SAUPC_WTP_SET_SPEED"
@return always 1
*/
int SauPcProtocol::sauPc_setWtpSpeedCmdDecode()
{
	m_lastWtpSpeed = *((word*)m_messageReceived);
	sendMsg(SauPcMsg_wtpSpeed);
	return 1;
}

/**
Decodes the command "SAUPC_WTH_SET_STATUS"
@return always 1
*/
inline int SauPcProtocol::sauPc_setWthStatusCmdDecode()
{
	m_lastWthPowered = *((byte*)m_messageReceived);
	m_lastWthStatus = *((byte*)&m_messageReceived[1]);
	m_lastWthStep = *((word*)(&m_messageReceived[2]));
	sendMsg(SauPcMsg_wthStatus);
	return 1;
}

/**
Decodes the command "SAUPC_WTP_SET_STATUS"
@return always 1
*/
inline int SauPcProtocol::sauPc_setWtpStatusCmdDecode()
{
	m_lastWtpStatus = *((byte*)m_messageReceived);
	sendMsg(SauPcMsg_wtpStatus);
	return 1;
}

/**
Decodes the command "SAUPC_BUBBLE_SET_STATUS"
@return always 1
*/
inline int SauPcProtocol::sauPc_setBubbleStatusCmdDecode()
{
	m_lastBubbleStatus = *((byte*)m_messageReceived);
	m_bubbleDetectionEn = (m_messageReceived[1] == 0) ? false : true;
	sendMsg(SauPcMsg_bubbleStatus);
	return 1;
}

/**
Decodes the command "SAUPC_JUMP_TO_LOADER"
@return always 1
*/
int SauPcProtocol::sauPc_JumpToLoaderCmdDecode()
{
	sendMsg(SauPcMsg_jumpToLoader);
	return 1;
}

int SauPcProtocol::sauPC_emgz_JumpToLoaderCmdDecode()
{
	sendMsg(SauPcMsg_emgz_jumpToLoader);
	return 1;
}
/**

*/
int SauPcProtocol::sauPc_getCalWeightCmdDecode()
{
	sendMsg(SauPcMsg_WeightCalAsked);
	return 1;
}

/**

*/
int SauPcProtocol::sauPc_getCalPrsCmdDecode()
{
	sendMsg(SauPcMsg_PrsCalAsked);
	return 1;
}
/**
Not used
*/
int SauPcProtocol::sauPc_getCalEmgCmdDecode()
{
	sendMsg(SauPcMsg_EmgCalAsked);
	return 1;
}
/**
Not used
*/
int SauPcProtocol::sauPc_getCalGenericCmdDecode()
{
	sendMsg(SauPcMsg_GenericCalAsked);
	return 1;
}

/**

*/
int SauPcProtocol::sauPc_setCalWeightCmdDecode()
{
	int i, k;
	double gain;
	long offset;
	for(k= 0, i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
	{
		((byte*)&gain)[0] = m_messageReceived[k++];
		((byte*)&gain)[1] = m_messageReceived[k++];
		((byte*)&gain)[2] = m_messageReceived[k++];
		((byte*)&gain)[3] = m_messageReceived[k++];
		((byte*)&offset)[0] = m_messageReceived[k++];
		((byte*)&offset)[1] = m_messageReceived[k++];
		((byte*)&offset)[2] = m_messageReceived[k++];
		((byte*)&offset)[3] = m_messageReceived[k++];
		
		m_weightChan->setGain(i, gain);
		m_weightChan->setOffset(i, offset);
	}
	sendMsg(SauPcMsg_WeightCalSet);
	return 1;
}

/**

*/
int SauPcProtocol::sauPc_setCalPrs_CmdDecode()
{
	int i, k;
	double gain;
	long offset;
	for(k= 0, i = 0; i < _NUM_OF_PRS_CHAN_; i++)
	{
		((byte*)&gain)[0] = m_messageReceived[k++];
		((byte*)&gain)[1] = m_messageReceived[k++];
		((byte*)&gain)[2] = m_messageReceived[k++];
		((byte*)&gain)[3] = m_messageReceived[k++];
		((byte*)&offset)[0] = m_messageReceived[k++];
		((byte*)&offset)[1] = m_messageReceived[k++];
		((byte*)&offset)[2] = m_messageReceived[k++];
		((byte*)&offset)[3] = m_messageReceived[k++];
		
		m_prsChan->setGain(i, gain);
		m_prsChan->setOffset(i, offset);
	}
	sendMsg(SauPcMsg_Prs_CalSet);
	return 1;
}	
/**
Not used
*/
int SauPcProtocol::sauPc_setCalEmgCmdDecode()
{
	sendMsg(SauPcMsg_EmgCalSet);
	return 1;
}
/**

*/
int SauPcProtocol::sauPc_setCalGeneric_CmdDecode()
{
	int i, k;
	double gain;
	long offset;
	k = 0;

	for(k= 0, i = 0; i < _NUM_OF_GENERIC_CHAN_; i++)
	{
		((byte*)&gain)[0] = m_messageReceived[k++];
		((byte*)&gain)[1] = m_messageReceived[k++];
		((byte*)&gain)[2] = m_messageReceived[k++];
		((byte*)&gain)[3] = m_messageReceived[k++];
		((byte*)&offset)[0] = m_messageReceived[k++];
		((byte*)&offset)[1] = m_messageReceived[k++];
		((byte*)&offset)[2] = m_messageReceived[k++];
		((byte*)&offset)[3] = m_messageReceived[k++];
		
		m_pGenChanGainOffset[i]->gain = gain;
		m_pGenChanGainOffset[i]->offset = offset;
	}

	sendMsg(SauPcMsg_Generic_CalSet);
	return 1;
}

/**
Not used
*/
int SauPcProtocol::sauPc_zeroPrsCmdDecode()
{
	sendMsg(SauPcMsg_zeroChan);
	return 1;
}

int SauPcProtocol::sauPc_setDisplayCmdDecode()
{
	m_displayBacklight = m_messageReceived[0];
	m_displayBrightness = m_messageReceived[1];
	m_displayContrast = m_messageReceived[2];
	
	m_displayWriteSettingToEEPROM = m_messageReceived[3];
	
	sendMsg(SauPcMsg_setDisplay);
	return 1;
}

int SauPcProtocol::sauPc_getDisplayCmdDecode()
{
	sendMsg(SauPcMsg_getDisplay);
	return 1;
}

int SauPcProtocol::sauPc_emgzContactImpCmdDecode()
{
	m_enableEmgzToReadContactImp = m_messageReceived[0];
	sendMsg(SauPcMsg_emgzContacImp);
	return 1;
}

int SauPcProtocol::sauPc_emgzSetFilteringCmdDecode()
{
	m_LPF_type = m_messageReceived[0];
	m_NOTCHF_type = m_messageReceived[1];
	m_HPF_type = m_messageReceived[2];
	sendMsg(SauPcMsg_emgzSetFiltering);
	
	return 1;
}

int SauPcProtocol::sauPc_setWatchdogCmdDecode()
{
	m_watchdogIsEnable = (m_messageReceived[0] == 0) ? false : true;
	m_watchdogTime = m_messageReceived[1] << 8;
	m_watchdogTime += m_messageReceived[2];
	if(m_watchdogTime < 2000)
		m_watchdogTime = 2000;
	
	sendMsg(SauPcMsg_watchdogSetting);
	return 1;
}

int SauPcProtocol::sauPc_setBT_bypassModeCmdDecode()
{
	sendMsg(SauPcMsg_BT_bypassMode);
	return 1;
}

int SauPcProtocol::sauPc_setMotorCalCmdDecode()
{	
	for(int i = 0; i < WTP_CALIBRATION_BOUNDS; i++)
	{
		m_wtpCalibrationValues[i] = *((wtpCalibration_t*)(&m_messageReceived[sizeof(wtpCalibration_t)*i]));
	}
	sendMsg(SauPcMsg_setMotorCal);
	return 1;
}

int SauPcProtocol::sauPc_getMotorCalCmdDecode()
{	
	sendMsg(SauPcMsg_getMotorCal);
	return 1;
}

int SauPcProtocol::sauPc_setOverpressureCmdDecode()
{
	if(m_messageReceivedLenght != 4)
	{
		sendAcknowledge(0);
		return -1;
	}
	m_overpressureLimit = *((short*)(&m_messageReceived[0]));
	m_overpressureChanEn = *((overpressure_bitF_t*)(&m_messageReceived[2]));	
	sendMsg(SauPcMsg_setOverpressure);
	return 1;
}

int SauPcProtocol::sauPc_setWtpSpeedCalibrationCmdDecode()
{

	if(m_messageReceivedLenght != 4)
	{
		sendAcknowledge(0);
		return -1;
	}

	m_wtpSpeedCalibration = *((dword*)(&m_messageReceived[0]));
	sendMsg(SauPcMsg_setWtpSpeedCalibration);
	return 1;
}
/**
Sends the information packets.

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_INFO - byte
- packet length - byte
- sAppRevision - String
- '\\t' - byte
- sAppDate - String
- '\\t' - byte
- sHwVersion - String
- '\\t' - byte
- FPGA info - byte
- uC Vinf firmware version - word
- uC VV firmware version - word
- uC P1-P2 firmware version - word
- uC P3-P4 firmware version - word
- uC P5-P6 firmware version - word
- uC P7-P8 firmware version - word
- uC Genric channels firmware version - word
- board EMG firmware version - byte
- board EMG harware version - byte
- checksum - byte

@param ADuCFirmwareVersion array of lenght 7 containing the ADuC firmware version. if -1 the correspondig ADuC is out of work.
@param len always 7 (number of ADuC7061 microcontrollers)
@param FPGAinfo FPGA information byte
@param emgBoardFwVersion High resolution EMG board firmware version. If -1 the EMG board is disconnected
@param emgBoardHwVersion High resolution EMG board Hardware version. If -1 the EMG board is disconnected
@return the number of byte transmitted. if 0 no bytes were transmitted.

@see SAU7000_Manager::SauPcMsg_infoAskedHandler
*/
int SauPcProtocol::sendSauPcInfo(word *ADuCFirmwareVersion, byte len, word FPGAinfo, char* emgAppRevision, char* emgAppDate, char* emgBoardHwRevision)
{
	byte packet[150];
	int i = 0;
	int k;
	int strLen;
	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_INFO;
	i++;
	packet[i] = 0;
	i++;
	for (k = 0; k < sAppRevisionLength; k++)
	{
		packet[i] = sAppRevision[k];
		i++;
	}
	packet[i] = '\t';
	i++;
	for (k = 0; k < sAppDateLength; k++)
	{
		packet[i] = sAppDate[k];
		i++;
	}
	packet[i] = '\t';
	i++;
	for (k = 0; k < sHwVersionLength; k++)
	{
		packet[i] = sHwVersion[k];
		i++;
	}
	packet[i] = '\t';
	i++;

	*((word*)&packet[i]) = FPGAinfo;
	i++;
	i++;
	
	for (k = 0; k < len ; k++)
	{
		*((word*)&packet[i]) = ADuCFirmwareVersion[k];
		i += 2;
	}

	strLen = strlen(emgAppRevision);
	if(strLen == 0)
		strLen = 1;
	for (k = 0; k <  strLen;  k++)
	{
		packet[i] = emgAppRevision[k];
		i++;
	}
	packet[i] = '\t';
	i++;
	
	strLen = strlen(emgAppDate);
	if(strLen == 0)
		strLen = 1;
	for (k = 0; k <  strLen;  k++)
	{
		packet[i] = emgAppDate[k];
		i++;
	}
	packet[i] = '\t';
	i++;

	strLen = strlen(emgBoardHwRevision);
	if(strLen == 0)
		strLen = 1;
	for (k = 0; k <  strLen;  k++)
	{
		packet[i] = emgBoardHwRevision[k];
		i++;
	}
	packet[i] = '\t';
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_INFO;
	return SendCommand(packet, i, 0);
}

/**
Answer to the commad get status

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_STATUS - byte
- data len - byte
- last current cunsumption value sampled (adc value) - word
- last power voltage value sampled (adc value) - _NUM_VOLTAGE_MEASURE_ words
- SAU7000_Manager status - byte @see SAU7000_Manager::Sau7000_Status
- SAU7000_Manager number of errors occurred - word
- ADuC Status - ADuC_NUM bytes
- HR EMG Board Status - 1 byte
- wtp status - byte (low nibble lit status, high nibble wtp status)
- wtp speed - word
- wth status - byte (low nibble switches status, high nibble wth status)
- wth speed - word
- wth step - word
- Bubble status - byte (low nibble clamp status, high nibble air status)
- 
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
@see SAU7000_Manager::SauPcMsg_statusAskedHandler
*/
int SauPcProtocol::sendSauPcStatus(byte *status, int len)
{
	byte packet[150];
	int i,k;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_STATUS;
	i++;
	packet[i] = 0;
	i++;
	for(k = 0; k < len; k++)
	{
		packet[i] = status[k];
		i++;
	}
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_STATUS;
	return SendCommand(packet, i, 0);
}

/**
Acknowledge to the command start acquisition

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_ACQUISITION_STARTED - byte
- 0 - byte
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.

@see SAU7000_Manager::SauPcMsg_startAcquisitionHandler
*/
int SauPcProtocol::sendSauPcAcquisitionStarted()
{
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_ACQUISITION_STARTED;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_ACQUISITION_STARTED;
	return SendCommand(packet, i, 0);
}

/**
Acknowledge to the command stop acquisition

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_ACQUISITION_STOPPED - byte
- 1 - byte
- cuase that stopped the acquisition
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.

@see SAU7000_Manager::SauPcMsg_stopAcquisitionHandler
*/
int SauPcProtocol::sendSauPcAcquisitionStopped(byte cause)
{
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_ACQUISITION_STOPPED;
	i++;
	packet[i] = 1;
	i++;
	packet[i] = cause;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_ACQUISITION_STOPPED;
	return SendCommand(packet, i, 0);
}


/**
Sends when an error occurred

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_ERRORS - byte
- data len - byte

- uC status - 1 byte 
  (bit6 (ADuC GEN)) bit5 (ADuC P7-P8), bit4 (ADuC P5-P6), bit3 (ADuC P3-P4), bit2 (ADuC P1-P2), bit1 (ADuC FLW) ,bit0 (ADuC VOL))
  if the bit if 0 the ADuC con not be reached. if the bit is 1 the ADuC works correclty.

- uC errors - 6 bytes
- packets lost with ADuCs - 6 bytes
- communication error with ADuC VOL - word
- communication error with ADuC FLW - word
- communication error with ADuC P1-P2 - word
- communication error with ADuC P3-P4 - word
- communication error with ADuC P5-P6 - word
- communication error with ADuC P7-P8 - word

- emg board error - byte- emg board error - byte
- packets lost with emg board - 2 bytes
- communication error with EMG board

- communication error with PC - word @see m_sauPcComErrors
- last uart hw error with PC - byte @see getLastHwError

- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
@see SAU7000_Manager::sendErrorsMsg
*/
int SauPcProtocol::sendSauPcErrors(byte *err, int len)
{
	byte packet[100];
	int i,k;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_ERRORS;
	i++;
	packet[i] = 0;
	i++;
	for(k = 0; k < len; k++)
	{
		packet[i] = err[k];
		i++;
	}
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_ERRORS;
	return SendCommand(packet, i, 0);
}
						
/**
Sends High Resolution EMG and Generic samples.

This packet is sent every 100Hz, so _NUM_HR_EMG_SAMPLES_PER_PACKET_ EMG samples are present and 
_NUM_GEN_SAMPLES_PER_PACKET_ generic samples (10 per each channel).

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_EMG_GEN_SAMPLES - byte
- data len - byte
- number of HrEmg&Generic packets sent - byte
- HrEmg samples - _NUM_HR_EMG_SAMPLES_PER_PACKET_ words
- Generic samples _NUM_GEN_SAMPLES_PER_PACKET_ words (the first 10 samples belong to the first channel the second 10 to the second one)
- checksum - byte

@param emgSamples array of the EMG samples
@param emgLen number of emg samples. It has always to be  _NUM_HR_EMG_SAMPLES_PER_PACKET_
@param genSamples array of the generic samples
@param genLen number of generic samples. It has always to _NUM_GEN_SAMPLES_PER_PACKET_. 
@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int SauPcProtocol::sendSauPcGenSamples(word *genSamples, int genLen)
{
	byte packet[120];
	int i, k;
	int genLenByte = genLen*sizeof(word);
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_GEN_SAMPLES;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = m_numGenPacketsSent;
	i++;
	
	memcpy( &packet[i], genSamples, genLenByte);
	i += genLenByte;

	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_numGenPacketsSent++;
	m_lastCmdSent = SAUPC_GEN_SAMPLES;
	return SendCommand(packet, i, 0);

}							 					

int SauPcProtocol::sendSauPcEmgSamples(word *emgSamples, int emgLen)
{
	byte packet[200];
	int i, k;
	int emgLenByte =  emgLen*sizeof(word);
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_EMG_SAMPLES;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = m_numEmgPacketsSent;
	i++;

	memcpy(&packet[i], emgSamples, emgLenByte);
	i+= emgLenByte;
	/*for(k = 0; k < emgLen; k++)
	{
		*((word*)&packet[i]) = emgSamples[k];
		i+=2;
	}*/
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_numEmgPacketsSent++;
	m_lastCmdSent = SAUPC_EMG_SAMPLES;
	return SendCommand(packet, i, 0);

}			

/**
Sends pressure samples.

This packet is sent every 50Hz, so _NUM_PRS_SAMPLES_PER_PACKET_ prs samples are present into the packets.

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_PRS_SAMPLES - byte
- data len - byte
- number of prs packets sent - byte 
- prs samples - _NUM_PRS_SAMPLES_PER_PACKET_ words (the fist 2 samples belog to P1, the second 2 samples to P2 and so on).
- checksum - byte

@param prsSamples array of the prs samples
@param prsLen number of prs samples. It has always to be  _NUM_PRS_SAMPLES_PER_PACKET_
@return the number of byte transmitted. if 0 no bytes were transmitted.
*/

int SauPcProtocol::sendSauPcPrsSamples(short *prsSamples, int prsLen)
{
	byte packet[50];
	int i, k;
	int prsLenByte = prsLen * sizeof(short);
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_PRS_SAMPLES;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = m_numPrsPacketsSent;
	i++;
	
	memcpy(&packet[i], prsSamples, prsLenByte);
	i += prsLenByte;
	
	/*for(k = 0; k < prsLen; k++)
	{
		*((word*)&packet[i]) = prsSamples[k];
		i+=2;
	}*/
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_numPrsPacketsSent++;
	m_lastCmdSent = SAUPC_PRS_SAMPLES;
	return SendCommand(packet, i, 0);

}

/**
Sends weight samples (infused volume,VOL, and voided volume, FLW,).

This packet is sent every 10Hz, so 1 VOL sample and 1 FLW sample are present into the packets.

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_FLW_VOL_SAMPLES - byte
- data len - byte
- number of weight packets sent - byte 
- VINF sample - 3 bytes
- VV sample - 3 bytes
- checksum - byte

@param weightSamples array of the weight samples (element 0 is the VOL, element 1 is the FLW)
@param weightLen number of weight samples. It has always to be _NUM_WEIGHT_SAMPLES_PER_PACKET_.
@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int SauPcProtocol::sendSauPcWeightSamples(long *weightSamples, int weightLen)
{
	byte packet[20];
	int i, k;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_WEIGHT_SAMPLES;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = m_numWeightPacketsSent;
	i++;
	for(k = 0; k < weightLen; k++)
	{
		packet[i] = (byte)((weightSamples[k] >> 16) & 0x000000FF) ;
		i++;
		packet[i] = (byte)((weightSamples[k] >> 8) & 0x000000FF) ;
		i++;
		packet[i] = (byte)((weightSamples[k]) & 0x000000FF) ;
		i++;
	}
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_numWeightPacketsSent++;
	m_lastCmdSent = SAUPC_WEIGHT_SAMPLES;
	return SendCommand(packet, i, 0);

}

int SauPcProtocol::sendSauPcEmgContactImpSamples(word *emgContactImpSamples, int len)
{
	byte packet[30];
	int i, k;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_EMG_CONTACT_IMP_SAMPLES;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = m_numEmgContactImpPacketsSent;
	i++;
	for(k = 0; k < len; k++)
	{
		*((word*)&packet[i]) = emgContactImpSamples[k];
		i+=2;
	}
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_numEmgContactImpPacketsSent++;
	m_lastCmdSent = SAUPC_EMG_CONTACT_IMP_SAMPLES;
	return SendCommand(packet, i, 0);

}


/**
Sends the WTP speed and status

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_WTP - byte
- data len - byte
- speed - word 
- status - byte
- lit status -byte
- checksum - byte

@param speed wtp speed...
@param status status (running, stop, limit switches status....)
*/
int SauPcProtocol::sendSauPcWtp(word speed, byte status, byte litStatus, dword driverStatus)
{
	byte packet[18];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_WTP;
	i++;
	packet[i] = 0;
	i++;
	*((word*)&packet[i]) = speed;
	i+=2;
	packet[i] = status;
	i++;
	packet[i] = litStatus;
	i++;
	*((dword*)&packet[i]) = driverStatus;
	i+=4;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_lastCmdSent = SAUPC_WTP;
	return SendCommand(packet, i, 0);
}

/**
Sends the WTH speed and status

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_WTH - byte
- data len - byte
- speed - word 
- status - byte
- step - word
- end switches status - byte
- checksum - byte

@param speed wth speed...
@param status status (running, stop, lit's switche status....)
*/
int SauPcProtocol::sendSauPcWth(byte power, word speed, byte status, byte endSwitchesStatus, word step)
{
	byte packet[14];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_WTH;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = power;
	i++;	
	*((word*)&packet[i]) = speed;
	i+=2;
	packet[i] = status;
	i++;
	packet[i] = endSwitchesStatus;
	i++;
	*((word*)&packet[i]) = step;
	i+=2;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_lastCmdSent = SAUPC_WTH;
	return SendCommand(packet, i, 0);

}

/**
Sends the Bubble status

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_WTH - byte
- data len - byte
- air status - word 
- clamp status - byte
- checksum - byte

@param air status wth speed...
@param clamp status
*/
int SauPcProtocol::sendSauPcBubble(byte airStatus, byte clampStatus, bool detectionEn, bool airTestFailed, bool airDetected, bool airTestRunning)
{
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_BUBBLE_STATUS;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = airStatus;
	i++;
	packet[i] = clampStatus;
	i++;
	packet[i] = detectionEn ? 1:0;
	i++;
	packet[i] = airTestFailed ? 1:0;
	i++;
	packet[i] = airDetected ? 1:0;
	i++;
	packet[i] = airTestRunning ? 1:0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
	m_lastCmdSent = SAUPC_BUBBLE_STATUS;
	return SendCommand(packet, i, 0);
}

/**
Sends the pressed key.

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_KEY_PRESSED - byte
- data len - byte
- key code - byte
- checksum - byte

@param key remote's key pressed
@return the number of byte transmitted. if 0 no bytes were transmitted.
@see kindOfKey
*/
int SauPcProtocol::sendSauPcRemoteKey(int key)
{
	byte packet[10];
	int i = 0;

	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_REMOTE_CONTROL;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = (byte)key;
	i++;
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_REMOTE_CONTROL;
	return SendCommand(packet, i, 0);

}

/**
Acknowledge to the request to jump to loader.

The packet is built as follow:
- SAUPC_STX - byte
- SAUPC_JUMPED_TO_LOADER - byte
- 0 - byte
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int SauPcProtocol::sendSauPcJumpedToLoader()
{
	byte packet[10];
	int i;

	i = 0;
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_JUMPED_TO_LOADER;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_JUMPED_TO_LOADER;
	return SendCommand(packet, i, 0);
}

int SauPcProtocol::sendSauPc_emgz_JumpedToLoader()
{
	byte packet[10];
	int i;

	i = 0;
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_EMGZ_JUMPED_TO_LOADER;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_EMGZ_JUMPED_TO_LOADER;
	return SendCommand(packet, i, 0);
}

/**
not used
*/
int SauPcProtocol::sendAcknowledge(byte ack)
{
	
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_ACKNOWLEDGE;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = ack;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_ACKNOWLEDGE;
	return SendCommand(packet, i, 0);
}

int SauPcProtocol::sendDisplaySetting(byte backlight, byte brightness, byte contrast)
{
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_DISPLAY;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = backlight;
	i++;
	packet[i] = brightness;
	i++;
	packet[i] = contrast;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_DISPLAY;
	return SendCommand(packet, i, 0);
}

/**

*/
int SauPcProtocol::sendPrsCalibrationValues(byte error)
{
	byte packet[90];
	int i;
	int k, chan;
	double gain;
	long offset;
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_CAL_PRS;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = error;
	i++;
	for (chan = 0; chan < _NUM_OF_PRS_CHAN_; chan++)
	{
		gain = m_prsChan->getGain(chan);
		offset = m_prsChan->getOffset(chan);
		for(k = 0; k < sizeof(double); k++)
		{
			packet[i++] = ((byte*)&gain)[k];
		}
		for(k = 0; k < sizeof(long); k++)
		{
			packet[i++] = ((byte*)&offset)[k];
		}
	}
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_CAL_PRS;
	return SendCommand(packet, i, 0);
}
/**

*/
int SauPcProtocol::sendWeightCalibrationValues(byte error)
{
	byte packet[50];
	int i;
	int k, chan;
	double gain;
	long offset;
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_CAL_WEIGHT;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = error;
	i++;
	for (chan = 0; chan < _NUM_OF_WEIGHT_CHAN_; chan++)
	{
		gain = m_weightChan->getGain(chan);
		offset = m_weightChan->getOffset(chan);
		for(k = 0; k < sizeof(double); k++)
		{
			packet[i++] = ((byte*)&gain)[k];
		}
		for(k = 0; k < sizeof(long); k++)
		{
			packet[i++] = ((byte*)&offset)[k];
		}
	}
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_CAL_WEIGHT;
	return SendCommand(packet, i, 0);

}

/**
not used
*/
int SauPcProtocol::sendGenericCalibrationValues(byte error)
{
	byte packet[50];
	int i;
	int k, chan;
	double gain;
	long offset;
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_CAL_GENERIC;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = error;
	i++;
	for (chan = 0; chan < _NUM_OF_GENERIC_CHAN_; chan++)
	{
		gain = m_pGenChanGainOffset[chan]->gain;
		offset = m_pGenChanGainOffset[chan]->offset;
		
		for(k = 0; k < sizeof(double); k++)
		{
			packet[i++] = ((byte*)&gain)[k];
		}
		for(k = 0; k < sizeof(long); k++)
		{
			packet[i++] = ((byte*)&offset)[k];
		}
	}
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_CAL_GENERIC;
	return SendCommand(packet, i, 0);

	return 0;
}

/**
not used
*/
int SauPcProtocol::sendEmgCalibrationValues(byte error)
{
	return 0;
}		

int SauPcProtocol::sendMotorsCalibrationValues(wtpCalibration_t * __wtpCalValues, byte error)
{
	
	byte packet[40];
	int i;
	
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_CAL_MOTOR;
	i++;
	packet[i] = 0;
	i++;

	*((wtpCalibration_t*)(&packet[i])) = __wtpCalValues[0];
	i += sizeof(wtpCalibration_t);
	*((wtpCalibration_t*)(&packet[i])) = __wtpCalValues[1];
	i += sizeof(wtpCalibration_t);
	*((wtpCalibration_t*)(&packet[i])) = __wtpCalValues[2];
	i += sizeof(wtpCalibration_t);
	
	packet[i] = error;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_CAL_MOTOR;
	return SendCommand(packet, i, 0);
	return 0;
}

int SauPcProtocol::sendOverpressureInfo(short __limit, overpressure_bitF_t __enChan, overpressure_bitF_t __overChan)
{
	byte packet[40];
	int i;
	
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_OVERPRESSURE_INFO;
	i++;
	packet[i] = 0;
	i++;
	*((short*)(&packet[i])) = __limit;
	i+=2;
	*((overpressure_bitF_t*)(&packet[i])) = __enChan;
	i+=2;
	*((overpressure_bitF_t*)(&packet[i])) = __overChan;
	i+=2;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_OVERPRESSURE_INFO;
	return SendCommand(packet, i, 0);

}


int SauPcProtocol::sendWtpSpeedCalibration(dword __speed, byte __error)
{
	byte packet[40];
	int i;
	
	
	i = 0;	
	packet[i] = SAUPC_STX;
	i++;
	packet[i] = (byte)SAUPC_WTP_SPEED_CALIBRATION;
	i++;
	packet[i] = 0;
	i++;
	*((dword*)(&packet[i])) = __speed;
	i+=4;
	packet[i] = __error;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = SAUPC_WTP_SPEED_CALIBRATION;
	return SendCommand(packet, i, 0);

}

/**
checks if the opcode received is valid

@param data opcode to check
@return true if the opcode is recognized, false otherwise 
@see SauPcCmd_FromPcToBoard
*/
bool SauPcProtocol::checkOpCode (byte data)
{
	if(data != SAUPC_GET_INFO)	
	if(data != SAUPC_GET_STATUS)
	if(data != SAUPC_START_ACQUISITION)
	if(data != SAUPC_STOP_ACQUISITION)
	if(data != SAUPC_WTH_SET_SPEED)
	if(data != SAUPC_WTP_SET_SPEED)
	if(data != SAUPC_WTH_SET_STATUS)
	if(data != SAUPC_WTP_SET_STATUS)
	if(data != SAUPC_BUBBLE_SET_STATUS)
	if(data != SAUPC_JUMP_TO_LOADER)
	if(data != SAUPC_GET_CAL_WEIGHT)
	if(data != SAUPC_GET_CAL_PRS)
	if(data != SAUPC_GET_CAL_EMG)
	if(data != SAUPC_GET_CAL_GENERIC)
	if(data != SAUPC_SET_CAL_WEIGHT)
	if(data != SAUPC_SET_CAL_PRS)
	if(data != SAUPC_SET_CAL_EMG)
	if(data != SAUPC_SET_CAL_GENERIC)
	if(data != SAUPC_ZERO_CHAN)
	if(data != SAUPC_SET_DISPLAY)
	if(data != SAUPC_GET_DISPLAY)
	if(data != SAUPC_EMGZ_CONTAC_IMP)
	if(data != SAUPC_EMGZ_SET_FILTERING)
	if(data != SAUPC_SET_WATCHDOG)
	if(data != SAUPC_EMGZ_JUMP_TO_LOADER)
	if(data != SAU_PC_BT_BYPASS_MODE)
	if(data != SAUPC_SET_MOTOR_CAL)
	if(data != SAUPC_GET_MOTOR_CAL)	
	if(data != SAUPC_SET_OVERPRESSURE)
	if(data != SAUPC_WTP_SET_SPEED_CALIBRATION)
		return false;

	return true;											
}
		
/**
Calculates the checksum of the receiving packet and verify if it has the same value of the received one.

@param receivedChk received checksum
@param opcode opcode of the command received
@return true if the two opcode are the same, flase otherwise
*/
bool SauPcProtocol::checksum(byte receivedChk, byte opCode)
{
	int i;
	byte myChk = 0;
	myChk += opCode;
	myChk += getNumDataReceived();

	for ( i = 0; i < getNumDataReceived(); i++)
	{
		myChk += bufferLastDataReceived()[i];
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
@see SauPcMsg
*/
int SauPcProtocol::sendMsg(int i)
{
	if(!m_message.full())
	{
		m_message.push_back(i);
		return 0;
	}	
	return -1;
}

/**
@return the message within the fifo. if the fifo is empty return SauPcMsg_noMsg
@see SauPcMsg
*/
int SauPcProtocol::getMsg()
{
	int msg = SauPcMsg_noMsg;
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
	}
	return msg;
}





































