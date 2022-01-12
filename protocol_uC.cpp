/**
@file		protocol_uC.cpp
@brief		Classes that manage a uart protocol with an external uC that sample HwChannels.

			Istances of this class are allocated to manage the communication with the ADuC7061 and the EMG board.
			
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/

#include "protocol_uC.h"

/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
uC_Protocol::uC_Protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	stdUartProtocolAbstraction(__pUartDevice)
{ 
 	int i;

	m_ADuCsettingReadback = new ADuCsetting();
	m_ADuCsetting = new ADuCsetting();
	
	__pUartDevice->SetBaudRate(UartDeviceAbstract::BR57600, true);

	setReceiverState(stdUart_waitStx);
	setLastDataReceivedBuffer(__rxProtoBufSize);
	setMultiMessagesAnalisyPerLoop(true);
	rstBytesReceived();
	
	m_lastHwError = sauuCUart_Hw_NoError;
	m_lastADuC_commError = __e_comm_NoError;
	m_message.clear();
	
	m_lastCmdSent = Cmd_toADuC_NO_CMD_FROM_HOST;
	m_lastCmdReceived = Cmd_fromADuC_NO_CMD_FROM_BOARD;
		
	m_numSample_01_PacketsReceived = 0;
	m_numSample_02_PacketsReceived = 0;
	m_numSample_03_PacketsReceived = 0;

	m_packetsLost_01 = 0;
	m_packetsLost_02 = 0;
	m_packetsLost_03 = 0;

	m_messageReceivedLenght = 0;
	
	m_ADuCStates = ADuC_unknownState;
	m_ADuCFwRevision = 0xFFFF;

	for(i = 0; i < __FW_NAME_DATE_MAX_LENGTH_; i++)
	{
		m_ADuCFwDate[i] = '\0';
		m_ADuCFwName[i] = '\0';
	}

	setLocalNodeID((int)'c');
	m_sendingSettingState = __sndSet_none;
	
	__pUartDevice->TurnOn();

	m_opCodeDataAnalyse = 0;
	m_lastRemoteIDDataAnalyse = 0;
	m_numStxNotRec = 0;

	m_enFrequencyCal = false;
	m_wantedSamplingFrq = 10;
	m_numOfPacketsReceived = 0;
	m_numOfSamplesPerPackets = 1;
	m_samplingFrqCalibrationTime = 10000; // in ms
	m_calculatedSampligFrq = 0;
	m_percentErrorOnSamplingFrq = 0;
	m_correctedSamplingFrq = 0;
	m_samplingFrq = m_wantedSamplingFrq;

	
	m_pFClearAllBuffers = NULL;
}

/**
Class destructor
*/
uC_Protocol::~uC_Protocol()
{
	delete m_ADuCsettingReadback;
	delete m_ADuCsetting;
}

