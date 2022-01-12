/**
@file		protocol_uC.h
@brief		Classes that manage a uart protocol with an external uC that sample HwChannels.

			Istances of this classes are allocated to manage the communication with the ADuC7061 and the EMG board.
			
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


/**
STX value
*/


#ifndef _PROTOCOL_uC_H_
#define _PROTOCOL_uC_H_

#include "global.h"
#include "UartAbstract.h"
#include "protocolStdUart.h"
#include "protocolSAUandPC.h"
#include "channels.h"
#include "aducsetting.h"
#include "Util.h"

//#include "protocolSAUandPC.h"


/**
STX value
*/
#define ADuC_STX '\n'


#define __MAX_POWER_VOLTAGES__  20
#define __FW_NAME_DATE_MAX_LENGTH_	30
/**
Class to manage the serial communication between the board SAU-7000 and the ADuC7061 microcontrollers and the EMG board.

When a packet is decode a messages is pushed in the messages' fifo as well as an error occured. 
The messages have to be popped from the board manager using the propriety SauuCProtocol::getMsg().

The protocol packets are built as follow:
- (A) STX = '\\n'  - byte
- (B) OPCODE	  - byte	
- (C) DATA LENGTH - byte 
- (D) DATA        - DATA LENGTH bytes
- (E) CHECKSUM = (B)+(C)+(D)  - byte

Data length can be also 0, in this case the next data received will be the checksum.
Data length is the length of the field data.

The uart communication specifications are the follows:
- Baud rate: 115200
- Data bits: 8
- Stop bits: 1
- Parity: None
- Flow Control: None
*/
class uC_Protocol: public stdUartProtocolAbstraction
{
public:
	/**
    Commands opcode sent from the SAU-7000 to uC
    */
    enum Cmd_toADuC
    {
        Cmd_toADuC_NO_CMD_FROM_HOST = ' ',
        Cmd_toADuC_GET_INFO = 'i',				/**< sends info request to uC @see sauuC_getInfoCmdDecode @see sendSauuCInfo*/
        Cmd_toADuC_GET_STATUS = 'S',				/**< sends status request to uC @see sauuC_getStatusCmdDecode @see sendSauuCStatus*/
        Cmd_toADuC_SELF_TEST = 't',

        Cmd_toADuC_SETTING_ADC = 'A',				/**< sends setting (channels to sample, sample frq...)*/
        Cmd_toADuC_SETTING_IEXC = 'C',
        Cmd_toADuC_SETTING_DAC = 'D',
        Cmd_toADuC_SETTING_PWM = 'P',

        Cmd_toADuC_request_SETTING_ADC = 'a',				/**< sends setting (channels to sample, sample frq...)*/
        Cmd_toADuC_request_SETTING_IEXC = 'c',
        Cmd_toADuC_request_SETTING_DAC = 'd',
        Cmd_toADuC_request_SETTING_PWM = 'p',

        Cmd_toADuC_START_ACQUISITION = '1',		/**< starts acquisition @see sauuC_startAcquisitionCmdDecode @see sendSauuCAcquisitionStarted*/
        Cmd_toADuC_STOP_ACQUISITION = '2'		/**< stops acquisition @see sauuC_stopAcquuisitionCmdDecode @see sendSauuCAcquisitionStopped*/

    };
	
	/**
	   Commands opcode sent from the uC to SAU-7000
	   */
	   enum Cmd_fromADuC {
		   Cmd_fromADuC_NO_CMD_FROM_BOARD = ' ',
		   Cmd_fromADuC_INFO = Cmd_toADuC_GET_INFO, 						   /**< uC info @see sauuC_getInfoCmdDecode*/
		   Cmd_fromADuC_STATUS = Cmd_toADuC_GET_STATUS, 					   /**< board status @see sauuC_getStatusCmdDecode*/
	
		   Cmd_fromADuC_ACQUISITION_STARTED = Cmd_toADuC_START_ACQUISITION,    /**< acknowledge to cmd acquisition start @see sauuC_startAcquisitionCmdDecode*/
		   Cmd_fromADuC_ACQUISITION_STOPPED = Cmd_toADuC_STOP_ACQUISITION,	   /**< acknowledge to cmd acquisition stop @see sauuC_stopAcquuisitionCmdDecode*/
	
		   Cmd_fromADuC_SETTING_ADC = Cmd_toADuC_request_SETTING_ADC,
		   Cmd_fromADuC_SETTING_IEXC = Cmd_toADuC_request_SETTING_IEXC,
		   Cmd_fromADuC_SETTING_DAC = Cmd_toADuC_request_SETTING_DAC,
		   Cmd_fromADuC_SETTING_PWM = Cmd_toADuC_request_SETTING_PWM,
	
		   Cmd_fromADuC_ACK = 'k',								   /**< message aknowledge @see sendSauuCErrors*/
		   Cmd_fromADuC_NO_ACK = 'e',							   /**< error packets - not acknowledge @see sendSauuCErrors*/
		   Cmd_fromADuC_ERRORS = 'E',							   /**< error packets*/
		   Cmd_fromADuC_DATA_SAMPLES_01 = '3',						   /**< packets containing samples data type 1 (primary adc) @see  sauuC_sample_01_CmdDecode*/
		   Cmd_fromADuC_DATA_SAMPLES_02 = '4',						   /**< packets containing samples data type 2	(auxiliary adc)@see  sauuC_sample_02_CmdDecode*/
		   Cmd_fromADuC_DATA_SAMPLES_03 = '5'													   /**< primary and auxiliary ADC data*/
	   };
	
	/**
	Mesasges sent from the decoding protocol module to the board manager module.
	@see sendMsg
	@see getMsg
	*/
	enum Protocol_Msg
	{
		msg_noMsg = 0,

		msg_infoReceived,			/**< info packet received*/
		msg_statusReceived, 		/**< status packet received*/
		msg_acquisitionStarted, 		/**< acquisition started*/
		msg_acquisitionStopped, 		/**< acquisition stopped*/
		msg_packet01_received,
		msg_packet02_received,
		msg_packet03_received,
		msg_received_SETTING_ADC,
		msg_received_SETTING_IEXC,
		msg_received_SETTING_DAC,
		msg_received_SETTING_PWM,
		msg_ALL_Setting_ACK,
		
		msg_acquisitionNOTstarted,			/**< acquisition can not start @see sauuC_getStatusCmdDecode*/
		msg_messageSentNotAck,				/**< message sent not acknowledge*/
		msg_messageSentAck, 			/**< message sent acknowledge*/
		msg_ErrorPacketReceived,			/**< error message */

		msg_packetsLost_01, 			/**< sample packets lost kind 01 (primary adc)*/
		msg_packetsLost_02, 			/**< sample packets lost kind 02 (auxiliary adc)*/
		msg_packetsLost_03,
		msg_timoutRx,						/**< timeout RX elapsed*/
		msg_timoutTx,						/**< timeout TX elapsed*/
		msg_wrongStx,						/**< received wrong STX*/
		msg_opcodeUnknown,					/**< unknown opcode received*/
		msg_checkSumError,					/**< checksum error*/
		msg_HwError,						/**< uart hardware error @see sauuC_HwErrors*/
		msg_transmitBufferOverflowError,	/**< ADuC error: transmission buffer overflow error*/
		msg_receptionBufferOverflowError,	/**< ADuC error: reception buffer overflow error*/
		msg_dataReceivedBufferOverflow,

		msg_weightChanBufferOverflow,
		msg_prsChanBufferOverflow,
		msg_genChanBufferOverflow,
		
		msg_unknownError					/**< unknown error*/
	};
	

	/**
	uart hardware error
	@see m_lastHwError
	*/
	enum sauuC_HwErrors
	{
		sauuCUart_Hw_NoError 			= 0x00,
		sauuCUart_Hw_ReceptionError 	= 0x01,
		sauuCUart_Hw_ParityError    	= 0x02,
		sauuCUart_Hw_FrameError 		= 0x04, 
		sauuCUart_Hw_OverrunError 		= 0x08,
		sauuCUart_Hw_NoDevice			= 0x10,
		sauuCUart_Hw_ErrorUnknown		= 0x20,
		sauuCUart_Hw_OverflowRxBuff		= 0x40,
		sauuCUart_OverrunFpgaFifo		= 0x08
	};

	 /**
    ADuC possibles states
    */
    typedef enum
    {
        ADuC_unknownState = 0,
        ADuC_idleState = 's',
        ADuC_AquiringState = 'A',
        ADuC_SelfTestState = 't'
    }ADuC_states;

    enum __comm_Errors_type
    {
        __e_comm_NoError = 0,
		__e_comm_uart_overrun,
		__e_comm_uart_parity,
		__e_comm_uart_frame,
        __e_comm_OpCodeUnknown,
        __e_comm_WrongNumData,
        __e_comm_CrcError,
        __e_comm_EtxNotReceived,
        __e_comm_StxNotReceived,
        __e_comm_TxFifoOverflow,
        __e_comm_RxFifoOverflow,
        __e_comm_TimeoutRx,
        __e_comm_TimeoutTx,
        __e_comm_OpcodeFifoOverflow,
        __e_comm_ReceivedDataFifoOverflow,
        __e_comm_pAdcFifoOverflow,
        __e_comm_aAdcFifoOverflow
    };
		