/**
protocol decoding's function
*/
bool uC_Protocol::DataAnalyse()
{ 
char dataBuff;
						// prima la gestione errori
	stdUartProtocolAbstraction::DataAnalyse();
						// succhia dalla uart dati ed eventuali stati di errore
	if(m_pUart == NULL)
		return false;
	
	while( (m_pUart->ReceiveData(dataBuff)) == true )
	{
		switch(getReceiverState())
		{
			case stdUart_waitStx:
				if (dataBuff == ADuC_STX)
				{
					stopTxTimeoutTimer();
					setTimeoutRx(110);
					startRxTimeoutTimer();
					
					setReceiverState(stdUart_waitOpCode1);
					
					if(m_numStxNotRec !=0)
					{
						/*setcolor(BLACK);
						charset_select("xm5x6");
						gprintfxy(0,58,"num:%d", m_numStxNotRec);*/
					}
					m_numStxNotRec = 0;
				}else
				{
					
					if(getLastError() != stdUart_stxNotReceived)
					{
						setError((int)stdUart_stxNotReceived);
					}
					m_numStxNotRec++;
					/*switch(m_numStxNotRec)
					{
						case 1:
							cleardevice();
							setcolor(BLACK);
							charset_select("xm5x6");
							
							gprintfxy(0,0, "I'm in stx not rec");
							gprintfxy(0,6,"wstx1:%d", (byte)dataBuff);
							break;
						case 2:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,12,"wstx2:%d", (byte)dataBuff);
							break;
						case 3:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,18,"wstx3:%d", (byte)dataBuff);
							break;
						case 4:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,24,"wstx4:%d", (byte)dataBuff);
							break;
						case 5:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,30,"wstx5:%d", (byte)dataBuff);
							break;
						case 6:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,36,"wstx6:%d", (byte)dataBuff);
							break;
						case 7:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,42,"wstx7:%d", (byte)dataBuff);
							break;
						case 8:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,47,"wstx8:%d", (byte)dataBuff);
							break;
						case 9:
							setcolor(BLACK);
							charset_select("xm5x6");
							gprintfxy(0,52,"wstx9:%d", (byte)dataBuff);
							break;
					}*/
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
				
				setNumDataReceived(getPacketLength());
				if( checksum((byte)dataBuff, (byte)m_opCodeDataAnalyse) )
				{
					setReceivedOpCode(m_opCodeDataAnalyse);
					newPacketReceived();
					if(getMultiMessagesAnalisyPerLoop())
					{
						pushMultiMessages(bufferLastDataReceived(), getNumDataReceived());
					}
				}else
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
void uC_Protocol::manager()
{
	int error, opCode;

	if(m_pUart == NULL)
		return;
	
	stdUartProtocolAbstraction::manager();

	//---- Remapping stdUartProtocolAbstraction error ----
	error = popError();
	
	while (error != stdUart_NoError)
	{
		switch(error)
		{
			case stdUart_Hw_ReceptionError:
				m_lastHwError |=  sauuCUart_Hw_ReceptionError;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_ParityError:
				m_lastHwError |=  sauuCUart_Hw_ParityError;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_FrameError:
				m_lastHwError |=  sauuCUart_Hw_FrameError;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_OverrunError:
				m_lastHwError |=  sauuCUart_Hw_OverrunError;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_RxFifoOverrun:
			case stdUart_Hw_TxFifoOverrun:
				m_lastHwError |=  sauuCUart_OverrunFpgaFifo;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_NoDevice:
				m_lastHwError |=  sauuCUart_Hw_NoDevice;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_OverflowRxBuff:
				m_lastHwError |=  sauuCUart_Hw_OverflowRxBuff;
				sendMsg(msg_HwError);
				break;
			case stdUart_Hw_ErrorUnknown:
				m_lastHwError |= sauuCUart_Hw_ErrorUnknown;
				sendMsg(msg_HwError);
				break;
				
			//  + da Queue.h -> errori protocollo
			case stdUart_UnknowOpcode:
				sendMsg(msg_opcodeUnknown);
				break;
			case stdUart_stxNotReceived:
				sendMsg(msg_wrongStx);
				break;
			case stdUart_TimeOutErrorR:
				sendMsg(msg_timoutRx);
				m_sendingSettingState = __sndSet_none;
				break;
			case stdUart_TimeOutErrorT:
				sendMsg(msg_timoutTx);
				m_sendingSettingState = __sndSet_none;
				break;
			case stdUart_ChecksumError:
				sendMsg(msg_checkSumError);
				break;
			case stdUart_TransmitBufferOverflowError:
				sendMsg(msg_transmitBufferOverflowError);
				break;
			case stdUart_ReceptionBufferOverflowError:
				sendMsg(msg_receptionBufferOverflowError);
				break;
			case stdUart_DataReceivedBufferOverflow:
				sendMsg(msg_dataReceivedBufferOverflow);
				break;
			//case stdUart_ModuleResponseError:
			default:
				sendMsg(msg_unknownError);	
				break;

		}
		error = popError();
	}

	//------- Checking for packets receiving ----------------
	while( popPacketReceived(opCode) )
	{
		popMultiMessage(m_messageReceived, m_messageReceivedLenght);
		
		switch (opCode)
		{
			case Cmd_fromADuC_NO_CMD_FROM_BOARD:
				break;
		   case Cmd_fromADuC_INFO:
                __decode_InfoCmd();
                break;
            case Cmd_fromADuC_STATUS:
                __decode_SatusCmd();
                break;
            case Cmd_fromADuC_ACQUISITION_STARTED:
                __decode_StartAcquisitionCmd();
                break;
            case Cmd_fromADuC_ACQUISITION_STOPPED:
                __decode_StopAcquisitionCmd();
                break;
            case Cmd_fromADuC_SETTING_ADC:
                __decode_Set_ADC_Cmd();
                break;
            case Cmd_fromADuC_SETTING_IEXC:
                __decode_Set_IEXC_Cmd();
                break;
            case Cmd_fromADuC_SETTING_DAC:
                __decode_Set_DAC_Cmd();
                break;
            case Cmd_fromADuC_SETTING_PWM:
                __decode_Set_PWM_Cmd();
                break;
            case Cmd_fromADuC_ACK:
                __decode_AckCmd();
                break;
            case Cmd_fromADuC_NO_ACK:
                __decode_NotAckCmd();
                break;
            case Cmd_fromADuC_ERRORS:
                __decode_ErrorCmd();
                break;
            case Cmd_fromADuC_DATA_SAMPLES_01:
                __decode_Sample_01_Cmd();
                break;
            case Cmd_fromADuC_DATA_SAMPLES_02:
                __decode_Sample_02_Cmd();
                break;
            case Cmd_fromADuC_DATA_SAMPLES_03:
                __decode_Sample_03_Cmd();
                break;
            default:
                sendMsg(msg_opcodeUnknown);
                break;
		}
	}

	switch(m_sendingSettingState)
	{
		case __sndSet_none:
			break;
		case __sndSet_requestInfo:
			sendRequest_Info();
			m_sendingSettingState = __sndSet_waitingInfo;
			break;
		case __sndSet_waitingInfo:
			break;
		case __sndSet_send_ADC_setting:
			sendSetting_ADC();
			m_sendingSettingState = __sndSet_waiting_ADC_setting;
			break;
		case __sndSet_waiting_ADC_setting:
			break;
		case __sndSet_send_DAC_setting:
			sendSetting_DAC();
			m_sendingSettingState = __sndSet_waiting_DAC_setting;
			break;
		case __sndSet_waiting_DAC_setting:
			break;
		case __sndSet_send_IEXC_setting:
			sendSetting_IEXC();
			m_sendingSettingState = __sndSet_waiting_IEXC_setting;
			break;
		case __sndSet_waiting_IEXC_setting:
			break;
		case __sndSet_send_PWM_setting:
			sendSetting_PWM();
			m_sendingSettingState = __sndSet_waiting_PWM_setting;
			break;
		case __sndSet_waiting_PWM_setting:
			break;
		}
}

/**
Decodes the command "SAUPC_GET_INFO"
@return always 1
*/
int uC_Protocol::__decode_InfoCmd()
{
	int i;
	int k;
	byte node;
	
	i = 0;
	m_ADuCFwRevision =  m_messageReceived[i] << 8;
	i++;
	m_ADuCFwRevision += m_messageReceived[i];
	i++;

	k = 0;
	while ((m_messageReceived[i] != '\t') && (k < __FW_NAME_DATE_MAX_LENGTH_))
	{
		m_ADuCFwDate[k] = m_messageReceived[i];
		i++;
		k++;
	}
	i++;
	if(k >= __FW_NAME_DATE_MAX_LENGTH_)
		k = __FW_NAME_DATE_MAX_LENGTH_ - 1;
	m_ADuCFwDate[k] = '\0';

	k = 0;
	while ((m_messageReceived[i] != '\t') && (k < __FW_NAME_DATE_MAX_LENGTH_))
	{
		m_ADuCFwName[k] = m_messageReceived[i];
		i++;
		k++;
	}
	i++;
	if(k >= __FW_NAME_DATE_MAX_LENGTH_)
		k = __FW_NAME_DATE_MAX_LENGTH_ - 1;
	m_ADuCFwName[k] = '\0';
	
	m_ADuCHwRevision = m_messageReceived[i] << 8;
	i++;
	m_ADuCHwRevision += m_messageReceived[i];
	i++;

	m_ADuCStates = (ADuC_states)m_messageReceived[i];
	i++;

	node = m_messageReceived[i];
	i++;
	setLastRemoteNodeID(node);

	m_lastCmdReceived = Cmd_fromADuC_INFO;
	sendMsg(msg_infoReceived);

	if(m_sendingSettingState == __sndSet_waitingInfo)
	{
		m_sendingSettingState = __sndSet_send_ADC_setting;
	}
	return 1;
}

/**
Decodes the command "SAUPC_GET_STATUS"
@return always 1
*/
int uC_Protocol::__decode_SatusCmd()
{
	byte node;
	int dim = 0;
	dword dwData;
	
	m_ADuCStates =	(ADuC_states)m_messageReceived[dim++];
	
	node = m_messageReceived[dim++];
	setLastRemoteNodeID(node);
	

	m_selfTestEnable = m_messageReceived[dim++] == 0 ? false : true;
	m_selfTestTime = m_messageReceived[dim++] << 8;
	m_selfTestTime += m_messageReceived[dim++];
	
	m_lastCmdReceived = Cmd_fromADuC_STATUS;
	sendMsg(msg_statusReceived);

	return 1;
}

int uC_Protocol::__decode_Set_ADC_Cmd()
{
    int i, k;
    i = 0;
    word dataW;

    dataW = ((word)m_messageReceived[i++]) << 8;
    dataW += m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_frq(dataW);

    dataW = ((word)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADCFLT(dataW);

    //- PRIMARY ADC
    m_ADuCsettingReadback->set_ADC_P_enable(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_ADC_P_VREF((type_ADC_P_VREF) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_P_PGAgain((type_ADC_P_PGAgain) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_P_Mode((type_ADC_P_Mode) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_P_Coding((type_ADC_P_Coding) m_messageReceived[i++]);
    for (k = 0; k < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; k++)
    {
        m_ADuCsettingReadback->set_ADC_P_DifferentialChanEnabled(k, m_messageReceived[i++] == 0 ? 0 : 1);
    }
    for (k = 0; k < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; k++)
    {
        m_ADuCsettingReadback->set_ADC_P_SingleEndedChanEnabled(k, m_messageReceived[i++] == 0 ? 0 : 1);
    }
    //- AUXILIARY ADC
    m_ADuCsettingReadback->set_ADC_A_enable(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_ADC_A_VREF((type_ADC_A_VREF) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_A_PGAgain((type_ADC_A_PGAgain) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_A_Mode((type_ADC_A_Mode) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_A_Coding((type_ADC_A_Coding) m_messageReceived[i++]);
    for (k = 0; k < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; k++)
    {
        m_ADuCsettingReadback->set_ADC_A_DifferentialChanEnabled(k, m_messageReceived[i++] == 0 ? 0 : 1);
    }
    for (k = 0; k < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; k++)
    {
        m_ADuCsettingReadback->set_ADC_A_SingleEndedChanEnabled(k, m_messageReceived[i++] == 0 ? 0 : 1);
    }

    //- ADC Protocol
    m_ADuCsettingReadback->set_ADC_P_NumBitToSend((type_ADC_P_NumBitToSend) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_A_NumBitToSend((type_ADC_A_NumBitToSend) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_P_NumSamplesPerPacket((byte)m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_A_NumSamplesPerPacket((byte)m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_SendSamplesWithinSamePacket(m_messageReceived[i++] == 0 ? false : true);

    //- ADC REG
    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADCSTA(dataW);

    m_ADuCsettingReadback->set_ADC_reg_ADCMSKI((byte)m_messageReceived[i++]);
    m_ADuCsettingReadback->set_ADC_reg_ADCMDE((byte)m_messageReceived[i++]);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0CON(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC1CON(dataW);

    m_ADuCsettingReadback->set_ADC_reg_ADCCFG((byte)m_messageReceived[i++]);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0OF(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0GN(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC1OF(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC1GN(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0RCR(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0RCV(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0TH(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0THC(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0THV(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0ACC(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_ADC_reg_ADC0ATH(dataW);

    m_ADuCsettingReadback->checkSettingConsistency_ADC();

    m_lastCmdReceived = Cmd_fromADuC_SETTING_ADC;
    sendMsg(msg_received_SETTING_ADC);

	if(m_sendingSettingState == __sndSet_waiting_ADC_setting)
	{
		m_sendingSettingState = __sndSet_send_DAC_setting;
	}
	
    return 1;
}

int uC_Protocol::__decode_Set_IEXC_Cmd()
{
    int i;
    i = 0;
    m_ADuCsettingReadback->set_IEXC0_enable(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_IEXC1_enable(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_IEXC_Current((type_IEXC_Current) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_IEXC0_DIR((type_IEXC0_DIR) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_IEXC1_DIR((type_IEXC1_DIR) m_messageReceived[i++]);

    m_ADuCsettingReadback->set_IEXC_reg_IEXCON((byte)m_messageReceived[i++]);

    m_ADuCsettingReadback->checkSettingConsistency_IEXC();

    m_lastCmdReceived = Cmd_fromADuC_SETTING_IEXC;
    sendMsg(msg_received_SETTING_IEXC);

	if(m_sendingSettingState == __sndSet_waiting_IEXC_setting)
	{
		m_sendingSettingState = __sndSet_send_PWM_setting;
	}
	
    return 1;
}

int uC_Protocol::__decode_Set_DAC_Cmd()
{
    int i = 0;
    word dataW;
    dword dataD;

    m_ADuCsettingReadback->set_DAC_enable(m_messageReceived[i++] == 0 ? false : true);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_DAC_DAT(dataW);

    m_ADuCsettingReadback->set_DAC_range((type_DAC_Range) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_DAC_mode((type_DAC_mode) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_DAC_interpolationRate((type_DAC_interpolationRate) m_messageReceived[i++]);
    m_ADuCsettingReadback->set_DAC_OpampMode(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_DAC_Bypass(m_messageReceived[i++] == 0 ? false : true);

    dataW = ((word)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];;
    m_ADuCsettingReadback->set_DAC_reg_DACCON(dataW);

    dataD = ((dword)(byte)m_messageReceived[i++]) << 24;
    dataD += ((dword)(byte)m_messageReceived[i++]) << 16;
    dataD += ((dword)(byte)m_messageReceived[i++]) << 8;
    dataD += ((dword)(byte)m_messageReceived[i++]);
    m_ADuCsettingReadback->set_DAC_reg_DAC0DAT(dataD);

    m_ADuCsettingReadback->checkSettingConsistency_DAC();

    m_lastCmdReceived = Cmd_fromADuC_SETTING_DAC;
    sendMsg(msg_received_SETTING_DAC);

	if(m_sendingSettingState == __sndSet_waiting_DAC_setting)
	{
		m_sendingSettingState = __sndSet_send_IEXC_setting;
	}
	
    return 1;
}

int uC_Protocol::__decode_Set_PWM_Cmd()
{
    int i = 0;
    word dataW;
    dword dataD;

    //- PWM
    m_ADuCsettingReadback->set_PWM0_enable(m_messageReceived[i++] == 0 ? false : true);
    m_ADuCsettingReadback->set_PWM1_enable(m_messageReceived[i++] == 0 ? false : true);

    dataD = ((dword)(byte)m_messageReceived[i++]) << 24;
    dataD += ((dword)(byte)m_messageReceived[i++]) << 16;
    dataD += ((dword)(byte)m_messageReceived[i++]) << 8;
    dataD += ((dword)(byte)m_messageReceived[i++]);
    m_ADuCsettingReadback->set_PWM_frq(dataD);


    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_PWM0_duty(((double)dataW) / 10.0);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_PWM1_duty(((double)dataW) / 10.0);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_PWM_reg_PWMCON(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_PWM_reg_PWM0COM0(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];
    m_ADuCsettingReadback->set_PWM_reg_PWM0COM1(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];;
    m_ADuCsettingReadback->set_PWM_reg_PWM0COM2(dataW);

    dataW = ((word)(byte)m_messageReceived[i++]) << 8;
    dataW += (byte)m_messageReceived[i++];;
    m_ADuCsettingReadback->set_PWM_reg_PWM0LEN(dataW);

    m_ADuCsettingReadback->checkSettingConsistency_PWM();

    m_lastCmdReceived = Cmd_fromADuC_SETTING_PWM;
    sendMsg(msg_received_SETTING_PWM);

	if(m_sendingSettingState == __sndSet_waiting_PWM_setting)
	{
		m_sendingSettingState = __sndSet_none;
		sendMsg(msg_ALL_Setting_ACK);
	}
	
    return 1;
}

/**
Decodes the command
@return always 1
*/
int uC_Protocol::__decode_StopAcquisitionCmd()
{
    m_lastCmdReceived = Cmd_fromADuC_ACQUISITION_STOPPED;
    sendMsg(msg_acquisitionStopped);

    return 1;
}

/**
Decodes the command
@return always 1
*/
int uC_Protocol::__decode_AckCmd()
{
	m_lastCmdReceived = Cmd_fromADuC_ACK;
    sendMsg(msg_messageSentAck);
   
    return 1;
}

/**
Decodes the command
@return always 1
*/
int uC_Protocol::__decode_NotAckCmd()
{
	int i = 0;

	m_lastNotAckReason = m_messageReceived[i++];
	m_lastADuCsettingReceived = m_messageReceived[i++];
	m_ADuCsetting_consistencyError = m_messageReceived[i++];
		
    m_lastCmdReceived = Cmd_fromADuC_NO_ACK;
   
    sendMsg(msg_messageSentNotAck);
    return 1;
}

/**
Decodes the command
@return always 1
*/
int uC_Protocol::__decode_ErrorCmd()
{
    int i = 0;

	m_lastADuC_commError = ((dword)m_messageReceived[i++] << 24);
	m_lastADuC_commError = ((dword)m_messageReceived[i++] << 16);
    m_lastADuC_commError = ((dword)m_messageReceived[i++] << 8);
    m_lastADuC_commError += m_messageReceived[i++];

	/*cleardevice();
	gprintfxy(0,0, "I'M IN %d", 3);
	gprintfxy(0,10, "cmdS:%c", (char)m_lastCmdSent);
	gprintfxy(0,20, "cmdR:%c", (char)m_lastCmdReceived);
	gprintfxy(0,30, "ERR:0x%04X", (word)m_lastADuC_commError);
	display_update();
	while(1);*/
	
    m_lastCmdReceived = Cmd_fromADuC_ERRORS;
    sendMsg(msg_ErrorPacketReceived);

    return 1;
}

/**
Decodes the command
@return always 1
*/
int uC_Protocol::__decode_StartAcquisitionCmd()
{
    if( acquisitionStartedPackedDecode() != 1)
    {
    }

    m_lastCmdReceived = Cmd_fromADuC_ACQUISITION_STARTED;
    sendMsg(msg_acquisitionStarted);

    return 1;
}

/**
Decodes the command "SAUuC_DATA_SAMPLES_01"
@return always 1
*/
int uC_Protocol::__decode_Sample_01_Cmd()
{
int lost;
int numPacketReceived;

	numPacketReceived = samplePacket_01_decode();

	lost = checkPacketsLost(numPacketReceived, m_numSample_01_PacketsReceived);

	if ( lost != 0 )
	{
		m_packetsLost_01 = lost;

		sendMsg(msg_packetsLost_01);
		
		packetLost_01_handler();
	}

	m_numSample_01_PacketsReceived = numPacketReceived + 1;
	return 1;
}

/**
Decodes the command "SAUuC_DATA_SAMPLES_02"
@return always 1
*/
int uC_Protocol::__decode_Sample_02_Cmd()
{
byte numPacketReceived;
int lost;

	numPacketReceived = samplePacket_02_decode();

	lost = checkPacketsLost(numPacketReceived, m_numSample_02_PacketsReceived);
	
	if ( lost != 0)
	{
		m_packetsLost_02 = lost;
		
		sendMsg(msg_packetsLost_02);
		packetLost_02_handler();
	}

	m_numSample_02_PacketsReceived = numPacketReceived + 1;
	return 1;
}
	

int uC_Protocol::__decode_Sample_03_Cmd()
{
byte numPacketReceived;
int lost;

	numPacketReceived = samplePacket_03_decode();

	lost = checkPacketsLost(numPacketReceived, m_numSample_03_PacketsReceived);
	
	if ( lost != 0)
	{
		m_packetsLost_03 = lost;
		
		sendMsg(msg_packetsLost_03);
		packetLost_03_handler();
	}

	m_numSample_03_PacketsReceived = numPacketReceived + 1;
	return 1;
}

/**
Send a info request

The packet is built as follow:
- ADuC_STX - byte
- SAUuC_INFO - byte
- 0 - byte
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int uC_Protocol::sendRequest_Info()
{
	byte packet[10];
	int i;

	i = 0;
	packet[i] = ADuC_STX;
	i++;
	packet[i] = (byte)Cmd_toADuC_GET_INFO;
	i++;
	packet[i] = 0;
	i++;
	packet[i] = (byte)getLocalNodeID();
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = Cmd_toADuC_GET_INFO;
	return SendCommand(packet, i, 200);
}

/**
Send a status request

The packet is built as follow:
- ADuC_STX - byte
- SAUuC_STATUS - byte
- 0 - byte
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int uC_Protocol::sendRequest_Status()
{
byte packet[10];
int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
	packet[i] = (byte)Cmd_toADuC_GET_STATUS;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = Cmd_toADuC_GET_STATUS;
	return SendCommand(packet, i, 500);
}

int uC_Protocol::sendRequestSelfTest(bool __enable, int __time)
{
byte packet[10];
int i;
    if(!__enable)
        __time = 0;

    i = 0;	
	packet[i] = ADuC_STX;
	i++;
	packet[i] = (byte)Cmd_toADuC_SELF_TEST;
	i++;
	packet[i] = 0;
	i++;

	packet[i] = ((__time >> 8) & 0x00FF);
	i++;
	packet[i] = (__time & 0x00FF);
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = Cmd_toADuC_SELF_TEST;
	return SendCommand(packet, i, 500);
}

/**
Stop Acquisition

The packet is built as follow:
- ADuC_STX - byte
- SAUuC_START_ACQUISITION - byte
- length - byte
- param  - lenght byte (set the parametrs of the acquisition on the remote uC)
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int uC_Protocol::sendStartAcquisition()
{
byte packet[10];
int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
	packet[i] = (byte)Cmd_toADuC_START_ACQUISITION;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	rstSample_01_packetsReceived();
	rstSample_02_packetsReceived();
	rstSample_03_packetsReceived();
	
	m_lastCmdSent = Cmd_toADuC_START_ACQUISITION;
	return SendCommand(packet, i, 100);
	
}


/**
Stop Acquisition

The packet is built as follow:
- ADuC_STX - byte
- SAUuC_STOP_ACQUISITION - byte
- 0 - byte
- checksum - byte

@return the number of byte transmitted. if 0 no bytes were transmitted.
*/
int uC_Protocol::sendStopAcquisition()
{
	byte packet[10];
	int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
	packet[i] = (byte)Cmd_toADuC_STOP_ACQUISITION;
	i++;
	packet[i] = 0;
	i++;
	
	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

	m_lastCmdSent = Cmd_toADuC_STOP_ACQUISITION;
	return SendCommand(packet, i, 100);
}

int uC_Protocol::sendRequest_Setting_ADC()
{
    byte packet[10];
	int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
    packet[i] = Cmd_toADuC_request_SETTING_ADC;
	i++;
    packet[i] = 0;
	i++;

	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

    m_lastCmdSent = Cmd_toADuC_request_SETTING_ADC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendRequest_Setting_IEXC()
{
    byte packet[10];
	int i;
	
	i = 0;	
	packet[i] = ADuC_STX;
	i++;

    packet[i] = Cmd_toADuC_request_SETTING_IEXC;
	i++;
    packet[i] = 0;
	i++;

	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
	
    m_lastCmdSent = Cmd_toADuC_request_SETTING_IEXC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendRequest_Setting_DAC()
{
    byte packet[10];
	int i;
	
	i = 0;	
	packet[i] = ADuC_STX;
	i++;
;
    packet[i] = Cmd_toADuC_request_SETTING_DAC;
	i++;
    packet[i] = 0;
	i++;

	packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

    m_lastCmdSent = Cmd_toADuC_request_SETTING_DAC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendRequest_Setting_PWM()
{
    byte packet[10];
	int i;
	
	i = 0;	
	packet[i] = ADuC_STX;
	i++;

    packet[i] = Cmd_toADuC_request_SETTING_PWM;
	i++;
    packet[i] = 0;
	i++;

    packet[2] = i - 3;
	packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

    m_lastCmdSent = Cmd_toADuC_request_SETTING_PWM;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendSetting_ADC()
{
    byte packet[70];
    int k = 0;
	int i;
	
	i = 0;	
	packet[i] = ADuC_STX;
	i++;

    packet[i] = Cmd_toADuC_SETTING_ADC;
	i++;
    packet[i] = 0;
	i++;

    packet[i] = ((m_ADuCsetting->get_ADC_frq() & 0xFF00) >> 8);
	i++;
    packet[i] = (m_ADuCsetting->get_ADC_frq() & 0x00FF);
	i++;
    packet[i] = ((m_ADuCsetting->get_ADC_reg_ADCFLT() & 0xFF00) >> 8);
	i++;
    packet[i] = (m_ADuCsetting->get_ADC_reg_ADCFLT() & 0x00FF);
	i++;

    //- PRIMARY ADC
    packet[i] = m_ADuCsetting->get_ADC_P_isEnable() == true ? 1 : 0;
	i++;
    packet[i] = m_ADuCsetting->get_ADC_P_VREF();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_P_PGAgain();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_P_Mode();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_P_Coding();
	i++;
    for(k = 0; k < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; k ++)
    {
        packet[i] = m_ADuCsetting->get_ADC_P_DifferentialChanEnabled(k);
		i++;
    }
	for(k = 0; k < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; k ++)
	{
        packet[i] = m_ADuCsetting->get_ADC_P_SingleEndedChanEnabled(k);
		i++;
	}
    //- AUXILIARY ADC
    packet[i] = m_ADuCsetting->get_ADC_A_isEnable() == true ? 1 : 0;
	i++;
    packet[i] = m_ADuCsetting->get_ADC_A_VREF();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_A_PGAgain();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_A_Mode();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_A_Coding();
	i++;
    for(k = 0; k < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; k ++)
    {
        packet[i] = m_ADuCsetting->get_ADC_A_DifferentialChanEnabled(k);
		i++;
    }
    for(k = 0; k < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; k ++)
    {
		packet[i] = m_ADuCsetting->get_ADC_A_SingleEndedChanEnabled(k);
		i++;
    }
    //- ADC Protocol
    packet[i] = m_ADuCsetting->get_ADC_P_NumBitToSend();
	i++;
    packet[i] = m_ADuCsetting->get_ADC_A_NumBitToSend();
	i++;
    packet[i] = (m_ADuCsetting->get_ADC_P_NumSamplesPerPacket());
	i++;
    packet[i] = (m_ADuCsetting->get_ADC_A_NumSamplesPerPacket());
	i++;
    packet[i] = m_ADuCsetting->get_ADC_SendSamplesWithinSamePacket() == true ? 1 : 0;
	i++;

    packet[2] = i - 3;
    packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

    m_lastCmdSent = Cmd_toADuC_SETTING_ADC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendSetting_IEXC()
{
    byte packet[20];
    int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
    packet[i] = Cmd_toADuC_SETTING_IEXC;
	i++;
    packet[i] = 0;
	i++;

    packet[i] = m_ADuCsetting->get_IEXC0_isEnable() == true ? 1 : 0;
	i++;
    packet[i] = m_ADuCsetting->get_IEXC1_isEnable() == true ? 1 :0 ;
	i++;
    packet[i] = m_ADuCsetting->get_IEXC_Current();
	i++;
    packet[i] =  m_ADuCsetting->get_IEXC0_DIR();
	i++;
    packet[i] = m_ADuCsetting->get_IEXC1_DIR();
	i++;

    packet[2] = i - 3;
    packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;

    m_lastCmdSent = Cmd_toADuC_SETTING_IEXC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendSetting_DAC()
{
    byte packet[20];
    int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
    packet[i] = Cmd_toADuC_SETTING_DAC;
	i++;
    packet[i] = 0;
	i++;
    packet[i] = m_ADuCsetting->get_DAC_isEnable() == true ? 1 : 0;
	i++;
    packet[i] = ((m_ADuCsetting->get_DAC_DAT() & 0xFF00) >> 8);
	i++;
    packet[i] = (m_ADuCsetting->get_DAC_DAT() & 0x00FF);
	i++;
    packet[i] = m_ADuCsetting->get_DAC_range();
	i++;
    packet[i] = m_ADuCsetting->get_DAC_mode();
	i++;
    packet[i] = m_ADuCsetting->get_DAC_interpolationRate();
	i++;
    packet[i] = m_ADuCsetting->get_DAC_OpampMode() == true ? 1 : 0;
	i++;
    packet[i] = m_ADuCsetting->get_DAC_Bypass() == true ? 1 : 0;
	i++;

    packet[2] = i - 3;
    packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
		
    m_lastCmdSent = Cmd_toADuC_SETTING_DAC;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendSetting_PWM()
{
    byte packet[30];
    int i;

	i = 0;	
	packet[i] = ADuC_STX;
	i++;
    packet[i] = Cmd_toADuC_SETTING_PWM;
	i++;
    packet[i] = 0;
	i++;

    packet[i] = m_ADuCsetting->get_PWM0_isEnable() == true ? 1 : 0;
	i++;
    packet[i] = m_ADuCsetting->get_PWM1_isEnable() == true ? 1 : 0;
	i++;

    packet[i] = ((m_ADuCsetting->get_PWM_frq()& 0xFF000000) >> 24);
	i++;
    packet[i] = ((m_ADuCsetting->get_PWM_frq()& 0x00FF0000) >> 16);
	i++;
    packet[i] = ((m_ADuCsetting->get_PWM_frq()& 0x0000FF00) >> 8);
	i++;
    packet[i] = (m_ADuCsetting->get_PWM_frq() & 0x000000FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM0_duty()*10)) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM0_duty()*10)) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM1_duty()*10)) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM1_duty()*10)) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWMCON())) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWMCON())) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM0())) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM0())) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM1())) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM1())) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM2())) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0COM2())) & 0x00FF);
	i++;

    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0LEN())) & 0xFF00) >> 8;
	i++;
    packet[i] = (((word)(m_ADuCsetting->get_PWM_reg_PWM0LEN())) & 0x00FF);
	i++;

    packet[2] = i - 3;
    packet[i] = calculateChecksum(&packet[1], i - 1);
	i++;
		
    m_lastCmdSent = Cmd_toADuC_SETTING_PWM;
    return SendCommand(packet, i, 200);
}

int uC_Protocol::sendSetting()
{
	m_sendingSettingState = __sndSet_requestInfo;
}

/**
checks if the opcode received is valid

@param data opcode to check
@return true if the opcode is recognized, false otherwise 
@see SauuCCmd_From_uCToSau
*/
bool uC_Protocol::checkOpCode (byte data)
{
	if( data != Cmd_fromADuC_NO_CMD_FROM_BOARD)
    if( data != Cmd_fromADuC_INFO)
    if( data != Cmd_fromADuC_STATUS)
    if( data != Cmd_fromADuC_ACQUISITION_STARTED)
    if( data != Cmd_fromADuC_ACQUISITION_STOPPED)
    if( data != Cmd_fromADuC_SETTING_ADC)
    if( data != Cmd_fromADuC_SETTING_IEXC)
    if( data != Cmd_fromADuC_SETTING_DAC)
    if( data != Cmd_fromADuC_SETTING_PWM)
    if( data != Cmd_fromADuC_ACK)
    if( data != Cmd_fromADuC_NO_ACK)
    if( data != Cmd_fromADuC_ERRORS)
    if( data != Cmd_fromADuC_DATA_SAMPLES_01)
    if( data != Cmd_fromADuC_DATA_SAMPLES_02)
    if( data != Cmd_fromADuC_DATA_SAMPLES_03)
        return false;
	return true;											
}
		
/**
Calculates the checksum of the receiving packet and verify if it has the same value of the received one.

@param receivedChk received checksum
@param opcode opcode of the command received
@return true if the two opcode are the same, flase otherwise
*/
bool uC_Protocol::checksum(byte receivedChk, byte opCode)
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
		
		/*int k, nonZero;
		cleardevice();
		setcolor(BLACK);
		charset_select("xm5x6");
		
		gprintfxy(0,0, "I'm in checksum err");
		gprintfxy(0,6, "chkR:%d chkC:%d", receivedChk, myChk);
		gprintfxy(0,12,"cmdR:%c,cmdS:%c, OpC:%c", (char)m_lastCmdReceived,(char)m_lastCmdSent, (char)opCode);
		gprintfxy(0,18,"Lr:%d,Lp:%d,LC:%d,%d", getNumDataReceived(), getPacketLength(), bufferLastDataReceived()[getNumDataReceived()-1],bufferLastDataReceived()[getPacketLength()-2]);
		nonZero = 0;	
		gprintfxy(0,24,"Val:%d,Nz:%d", bufferLastDataReceived()[0], nonZero);
		for(k = 0;k < getNumDataReceived()-1; k++)
		{
			if(bufferLastDataReceived()[k] != 0)
			{
				nonZero++;
				gprintfxy(0,24,"Val:%d,Nz:%d", bufferLastDataReceived()[k], nonZero);
			}
		}
		display_update();
		while(1);*/
		
	return false;
}

/**
Verify if samples packet were lost.

@param numPacketTx number of packets transmitted (this number is sent from the uC)
@param numPacketsRx number of packets received (this value is incremented every time a packets is recevied)
@return the number of packets lost
*/
byte uC_Protocol::checkPacketsLost(byte numPacketsTx, byte numPacketsRx)
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

/**
push a message in the messages' fifo.

@param i message
@return 0 if the message was pushed, -1 otherwise(full fifo).
@see SauuCMsg
*/
int uC_Protocol::sendMsg(int i)
{
	if(!m_message.full())
	{
		m_message.push_back(i);
		return 0;
	}	
	return -1;
}

/**
@return the message within the fifo. if the fifo is empty return SauuCMsg_noMsg
@see SauuCMsg
*/
int uC_Protocol::getMsg()
{
	int msg = msg_noMsg;
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
	}
	return msg;
}

void uC_Protocol::enableFrequencyCal(bool __en)
{
	m_enFrequencyCal = __en;
	if (__en)
	{
		m_numOfPacketsReceived = 0;
	}
}


void uC_Protocol::calculateSamplingFrq()
{
	m_calculatedSampligFrq = ((float)(m_numOfPacketsReceived*m_numOfSamplesPerPackets*1000) / ((float)m_samplingFrqCalibrationTime));
	m_percentErrorOnSamplingFrq = (m_calculatedSampligFrq - (float)m_wantedSamplingFrq)/ (float)m_wantedSamplingFrq;
	m_correctedSamplingFrq = round3(((float)m_wantedSamplingFrq - (m_percentErrorOnSamplingFrq*(float)m_wantedSamplingFrq)));
	m_samplingFrq = m_correctedSamplingFrq;
	ADuCDefaultSetting();
}


//-----------------------------------------------------------------------------------------------------------------
//------------------------------------------------ ADuC_WEIGHT ----------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
ADuC_WEIGHT_protocol::ADuC_WEIGHT_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	uC_Protocol(__pUartDevice, __rxProtoBufSize)
{
	m_weightChan = NULL;
	m_wantedSamplingFrq = 10;
	m_samplingFrq = m_wantedSamplingFrq;
	m_numOfSamplesPerPackets = 1;
	ADuCDefaultSetting();
	/*setEnablePrimaryAdc(true);
	setPGAgain(PGA_Gain_64);
	setPrimaryAdcVREF(pAdc_ExternalRef_Vref1, false);
	setPrimaryAdcChannels(1, 1, 0, 0);
		
	setEnableAuxiliaryAdc(false);
	setAuxiliaryAdcVREF(aAdc_InternalRef, false);
	setAuxiliaryAdcChannels(0, 1, 4, 4, 4, 4);
		
	setADuCsampleFrq(10);

	setAdcsDataWhitinSamePacket(false);
	setSendOnly16Bit(false);
	
	setPrimaryAdcCoding(coding_unipolar);
	setAuxiliaryAdcCoding(coding_unipolar);*/
}

void ADuC_WEIGHT_protocol::ADuCDefaultSetting()
{
	int i;
	
    m_ADuCsetting->set_ADC_P_enable(true);
    m_ADuCsetting->set_ADC_frq(m_samplingFrq);
    
    m_ADuCsetting->set_ADC_P_VREF(pADC_Vref_ExternalRef_Vref1);
    m_ADuCsetting->set_ADC_P_VREF_mV(1200);
    m_ADuCsetting->set_ADC_P_PGAgain(pADC_PGA_Gain_64);
    m_ADuCsetting->set_ADC_P_Mode(pADC_mode_differential);
    m_ADuCsetting->set_ADC_P_Coding(pADC_coding_bipolar);

    for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_SingleEndedChanEnabled(i, 0);
    }

	m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(ADuC_ADC_P_DIFF_CHAN_ADC0_ADC1, 1);
	
    m_ADuCsetting->set_ADC_A_enable(false);
    m_ADuCsetting->set_ADC_A_VREF(aADC_Vref_InternalRef);
    m_ADuCsetting->set_ADC_A_VREF_mV(1200);
    m_ADuCsetting->set_ADC_A_PGAgain(aADC_PGA_Gain_1);
    m_ADuCsetting->set_ADC_A_Mode(aADC_mode_differential);
    m_ADuCsetting->set_ADC_A_Coding(aADC_coding_unipolar);
    for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_SingleEndedChanEnabled(i, 0);
    }

    m_ADuCsetting->set_ADC_P_NumBitToSend(pADC_bitToSend_24bit);
    m_ADuCsetting->set_ADC_A_NumBitToSend(aADC_bitToSend_24bit);
    m_ADuCsetting->set_ADC_P_NumSamplesPerPacket(m_numOfSamplesPerPackets);
    m_ADuCsetting->set_ADC_A_NumSamplesPerPacket(0);
    m_ADuCsetting->set_ADC_SendSamplesWithinSamePacket(false);

    
    m_ADuCsetting->set_IEXC0_enable(false);
    m_ADuCsetting->set_IEXC1_enable(false);
    m_ADuCsetting->set_IEXC_Current(IEXC_0uA);
    m_ADuCsetting->set_IEXC0_DIR(IEXC0_DIR_toIEXC0_pin);
    m_ADuCsetting->set_IEXC1_DIR(IEXC1_DIR_toIEXC1_pin);


    m_ADuCsetting->set_DAC_enable(true);
    m_ADuCsetting->set_DAC_DAT(0x0EAA);
    m_ADuCsetting->set_DAC_range(DAC_range_VREF_INT);
    m_ADuCsetting->set_DAC_VREF_mV(1200);
    m_ADuCsetting->set_DAC_mode(DAC_mode_normal_12bit);
    m_ADuCsetting->set_DAC_interpolationRate(DAC_interpolationRate_UCLK_16);
    m_ADuCsetting->set_DAC_OpampMode(0);
    m_ADuCsetting->set_DAC_Bypass(0);

    m_ADuCsetting->set_PWM0_enable(false);
    m_ADuCsetting->set_PWM1_enable(false);
    m_ADuCsetting->set_PWM_frq(0);
    m_ADuCsetting->set_PWM0_duty(0.0);
    m_ADuCsetting->set_PWM1_duty(0.0);

	ADuCsetting::settingConsistency consistency = m_ADuCsetting->checkSettingConsistency();
	if( consistency != ADuCsetting::sc_noError)
	{
		cleardevice();
		setcolor(BLACK);
		charset_select("pc6x8");
		gprintfxy(0,10, "WEIGHT = 0x%02X ", consistency);
		display_update();
		while(1)
		{
			asm("nop");
		}
	}
}

/**
Decodes the sample packets 01 and push the value into the GenericHwChanManager 

@param packet, received packet
	  
@param len parameters length in byte
@param numPacketReceived number of packets transmitted from the uC

@return always 1

@see sauuC_sample_01_CmdDecode
*/
int ADuC_WEIGHT_protocol::samplePacket_01_decode()
{
int i = 0;
long sample;
int numPacketReceived;

	sample = ((long)m_messageReceived[i]) << 16;
	i++;
	sample += ((long)m_messageReceived[i]) << 8;
	i++;
	sample += ((long)m_messageReceived[i]);
	i++;

	/*if(m_chan == 0)
		sample = 10;
	else
		sample = 20;
	*/
	m_weightChan->pushAdcDataToChan_isr(m_chan, sample);
	
	numPacketReceived = m_messageReceived[i];
	i++;

	if (m_weightChan->HwChanManagerError())
	{
		m_weightChan->clearError();
		m_weightChan->clearFifo();
		sendMsg(msg_weightChanBufferOverflow);
	}

	if(m_enFrequencyCal)
		m_numOfPacketsReceived++;
	
	return numPacketReceived;
	
}


/**
Manage packets lost - packet type 01

@param lost, num of packets lost
@return always 1

@see sauuC_sample_01_CmdDecode
*/
int ADuC_WEIGHT_protocol::packetLost_01_handler()
{
int i;
dword value = 0x00FFFFFF;
byte lost = m_packetsLost_01;


	if(m_pFClearAllBuffers != NULL)
		(*m_pFClearAllBuffers)();
	/*if (lost > 10)
		lost = 10;

	for (i = 0; i < lost; i++)
		m_weightChan->pushAdcDataToChan_isr(m_chan, value);

	if (m_weightChan->HwChanManagerError())
	{
		m_weightChan->clearError();
		m_weightChan->clearFifo_isr();
		sendMsg(msg_weightChanBufferOverflow);
	}*/
	
	return m_packetsLost_01;
}

//-----------------------------------------------------------------------------------------------------------------
//------------------------------------------------ ADuC_PRS -------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
ADuC_PRS_protocol::ADuC_PRS_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	uC_Protocol(__pUartDevice, __rxProtoBufSize)
{
	m_prsChan = NULL;
	m_wantedSamplingFrq = 200;
	m_samplingFrq = m_wantedSamplingFrq;
	m_numOfSamplesPerPackets = 2;
	ADuCDefaultSetting();
	/*setEnablePrimaryAdc(true);
	setPGAgain(PGA_Gain_32);
	setPrimaryAdcVREF(pAdc_ExternalRef_Vref1, false);
	setPrimaryAdcChannels(2, 1, 0, 2);
		
	setEnableAuxiliaryAdc(false);
	setAuxiliaryAdcVREF(aAdc_InternalRef, false);
	setAuxiliaryAdcChannels(0, 1, 4, 4, 4, 4);
	
	setADuCsampleFrq(205); // 205 Hz instead of 200 to compensate the delay of the multiplexing: rela smapling frq = 101,2 Hz

	setAdcsDataWhitinSamePacket(false);
	setSendOnly16Bit(false);

	setPrimaryAdcCoding(coding_unipolar);
	setAuxiliaryAdcCoding(coding_unipolar);*/
	//m_test = 0;
}

void ADuC_PRS_protocol::ADuCDefaultSetting()
{
	int i;

    m_ADuCsetting->set_ADC_P_enable(true);
    m_ADuCsetting->set_ADC_frq(m_samplingFrq);
    
    m_ADuCsetting->set_ADC_P_VREF(pADC_Vref_ExternalRef_Vref1);
    m_ADuCsetting->set_ADC_P_VREF_mV(1200);
    m_ADuCsetting->set_ADC_P_PGAgain(pADC_PGA_Gain_32);
    m_ADuCsetting->set_ADC_P_Mode(pADC_mode_differential);
    m_ADuCsetting->set_ADC_P_Coding(pADC_coding_bipolar);

    for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_SingleEndedChanEnabled(i, 0);
    }

	m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(ADuC_ADC_P_DIFF_CHAN_ADC0_ADC1, 1);
	m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(ADuC_ADC_P_DIFF_CHAN_ADC2_ADC3, 1);
	
    m_ADuCsetting->set_ADC_A_enable(false);
    m_ADuCsetting->set_ADC_A_VREF(aADC_Vref_InternalRef);
    m_ADuCsetting->set_ADC_A_VREF_mV(1200);
    m_ADuCsetting->set_ADC_A_PGAgain(aADC_PGA_Gain_1);
    m_ADuCsetting->set_ADC_A_Mode(aADC_mode_differential);
    m_ADuCsetting->set_ADC_A_Coding(aADC_coding_unipolar);
    for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_SingleEndedChanEnabled(i, 0);
    }

    m_ADuCsetting->set_ADC_P_NumBitToSend(pADC_bitToSend_16bit);
    m_ADuCsetting->set_ADC_A_NumBitToSend(aADC_bitToSend_16bit);
    m_ADuCsetting->set_ADC_P_NumSamplesPerPacket(m_numOfSamplesPerPackets);
    m_ADuCsetting->set_ADC_A_NumSamplesPerPacket(0);
    m_ADuCsetting->set_ADC_SendSamplesWithinSamePacket(false);
    
    m_ADuCsetting->set_IEXC0_enable(false);
    m_ADuCsetting->set_IEXC1_enable(false);
    m_ADuCsetting->set_IEXC_Current(IEXC_0uA);
    m_ADuCsetting->set_IEXC0_DIR(IEXC0_DIR_toIEXC0_pin);
    m_ADuCsetting->set_IEXC1_DIR(IEXC1_DIR_toIEXC1_pin);

    m_ADuCsetting->set_DAC_enable(true);
    m_ADuCsetting->set_DAC_DAT(0x0EAA);
    m_ADuCsetting->set_DAC_range(DAC_range_VREF_INT);
    m_ADuCsetting->set_DAC_VREF_mV(1200);
    m_ADuCsetting->set_DAC_mode(DAC_mode_normal_12bit);
    m_ADuCsetting->set_DAC_interpolationRate(DAC_interpolationRate_UCLK_16);
    m_ADuCsetting->set_DAC_OpampMode(0);
    m_ADuCsetting->set_DAC_Bypass(0);

    m_ADuCsetting->set_PWM0_enable(false);
    m_ADuCsetting->set_PWM1_enable(false);
    m_ADuCsetting->set_PWM_frq(0);
    m_ADuCsetting->set_PWM0_duty(0.0);
    m_ADuCsetting->set_PWM1_duty(0.0);

	ADuCsetting::settingConsistency consistency = m_ADuCsetting->checkSettingConsistency();
	if( consistency != ADuCsetting::sc_noError)
	{
		cleardevice();
		setcolor(BLACK);
		charset_select("pc6x8");
		gprintfxy(0,30, "PRS = 0x%02X ", consistency);
		display_update();
		while(1)
		{
			asm("nop");
		}
	}
}

/**
Decodes the sample packets 01 and push the value into the PrsHwChanManager 

@param packet, received packet
	  
@param len parameters length in byte
@param numPacketReceived number of packets transmitted from the uC

@return always 1

@see sauuC_sample_01_CmdDecode
*/
int ADuC_PRS_protocol::samplePacket_01_decode()
{
int i = 0;
//dword sample;
short data;
int numPacketReceived;

	data = ((short)m_messageReceived[i]) << 8;
	i++;
	data += ((short)m_messageReceived[i]) ;
	i++;
	//sample += ((dword)m_messageReceived[i]);
	//i++;
	
	//data = (word)(sample >> 8);
	/*if(m_chan_A == 0)
		data = 10;
	else if(m_chan_A == 1)
		data = 20;
	else if(m_chan_A == 2)
		data = 30;
	else if(m_chan_A == 3)
		data = 40;
	else if(m_chan_A == 4)
		data = 50;
	else if(m_chan_A == 5)
		data = 60;
	else if(m_chan_A == 6)
		data = 70;
	else if(m_chan_A == 7)
		data = 80;*/
	m_prsChan->pushAdcDataToChan_isr(m_chan_A, data);
	
	data = ((short)m_messageReceived[i]) << 8;
	i++;
	data += ((short)m_messageReceived[i]) ;
	i++;
	//data += ((dword)m_messageReceived[i]);
	//i++;
	
	//data = (word)(sample >> 8);
	/*if(m_chan_B == 0)
		data = 110;
	else if(m_chan_B == 1)
		data = 120;
	else if(m_chan_B == 2)
		data = 130;
	else if(m_chan_B == 3)
		data = 140;
	else if(m_chan_B == 4)
		data = 150;
	else if(m_chan_B == 5)
		data = 160;
	else if(m_chan_B == 6)
		data = 170;
	else if(m_chan_B == 7)
		data = 180;*/
	m_prsChan->pushAdcDataToChan_isr(m_chan_B, data);

	numPacketReceived = m_messageReceived[i];
	i++;	
		
	if (m_prsChan->HwChanManagerError())
	{
		m_prsChan->clearError();
		m_prsChan->clearFifo();
		sendMsg(msg_prsChanBufferOverflow);
	}

	if(m_enFrequencyCal)
		m_numOfPacketsReceived++;
	
	return numPacketReceived;
}



/**
Manage packets lost - packet type 01

@param lost, num of packets lost
@return always 1

@see sauuC_sample_01_CmdDecode
*/
int ADuC_PRS_protocol::packetLost_01_handler()
{
int i;
word value = 0xFFFF;
byte lost = m_packetsLost_01;

	if(m_pFClearAllBuffers != NULL)
		(*m_pFClearAllBuffers)();

	/*if (lost > 10)
		lost = 10;
	
	for (i = 0; i < lost; i++)
	{
		m_prsChan->pushAdcDataToChan_isr(m_chan_A, value);
		m_prsChan->pushAdcDataToChan_isr(m_chan_B, value);
	}

	if (m_prsChan->HwChanManagerError())
	{
		m_prsChan->clearError();
		m_prsChan->clearFifo_isr();
		sendMsg(msg_prsChanBufferOverflow);
	}*/
	
	return m_packetsLost_01;
}


//-----------------------------------------------------------------------------------------------------------------
//------------------------------------------------ ADuC_GEN -------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

/**
Class constructor.
@param device uart device
@param dim received data buffer dimension
*/
ADuC_GEN_protocol::ADuC_GEN_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	uC_Protocol(__pUartDevice, __rxProtoBufSize)
{

	/*setEnablePrimaryAdc(true);
	setPGAgain(PGA_Gain_1);
	setPrimaryAdcVREF(pAdc_ExternalRef_Vref1, false);
	setPrimaryAdcChannels(1, _ADuC_GEN_NUM_SAMPLES_PER_PACKET, 0, 2);
		
	setEnableAuxiliaryAdc(true);
	setAuxiliaryAdcVREF(aAdc_ExternalRef_Vref1, false);
	setAuxiliaryAdcChannels(1, _ADuC_GEN_NUM_SAMPLES_PER_PACKET, 2, 4, 4, 4);

	setADuCsampleFrq(1000);
	
	setAdcsDataWhitinSamePacket(true);
	setSendOnly16Bit(true);

	setPrimaryAdcCoding(coding_unipolar);
	setAuxiliaryAdcCoding(coding_unipolar);*/
	m_wantedSamplingFrq = 1000;
	m_samplingFrq = m_wantedSamplingFrq;
	m_numOfSamplesPerPackets = _ADuC_GEN_NUM_SAMPLES_PER_PACKET;
	ADuCDefaultSetting();
	
	for(int i = 0; i < _ADuC_GEN_NUM_SAMPLES_PER_PACKET*2; i++)
	{
		m_packetDummy[i] = 0xFFFF;
	}
}

void ADuC_GEN_protocol::ADuCDefaultSetting()
{
	int i;

    m_ADuCsetting->set_ADC_P_enable(true);
    m_ADuCsetting->set_ADC_frq(m_samplingFrq);
    
    m_ADuCsetting->set_ADC_P_VREF(pADC_Vref_InternalRef);
    m_ADuCsetting->set_ADC_P_VREF_mV(1200);
    m_ADuCsetting->set_ADC_P_PGAgain(pADC_PGA_Gain_1);
    m_ADuCsetting->set_ADC_P_Mode(pADC_mode_differential);
    m_ADuCsetting->set_ADC_P_Coding(pADC_coding_unipolar);

    for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_P_SingleEndedChanEnabled(i, 0);
    }

	m_ADuCsetting->set_ADC_P_DifferentialChanEnabled(ADuC_ADC_P_DIFF_CHAN_ADC0_ADC1, 1);
	
    m_ADuCsetting->set_ADC_A_enable(true);
    m_ADuCsetting->set_ADC_A_VREF(aADC_Vref_InternalRef);
    m_ADuCsetting->set_ADC_A_VREF_mV(1200);
    m_ADuCsetting->set_ADC_A_PGAgain(aADC_PGA_Gain_1);
    m_ADuCsetting->set_ADC_A_Mode(aADC_mode_differential);
    m_ADuCsetting->set_ADC_A_Coding(aADC_coding_unipolar);
    for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_DifferentialChanEnabled(i, 0);
    }
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADuCsetting->set_ADC_A_SingleEndedChanEnabled(i, 0);
    }

	m_ADuCsetting->set_ADC_A_DifferentialChanEnabled(ADuC_ADC_A_DIFF_CHAN_ADC2_ADC3, 1);
	
    m_ADuCsetting->set_ADC_P_NumBitToSend(pADC_bitToSend_16bit);
    m_ADuCsetting->set_ADC_A_NumBitToSend(aADC_bitToSend_16bit);
    m_ADuCsetting->set_ADC_P_NumSamplesPerPacket(m_numOfSamplesPerPackets);
    m_ADuCsetting->set_ADC_A_NumSamplesPerPacket(m_numOfSamplesPerPackets);
    m_ADuCsetting->set_ADC_SendSamplesWithinSamePacket(true);

    
    m_ADuCsetting->set_IEXC0_enable(false);
    m_ADuCsetting->set_IEXC1_enable(false);
    m_ADuCsetting->set_IEXC_Current(IEXC_0uA);
    m_ADuCsetting->set_IEXC0_DIR(IEXC0_DIR_toIEXC0_pin);
    m_ADuCsetting->set_IEXC1_DIR(IEXC1_DIR_toIEXC1_pin);


    m_ADuCsetting->set_DAC_enable(true);
    m_ADuCsetting->set_DAC_DAT(0x0EAA);
    m_ADuCsetting->set_DAC_range(DAC_range_VREF_INT);
    m_ADuCsetting->set_DAC_VREF_mV(1200);
    m_ADuCsetting->set_DAC_mode(DAC_mode_normal_12bit);
    m_ADuCsetting->set_DAC_interpolationRate(DAC_interpolationRate_UCLK_16);
    m_ADuCsetting->set_DAC_OpampMode(0);
    m_ADuCsetting->set_DAC_Bypass(0);

    m_ADuCsetting->set_PWM0_enable(false);
    m_ADuCsetting->set_PWM1_enable(false);
    m_ADuCsetting->set_PWM_frq(0);
    m_ADuCsetting->set_PWM0_duty(0.0);
    m_ADuCsetting->set_PWM1_duty(0.0);

	ADuCsetting::settingConsistency consistency = m_ADuCsetting->checkSettingConsistency();
	if( consistency != ADuCsetting::sc_noError)
	{
		cleardevice();
		setcolor(BLACK);
		charset_select("pc6x8");
		gprintfxy(0,50, "GEN = 0x%02X ", consistency);
		display_update();
		while(1)
		{
			asm("nop");
		}	
	}
}


/**
Decodes the sample packets 01 and push the value into the PrsHwChanManager 

@param parameters.
	  - Gen A sample (word)
	  - Gen B sample (word)
	  	_ADuC_GEN_NUM_SAMPLES_PER_PACKET times GEN A then _ADuC_GEN_NUM_SAMPLES_PER_PACKET times GEN B 
	  - num packet transmitted from the uC
	  
@param len parameters length in byte
@param numPacketReceived number of packets transmitted from the uC

@return always 1

@see sauuC_sample_01_CmdDecode
@see _ADuC_GEN_NUM_SAMPLES_PER_PACKET
*/
int ADuC_GEN_protocol::samplePacket_03_decode()
{
int i = 0;
int numPacketReceived;

	if(!m_enFrequencyCal)
		m_sauPcProtocol->sendSauPcGenSamples(((word*)&m_messageReceived[i]), 40);//_ADuC_GEN_NUM_SAMPLES_PER_PACKET*2);
		
	i+= 80;//_ADuC_GEN_NUM_SAMPLES_PER_PACKET*2*sizeof(word);
		
	numPacketReceived = m_messageReceived[i];
		
	if(m_enFrequencyCal)
		m_numOfPacketsReceived++;
	
	
	sendMsg(msg_packet03_received);
	return numPacketReceived;
}

/**
Manage packets lost - packet type 01

@param lost, num of packets lost
@return always 1

@see sauuC_sample_01_CmdDecode
*/
int ADuC_GEN_protocol::packetLost_03_handler()
{
	int i;
	int lost = m_packetsLost_03;

	if(m_pFClearAllBuffers != NULL)
		(*m_pFClearAllBuffers)();

	/*if(lost > 10)
		lost = 10;
	for (i = 0; i < lost; i++)
	{
		m_sauPcProtocol->sendSauPcGenSamples(m_packetDummy, 40);//_ADuC_GEN_NUM_SAMPLES_PER_PACKET*2);
	}*/
	
	return m_packetsLost_03;
}





