    enum __NotAck_reasons_type
    {
        __nack_unknown = 0,
        __nack_wrong_ADC_setting,
        __nack_wrong_IEXC_setting,
        __nack_wrong_DAC_setting,
        __nack_wrong_PWM_setting,
        __nack_not_ALL_setting_received,
        __nack_kindOfNodeNotSet,
        __nack_wrongNumParam
    };

	
	uC_Protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);
	~uC_Protocol();

	virtual void manager();

	virtual void TurnOn(){stdUartProtocolAbstraction::TurnOn();}
	virtual void TurnOff(){stdUartProtocolAbstraction::TurnOff();}
	void clearTx(){stdUartProtocolAbstraction::clearTx();}
	void clearRx(){stdUartProtocolAbstraction::clearRx();}
	/**
	@return the number of bytes in the transmission buffer.
	*/
	/**
    @return the number of bytes in the transmission buffer.
    */
    int sendRequest_Info();
    int sendRequest_Status();
    int sendRequestSelfTest(bool __enable, int __time);
    int sendStopAcquisition();
    int sendStartAcquisition();
    int sendRequest_Setting_ADC();
    int sendRequest_Setting_IEXC();
    int sendRequest_Setting_DAC();
    int sendRequest_Setting_PWM();
    int sendSetting_ADC();
    int sendSetting_IEXC();
    int sendSetting_DAC();
    int sendSetting_PWM();
	int sendSetting();
	
	virtual int getMsg();
	
	/**
	@return the last command sent to the uC 
	@see SauuCCmd_FromSauTOuC
	*/
	Cmd_toADuC getLastCmdSent(){ return m_lastCmdSent;};

	/**
	@return the last command received from  the uC 
	@see SauuCCmd_From_uCToSau
	*/
	Cmd_fromADuC getlastCmdReceived(){ return m_lastCmdReceived;};

	
	/**
	@return the last error occured on the uC
	*/
	dword getLastADuC_commError(){return m_lastADuC_commError;};

	/**
	resets the last uC error
	*/
	void rstLLastADuC_commError(){m_lastADuC_commError = __e_comm_NoError;};

	int getLastADuC_notAckReason(){return m_lastNotAckReason;}
	int getLastADuC_settingReceived(){return m_lastADuCsettingReceived;}
	int getLastADuC_consistencyError(){return m_ADuCsetting_consistencyError;}

	/**
	@return the last uart hardware error
	*/
	int getLastHwError(){return m_lastHwError;}

	/**
	resets the last uart hardware error
	*/
	void rstLastHwError(){m_lastHwError = sauuCUart_Hw_NoError;}

	/**
	resets the number of packets type sample 01 received
	*/
	void rstSample_01_packetsReceived(){ m_numSample_01_PacketsReceived = 0;}
	/**
	resets the number of packets type sample 02 received
	*/
	void rstSample_02_packetsReceived(){ m_numSample_02_PacketsReceived = 0;}

	void rstSample_03_packetsReceived(){ m_numSample_03_PacketsReceived = 0;}
	/**
	@return the number of packets type 01 lost
	*/
	byte getNumPacketsLost01(){ return m_packetsLost_01;};

	/**
	@return the number of packets type 02 lost
	*/
	byte getNumPacketsLost02(){ return m_packetsLost_02;};

	byte getNumPacketsLost03(){ return m_packetsLost_03;};

	/**
	Sends a byte
	@return 1 if the byte was sent, 0 otherwise
	*/
	int sendByte(byte data){return stdUartProtocolAbstraction::SendCommand(&data, 1, 0);};
	
	ADuC_states getADuCState() { return m_ADuCStates;}
    word getADuCHwRevision() { return m_ADuCHwRevision;}
    word getADuCFwRevision() { return m_ADuCFwRevision;}
    char* getADuCFwDate() { return m_ADuCFwDate;}
    char* getADuCFwName(){ return m_ADuCFwName;}

    void set_ADuCsetting(ADuCsetting *__ADuCsetting){m_ADuCsetting = __ADuCsetting;}
    ADuCsetting *pADuCsettingReadback(){return m_ADuCsettingReadback;}

    bool getSelfTestEnable(){return m_selfTestEnable;}
    int getSelfTestTime(){return m_selfTestTime;}

    long getPacketsReceived();
    long getPacketsLost();
    long getNumTimeOutRx();
    long getNumTimeOutTx();
    long getNumCrcError();
    long getNumStxNotReceived();
    long getNumEtxNotReceived();
    long getNumUnkownOpcodeReceived();
    void rstCommunicationStatistics();

    double getPowerVoltage(int __i){if (__i < __MAX_POWER_VOLTAGES__) return m_powerVoltages[__i]; return 0;}
    double* getPowerVoltages(){return m_powerVoltages;}

	void setProtocolPc(SauPcProtocol *__sauPcProtocol){m_sauPcProtocol = __sauPcProtocol;}

	void enableFrequencyCal(bool __en);
	bool frqequencyCalIsEnabled(){return m_enFrequencyCal;}
	int getWantedSamplingFrq(){return m_wantedSamplingFrq;}
	void setWantedSamplingFrq(int __frq){ m_wantedSamplingFrq = __frq;}
	unsigned long getNumPacketsReceived(){return m_numOfPacketsReceived;}
	void rstNumPacketsReceived(){m_numOfPacketsReceived = 0;}
	int getNumOfSamplesPerPackets(){return m_numOfSamplesPerPackets;}
	void rstNumOfSamplesPerPackets(){m_numOfSamplesPerPackets = 0;}
	unsigned long getSamplingFrqCalibrationTime(){return m_samplingFrqCalibrationTime;}
	void setSamplingFrqCalibrationTime(unsigned long __time){m_samplingFrqCalibrationTime = __time;}
	float getCalculatedSampligFrq(){return m_calculatedSampligFrq;}
	float getPercentErrorOnSamplingFrq(){return m_percentErrorOnSamplingFrq;};
	int getCorrectedSamplingFrq(){return m_correctedSamplingFrq;}
	int getSamplingFrq(){ return m_samplingFrq;};
	void setSamplingFrq(int __frq){ m_samplingFrq = __frq;}
	
	void calculateSamplingFrq();

	void registerClearAllBuffFunct(void (*__pFClearAllBuffers)()){m_pFClearAllBuffers = __pFClearAllBuffers;}

	void setUartDevice(UartDeviceAbstract *__pUartDevice){m_pUart = __pUartDevice;};
protected:
	void (*m_pFClearAllBuffers)();

	enum __sendingSettingStates
	{
		__sndSet_none = 0,
		__sndSet_requestInfo,
		__sndSet_waitingInfo,
		__sndSet_send_ADC_setting,
		__sndSet_waiting_ADC_setting,
		__sndSet_send_DAC_setting,
		__sndSet_waiting_DAC_setting,
		__sndSet_send_IEXC_setting,
		__sndSet_waiting_IEXC_setting,
		__sndSet_send_PWM_setting,
		__sndSet_waiting_PWM_setting
	};
	
	virtual bool DataAnalyse();
	
	/**
    comandi fissi
    */
    int __decode_InfoCmd();
    virtual int __decode_SatusCmd();
    int __decode_Set_ADC_Cmd();
    int __decode_Set_IEXC_Cmd();
    int __decode_Set_DAC_Cmd();
    int __decode_Set_PWM_Cmd();

    /**
    messaggi variabili
    */
    int __decode_StopAcquisitionCmd();
    int __decode_AckCmd();
    int __decode_NotAckCmd();
    int __decode_ErrorCmd();
    int __decode_StartAcquisitionCmd();
    int __decode_Sample_01_Cmd();
    int __decode_Sample_02_Cmd();
    int __decode_Sample_03_Cmd();

    /**
    supporto messaggi variabili
    */
    virtual int acquisitionStartedPackedDecode() { return 0; }
    virtual int samplePacket_01_decode() { return 0; }
    virtual int samplePacket_02_decode() { return 0; }
    virtual int samplePacket_03_decode() { return 0; }
    virtual int packetLost_01_handler() { return 0; }
    virtual int packetLost_02_handler() { return 0; }
    virtual int packetLost_03_handler() { return 0; }

	virtual int sendMsg(int i);
	
	bool checkOpCode(byte data);

	bool checksum(byte receivedChk, byte opCode);

	byte checkPacketsLost(byte numPacketsTx, byte numPacketsRx);

	virtual void ADuCDefaultSetting(){};
	

	/**
	Last Message recieved. Its length as to be as the longest message.
	*/
	byte m_messageReceived[__UART_MAX_MESSAGE_LENGHT__];

	int m_messageReceivedLenght;

	byte m_packetsLost_01;
	byte m_packetsLost_02;
	byte m_packetsLost_03;
	
	SauPcProtocol *m_sauPcProtocol;

	ADuCsetting *m_ADuCsetting;
	ADuCsetting *m_ADuCsettingReadback;

	bool m_enFrequencyCal;
	int m_wantedSamplingFrq;
	unsigned long m_numOfPacketsReceived;
	int m_numOfSamplesPerPackets;
	unsigned long m_samplingFrqCalibrationTime; // in ms
	float m_calculatedSampligFrq;
	float m_percentErrorOnSamplingFrq;
	int m_correctedSamplingFrq;
	int m_samplingFrq;
	
private:
	/**
	Messages' fifo
	*/
	CSmallRingBuf<int , _stdUART_FIFOS_DEPTH_> m_message;

	/**
	last command sent to the uC
	*/
	Cmd_toADuC m_lastCmdSent;

	/**
	last command received from the uC
	*/
	Cmd_fromADuC m_lastCmdReceived;
	
	/**
	last uart hardware error.
	*/
	int m_lastHwError;

	/**
	Last error occured on the uC.
	*/
	dword m_lastADuC_commError;

	int m_lastNotAckReason;
	int m_lastADuCsettingReceived;
	int m_ADuCsetting_consistencyError;
	
	byte m_numSample_01_PacketsReceived;
	byte m_numSample_02_PacketsReceived;
	byte m_numSample_03_PacketsReceived;

	ADuC_states m_ADuCStates;
	word m_ADuCFwRevision;
	word m_ADuCHwRevision;
	char m_ADuCFwDate[__FW_NAME_DATE_MAX_LENGTH_];
	char m_ADuCFwName[__FW_NAME_DATE_MAX_LENGTH_];

	bool m_selfTestEnable;
    int m_selfTestTime;
	
	double m_powerVoltages[__MAX_POWER_VOLTAGES__];

	__sendingSettingStates m_sendingSettingState;

	int m_opCodeDataAnalyse;
	int m_lastRemoteIDDataAnalyse;
	int m_numStxNotRec;

};



/**
Class that manage the serial protocol to communicate with the ADuC7061 that samples the weight Channels (VOL and FLW)
*/
class ADuC_WEIGHT_protocol: public uC_Protocol
{
public:
	
	ADuC_WEIGHT_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);

	
	/**
	Sets the class that manages the weight samples.
	@param w poiter to the class GenericHwChanManager
	@param chan number of channel within w where add the sample
	*/
	void setWeightChan(GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *w, byte chan){ m_weightChan = w; m_chan = chan;};
	
protected:
	/**
	supporto messaggi variabili 
	*/
	virtual void ADuCDefaultSetting();
	virtual int samplePacket_01_decode();
	virtual int packetLost_01_handler();
	
private:

	GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *m_weightChan;
	byte m_chan;
};



/**
Class that manage the serial protocol to communicate with the ADuC7061 that samples the Pressure sensors (PRS)
*/
class ADuC_PRS_protocol: public uC_Protocol
{
public:
	
	ADuC_PRS_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);

	
	/**
	Sets the class that manages the prs samples.
	@param p poiter to the class PrsHwChanManager
	@param chan_A number of channel within p where add the sample
	@param chan_B number of channel within p where add the sample
	*/
	void setPrsChan(PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *p, int chan_A, int chan_B){ m_prsChan = p; m_chan_A = chan_A; m_chan_B = chan_B; };
	
protected:
	/**
	supporto messaggi variabili 
	*/
	virtual void ADuCDefaultSetting();
	virtual int samplePacket_01_decode();
	virtual int packetLost_01_handler();
	
private:
	PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *m_prsChan;
	int m_chan_A;
	int m_chan_B;
	//unsigned long m_test;
};


#define _ADuC_GEN_NUM_SAMPLES_PER_PACKET	20 /**< ADuC Generic channel: number of samples per packet per channel*/
/**
Class that manage the serial protocol to communicate with the ADuC7061 that samples the generic channels (GEN)
*/
class ADuC_GEN_protocol: public uC_Protocol
{
public:
	
	ADuC_GEN_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);
	
protected:
	/**
	supporto messaggi variabili 
	*/
	virtual void ADuCDefaultSetting();
	virtual int samplePacket_03_decode();
	virtual int packetLost_03_handler();
	
private:
	word m_packetDummy[_ADuC_GEN_NUM_SAMPLES_PER_PACKET*2];
};



#endif































