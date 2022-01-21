/**
@file		iWRAP_BLUEGIGA.h
@brief		Class to manage iWRAP bluegiga protocol
					
@author		Nicola
@date		13/11/2011
@version	01.00
*/




#ifndef __iWRAP_BLUEGIGA_H__
#define __iWRAP_BLUEGIGA_H__

#include "global.h"
#include "protocolStdUart.h"
#include "graph.h"
#include "sound.h"

#define _iWRAP_BUFFER_LENGHT_	100

const char str_CONNECT[] ="CONNECT";
const int str_CONNECT_lenght = (sizeof(str_CONNECT)-1)/sizeof(char);

const char str_INQUIRY_PARTIAL[] ="INQUIRY_PARTIAL";
const int str_INQUIRY_PARTIAL_lenght = (sizeof(str_INQUIRY_PARTIAL)-1)/sizeof(char);

const char str_NO_CARRIER[] ="NO CARRIER";
const int str_NO_CARRIER_lenght = (sizeof(str_NO_CARRIER)-1)/sizeof(char);

const char str_READY[] ="READY";
const int str_READY_lenght = (sizeof(str_READY)-1)/sizeof(char);

const char str_NAME[] ="NAME";
const int str_NAME_lenght = (sizeof(str_NAME)-1)/sizeof(char);

const char str_NAME_ERROR[] ="NAME ERROR";
const int str_NAME_ERROR_lenght = (sizeof(str_NAME_ERROR)-1)/sizeof(char);

const char str_PAIR[] ="PAIR";
const int str_PAIR_lenght = (sizeof(str_PAIR)-1)/sizeof(char);

const char str_RING[] ="RING";
const int str_RING_lenght = (sizeof(str_RING)-1)/sizeof(char);

const char str_SYNTAX_ERROR[] ="SYNTAX ERROR";
const int str_SYNTAX_ERROR_lenght = (sizeof(str_SYNTAX_ERROR)-1)/sizeof(char);

const char str_AUTH[] ="AUTH";
const int str_AUTH_lenght = (sizeof(str_AUTH)-1)/sizeof(char);

const char str_CALL[] ="CALL";
const int str_CALL_lenght = (sizeof(str_CALL)-1)/sizeof(char);

const char str_CLOSE[] ="CLOSE";
const int str_CLOSE_lenght = (sizeof(str_CLOSE)-1)/sizeof(char);

const char str_INQUIRY[] ="INQUIRY";
const int str_INQUIRY_lenght = (sizeof(str_INQUIRY)-1)/sizeof(char);

const char str_IC[] ="IC";
const int str_IC_lenght = (sizeof(str_IC)-1)/sizeof(char);

const char str_LIST[] ="LIST";
const int str_LIST_lenght = (sizeof(str_LIST)-1)/sizeof(char);

const char str_RESET[] ="RESET";
const int str_RESET_lenght = (sizeof(str_RESET)-1)/sizeof(char);

const char str_SELECT[] ="SELECT";
const int str_SELECT_lenght = (sizeof(str_SELECT)-1)/sizeof(char);

const char str_INFO[] ="INFO";
const int str_INFO_lenght = (sizeof(str_INFO)-1)/sizeof(char);

const char str_SET[] ="SET";
const int str_SET_lenght = (sizeof(str_SET)-1)/sizeof(char);

const char str_SET_PROFILE[] ="SET PROFILE";
const int str_SET_PROFILE_lenght = (sizeof(str_SET_PROFILE)-1)/sizeof(char);

const char str_SET_BT_BADDR[] ="SET BT BADDR";
const int str_SET_BT_BADDR_lenght = (sizeof(str_SET_BT_BADDR)-1)/sizeof(char);

const char str_SET_BT_NAME[] ="SET BT NAME";
const int str_SET_BT_NAME_lenght = (sizeof(str_SET_BT_NAME)-1)/sizeof(char);

const char str_SET_BT_CLASS[] ="SET BT CLASS";
const int str_SET_BT_CLASS_lenght = (sizeof(str_SET_BT_CLASS)-1)/sizeof(char);

const char str_SET_BT_LAP[] ="SET BT LAP";
const int str_SET_BT_LAP_lenght = (sizeof(str_SET_BT_LAP)-1)/sizeof(char);

const char str_SET_BT_AUTH[] ="SET BT AUTH";
const int str_SET_BT_AUTH_lenght = (sizeof(str_SET_BT_AUTH)-1)/sizeof(char);

const char str_SET_BT_PAIR[] ="SET BT AUTH";
const int str_SET_BT_PAIR_lenght = (sizeof(str_SET_BT_PAIR)-1)/sizeof(char);

const char str_SET_BT_PAGEMODE[] ="SET BT PAGEMODE";
const int str_SET_BT_PAGEMODE_lenght = (sizeof(str_SET_BT_PAGEMODE)-1)/sizeof(char);

const char str_SET_BT_ROLE[] ="SET BT ROLE";
const int str_SET_BT_ROLE_lenght = (sizeof(str_SET_BT_ROLE)-1)/sizeof(char);

const char str_SET_BT_SNIFF[] ="SET BT SNIFF";
const int str_SET_BT_SNIFF_lenght = (sizeof(str_SET_BT_SNIFF)-1)/sizeof(char);

const char str_SET_BT_SSP[] ="SET BT SSP";
const int str_SET_BT_SSP_lenght = (sizeof(str_SET_BT_SSP)-1)/sizeof(char);

const char str_SET_BT_POWER[] ="SET BT POWER";
const int str_SET_BT_POWER_lenght = (sizeof(str_SET_BT_POWER)-1)/sizeof(char);

const char str_SET_CONTROL_AUTOCALL[] ="SET CONTROL AUTOCALL";
const int str_SET_CONTROL_AUTOCALL_lenght = (sizeof(str_SET_CONTROL_AUTOCALL)-1)/sizeof(char);

const char str_SET_CONTROL_BAUD[] ="SET CONTROL BAUD";
const int str_SET_CONTROL_BAUD_lenght = (sizeof(str_SET_CONTROL_BAUD)-1)/sizeof(char);

const char str_SET_CONTROL_CD[] ="SET CONTROL CD";
const int str_SET_CONTROL_CD_lenght = (sizeof(str_SET_CONTROL_CD)-1)/sizeof(char);

const char str_SET_CONTROL_ECHO[] ="SET CONTROL ECHO";
const int str_SET_CONTROL_ECHO_lenght = (sizeof(str_SET_CONTROL_ECHO)-1)/sizeof(char);

const char str_SET_CONTROL_CONFIG[] ="SET CONTROL CONFIG";
const int str_SET_CONTROL_CONFIG_lenght = (sizeof(str_SET_CONTROL_CONFIG)-1)/sizeof(char);

const char str_SET_CONTROL_ESCAPE[] ="SET CONTROL ESCAPE";
const int str_SET_CONTROL_ESCAPE_lenght = (sizeof(str_SET_CONTROL_ESCAPE)-1)/sizeof(char);

const char str_SET_CONTROL_INIT[] ="SET CONTROL INIT";
const int str_SET_CONTROL_INIT_lenght = (sizeof(str_SET_CONTROL_INIT)-1)/sizeof(char);

const char str_SET_CONTROL_MUX[] ="SET CONTROL MUX";
const int str_SET_CONTROL_MUX_lenght = (sizeof(str_SET_CONTROL_MUX)-1)/sizeof(char);

const char str_SET_CONTROL_BIND[] ="SET CONTROL BIND";
const int str_SET_CONTROL_BIND_lenght = (sizeof(str_SET_CONTROL_BIND)-1)/sizeof(char);

const char str_SET_CONTROL_MSC[] ="SET CONTROL MSC";
const int str_SET_CONTROL_MSC_lenght = (sizeof(str_SET_CONTROL_MSC)-1)/sizeof(char);

const char str_TESTMODE[] ="TESTMODE";
const int str_TESTMODE_lenght = (sizeof(str_TESTMODE)-1)/sizeof(char);

const char str_BER[] ="BER";
const int str_BER_lenght = (sizeof(str_BER)-1)/sizeof(char);

const char str_RSSI[] ="RSSI";
const int str_RSSI_lenght = (sizeof(str_RSSI)-1)/sizeof(char);

const char str_TXPOWER[] ="TXPOWER";
const int str_TXPOWER_lenght = (sizeof(str_TXPOWER)-1)/sizeof(char);

const char str_SDP[] ="SDP";
const int str_SDP_lenght = (sizeof(str_SDP)-1)/sizeof(char);

const char str_SDP_ADD[] ="SDP ADD";
const int str_SDP_ADD_lenght = (sizeof(str_SDP_ADD)-1)/sizeof(char);

const char str_SLEEP[] ="SLEEP";
const int str_SLEEP_lenght = (sizeof(str_SLEEP)-1)/sizeof(char);

const char str_SCO_ENABLE[] ="SCO ENABLE";
const int str_SCO_ENABLE_lenght = (sizeof(str_SCO_ENABLE)-1)/sizeof(char);

const char str_SCO_OPEN[] ="SCO OPEN";
const int str_SCO_OPEN_lenght = (sizeof(str_SCO_OPEN)-1)/sizeof(char);

const char str_BOOT[] ="BOOT";
const int str_BOOT_lenght = (sizeof(str_BOOT)-1)/sizeof(char);

const char str_ECHO[] ="ECHO";
const int str_ECHO_lenght = (sizeof(str_ECHO)-1)/sizeof(char);

const char str_PING[] ="PING";
const int str_PING_lenght = (sizeof(str_PING)-1)/sizeof(char);

const char str_TEST[] ="TEST";
const int str_TEST_lenght = (sizeof(str_TEST)-1)/sizeof(char);

const char str_OK[] ="OK";
const int str_OK_lenght = (sizeof(str_OK)-1)/sizeof(char);



class iWRAP_BLUEGIGA_Protocol: public stdUartProtocolAbstraction
{
public:
	enum iWRAP_cmd
	{
		cmd_noCmd = 0,
		cmd_BER,
		cmd_CALL,
		cmd_CLOSE,
		cmd_HELP,
		cmd_INFO,
		cmd_INQUIRY,
		cmd_IC,
		cmd_LIST,
		cmd_NAME,
		cmd_PAIR,
		cmd_AUTH,
		cmd_RSSI,
		cmd_RESET,
		cmd_SCO,
		cmd_SCO_ENABLE,
		cmd_SCO_OPEN,
		cmd_SDP,
		cmd_SDP_ADD,
		cmd_SELECT,

		cmd_SET,
		
		cmd_SET_PROFILE,

		cmd_SET_BT_BADDR,
		cmd_SET_BT_NAME,
		cmd_SET_BT_CLASS,
		cmd_SET_BT_LAP,
		cmd_SET_BT_AUTH,
		cmd_SET_BT_PAIR, 
		cmd_SET_BT_PAGEMODE,
		cmd_SET_BT_ROLE,
		cmd_SET_BT_SNIFF,
		cmd_SET_BT_SSP,
		cmd_SET_BT_POWER,

		cmd_SET_CONTROL_AUTOCALL,
		cmd_SET_CONTROL_BAUD,
		cmd_SET_CONTROL_CD,
		cmd_SET_CONTROL_CONFIG,
		cmd_SET_CONTROL_ECHO,
		cmd_SET_CONTROL_ESCAPE,
		cmd_SET_CONTROL_INIT,
		cmd_SET_CONTROL_MUX,
		cmd_SET_CONTROL_BIND,
		cmd_SET_CONTROL_MSC,

		cmd_SET_LINK_ID_ACTIVE,
		cmd_SET_LINK_ID_MASTER,
		cmd_SET_LINK_ID_SLAVE,
		cmd_SET_LINK_ID_PARK,
		cmd_SET_LINK_ID_SNIFF,
		cmd_SET_LINK_ID_MSC,
		
		cmd_SLEEP,
		cmd_TESTMODE,
		cmd_TXPOWER,
		cmd_BCSP_ENABLE,
		cmd_BOOT,
		cmd_TEST,
		cmd_PING,
		cmd_ECHO,
		cmd_AT,
		cmd_ESCAPE_SEQUENCE,
	};

	enum iWRAP_msg
	{
		msg_noMsg = 0,
		evt_CONNECT,
		evt_INQUIRY_PARTIAL,
		evt_NO_CARRIER,
		evt_READY,	
		evt_RING,
		evt_NAME,
		evt_NAME_ERROR,
		evt_PAIR,
		evt_SYNTAX_ERROR,
		evt_AUTH,

		rsp_CALL,
		rsp_CLOSE,
		rsp_LIST,
		rsp_INQUIRY,
		rsp_SET_PROFILE,
		rsp_SET_BT_BADDR,
		rsp_SET_BT_NAME,
		rsp_SET_BT_CLASS,
		rsp_SET_BT_LAP,
		rsp_SET_BT_AUTH,
		rsp_SET_BT_PAIR,
		rsp_SET_BT_ROLE,
		rsp_SET_BT_SNIFF,
		rsp_SET_BT_SSP,
		rsp_SET_CONTROL_BAUD,
		rsp_SET_CONTROL_CD,	
		rsp_CONTROL_ECHO,
		rsp_SET_CONTROL_ESCAPE,
		rsp_SET_CONTROL_BIND,
		rsp_SET_CONTROL_MSC,
		rsp_TEST,
		rsp_BER,
		rsp_RSSI,
		rsp_TXPOWER,
		rsp_SDP,
		rsp_PING,
		rsp_OK,	
		rsp_UNKNOWN,
		

		msg_syntaxErrorSyncroError,
		msg_timoutRx,						/**< timeout RX elapsed*/
		msg_timoutTx,						/**< timeout TX elapsed*/
		msg_HwError,						/**< uart hardware error @see HwErrors*/
		msg_transmitBufferOverflowError,	/**< transmission buffer overflow error*/
		msg_receptionBufferOverflowError,	/**< reception buffer overflow error*/
		msg_dataReceivedBufferOverflow,		/**< data received buffer overflow errror*/	
		msg_unknownError					/**< unknown error*/
		
		
	};
	
	enum Uart_Hw_Errors
	{
		Uart_Hw_NoError 			= 0x00,
		Uart_Hw_ReceptionError 		= 0x01,
		Uart_Hw_ParityError    		= 0x02,
		Uart_Hw_FrameError 			= 0x04, 
		Uart_Hw_OverrunError 		= 0x08,
		Uart_Hw_NoDevice			= 0x10,
		Uart_Hw_ErrorUnknown		= 0x20,
		Uart_Hw_OverflowRxBuff		= 0x40,
		Uart_Hw_OverrunFpgaFifo		= 0x80
	};

	enum conenct_mode
	{
		cm_RFCOMM = 0,
		cm_HFP,
		cm_HFP_AG,
		cm_A2DP,
		cm_HID,
		cm_L2CAP
	};

	enum inquiryAccessCode_LAP
	{
		lap_none = 0,
		lap_GIAC = 0x9e8b33, /*< General/unlimimited Inquiry Access Code*/
		lap_LIAC = 0x9e8b00
	};

	enum profile_name
	{
		pn_HFP = 0, /*< Hands Free Profile*/
		pn_HFP_AG, /*< Hands Free Profile Audio gateway*/
		pn_SPP, 	/*< Serial Port Profile*/
		pn_OPP,  /*< Object Push Profile (server)*/
	};

	enum page_mode
	{
		pm_0 = 0, /*<iWRAP is NOT visible and inquiry does NOT answers call*/
		pm_1, 	/*<iWRAP is visible and inquiry does NOT answers call*/	
		pm_2, /*<iWRAP is NOT visible but inquiry answers call*/
		pm_3,  /*<iWRAP is  visible and inquiry answers call*/
		pm_4  /*< just like mode 3 if there are NO connection. If there are connection, it is like mode 0*/
	};

	enum page_scan_mode
	{
		psm_0 = 0, /*< mode R0 means that iWRAP IS connectable all the time, but NOT visible in inquiry*/
		psm_1,	   /*< mode R1 means that iWRAP IS connectable every 1.28 sec (the default value)*/
		psm_2,		 /*< mode R2 means that iWRAP IS connectable every 2.56 sec (lowest power consumption)*/
	};

	enum master_slave_policy
	{
		ms_policy_0 = 0, /*< this value allows master-slave switch when calling, but iWRAP does not request it when ansering (defalut value)*/
		ms_policy_1,	/*< this value allows master-slave switch when calling, but iWRAP  requests it when ansering */
		ms_policy_2,	/*< if this value is set, master-slave policy is not allowed when calling, but is request when answering*/
	};

	enum spp_capabilities
	{
		spp_cap_displayOnly = 0,
		spp_cap_displayAndYesNoButton,
		spp_cap_keyboardOnly,
		spp_cap_none,
	};

	enum spp_mitm
	{
		mitm_protectionDisable = 0,
		mitm_protectionEnable,
	};
	
	enum link_policy
	{
		lp_roleSwitch = 0x01,
		lp_holdMode	  = 0x02,
		lp_SniffMode  = 0x04,
		lp_ParkState  = 0x08,
		lp_allModesEnabled   = 0x0F,
		lp_allModeDisable	 = 0x00,
	};

	enum baud_rate
	{
		br_9600,
		br_19200,
		br_38400,
		br_115200,
		br_230400,
		br_460800,
	};

	enum parity
	{
		parity_none = 0,
		parity_even,
		parity_odd,
	};

	enum stop_bits
	{
		stop_bits_1 = 1,
		stop_bits_2,	
	};

	enum configuration_values
	{
		conf_RSSI_visible_in_inquiry = 0x0001,
		conf_interlaced_inquiry_scan = 0x0004,
		conf_interlace_page_scan = 0x0008,
		conf_deep_sleep_enabled = 0x0010,
		conf_bluetooth_addres_in_CONNECT = 0x0020,
		conf_display_PAIR_event = 0x0080,
		conf_enable_SCO_links = 0x0100,
		conf_enable_interactive_pairing = 0x0800,
		conf_paring_ramdomly_replaced = 0x1000,
		
	};

	enum echo_mask
	{
		echo_startup_banner_visible = 0x01,
		echo_char_backed_to_client = 0x02,
		echo_display_event = 0x04,
	};

	enum dtr_mode
	{
		dtr_disable = 0,
		dtr_back_to_command_mode_when_dropped,
		dtr_close_active_connection_when_dropped,
		dtr_reset_iWRAP_when_dropped,
	};

	enum direction
	{
		dir_RISE = 0,
		dir_FALL,
		dir_CHANGE,
	};

	enum msc_mode
	{
		msc_DTE = 0,
		msc_nDTE,	
		msc_DCE,
		msc_nDCE
	};

	enum boot_mode
	{
		boot_iWRAP = 0x0000,
		boot_HCI_BCSP_1158000_8n1 = 0x0001,
		boot_HCI_USB = 0x0003,
		boot_HCI_H4_115200_8n1 = 0x0004,
	};

	typedef struct
	{
		int code;
		char par[_iWRAP_BUFFER_LENGHT_];
		int len;
	}iWRAP_msg_struct;

	
	iWRAP_BLUEGIGA_Protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = 200);
	~iWRAP_BLUEGIGA_Protocol();


	virtual void manager();
	
	virtual void TurnOn(){stdUartProtocolAbstraction::TurnOn();}
	virtual void TurnOff(){stdUartProtocolAbstraction::TurnOff();}

	void setUartDevice(UartDeviceAbstract *__pUartDevice){m_pUart = __pUartDevice;}
	void SetBaudRate(UartDeviceAbstract::UARTBaudRate _br){m_pUart->SetBaudRate(_br, true);}
	void SetHwFlwCtl(UartDeviceAbstract::UARThwFlwCnt flwCtl){m_pUart->SetHwFlwCtl( flwCtl, true);}

	virtual bool getMsg(iWRAP_msg_struct &msg);

	iWRAP_cmd getLastCmdSent(){ return m_lastCmdSent;};


	/**
	@return the last uart harware error
	*/
	byte getLastHwError(){return m_lastHwError;};
	/**
	resets the last uart hardware error
	*/
	void rstLastHwError(){m_lastHwError = Uart_Hw_NoError;};

	int CALL(const char *_address, const char* _target, conenct_mode _cm, int MTU = -1, int _uart_timeout = 5000);

	int CLOSE(int _link_id, int _uart_timeout = 5000);
	
	int INQUIRY(int _timeout, bool _name, inquiryAccessCode_LAP _lap = lap_none, int _uart_timeout = 5000);

	int IC(int _uart_timeout = 5000);

	int LIST(int _uart_timeout = 5000);

	int NAME(const char* _address , int _uart_timeout = 5000);

	int PAIR(const char* _address , int _uart_timeout = 5000);
	
	int RESET(int timeout = 0);

	int SELECT(int _link_id, int _uart_timeout = 5000);

	int INFO(int _uart_timeout = 5000);

	int AUTH(const char* _bt_address, int _uart_timeout = 5000);

	int SET(int _uart_timeout = 5000);
	
	int SET_PROFILE(profile_name _pn, const char* _name, int _uart_timeout = 5000);

	int SET_BT_BDADDR(int _uart_timeout = 5000);

	int SET_BT_NAME(const char* _name, int _uart_timeout = 5000);

	int SET_BT_CLASS(const char* _class_of_device, int _uart_timeout = 5000);

	int SET_BT_LAP(inquiryAccessCode_LAP _lap, int _uart_timeout = 5000);

	int SET_BT_AUTH(const char* _pin_code, int _uart_timeout = 5000);

	int SET_BT_PAIR(const char *_bd_addr, const char* _link_key, int _uart_timeout = 5000);

	int SET_BT_PAGEMODE(page_mode _pm, int _page_timeout, page_scan_mode _scan_mode, int _uart_timeout = 5000 );

	int SET_BT_ROLE(master_slave_policy _ms_policy, link_policy _lk_policy, int _supervision_timeout, int _uart_timeout = 5000);

	int SET_BT_SNIFF(int _max, int _min, int _attempt, int _timeout, int _uart_timeout = 5000);

	int SET_BT_SNIFF(int _avg, int _uart_timeout = 5000);

	int SET_BT_SSP(spp_capabilities _capability, spp_mitm _mitm, int _uart_timeout = 5000);
	
	int SET_BT_POWER(int _default, int _maximum, bool _reset = false, int _uart_timeout = 5000 );
	
	int SET_CONTROL_AUTOCALL(const char* _target, int _timeout, const char *_profile, int _uart_timeout = 5000);

	int SET_CONTROL_BAUD(baud_rate _br, parity _parity, stop_bits _st_bit, int _uart_timeout = 5000);

	int SET_CONTROL_CD(byte _cd_mask, bool _data_mode, int _uart_timeout = 5000);

	int SET_CONTROL_CONFIG(configuration_values _config, int _uart_timeout = 5000);

	int SET_CONTROL_ECHO(echo_mask _echo, int _uart_timeout = 5000);

	int SET_CONTROL_ESCAPE(char _esc_char, word _dtr_mask, dtr_mode _mode, int _uart_timeout = 5000);

	int SET_CONTROL_INIT(const char *_command, int _uart_timeout = 5000);

	int SET_CONTROL_MUX(bool enable, int _uart_timeout = 5000);

	int SET_CONTROL_BIND(byte _priority, word _io_mask, direction _dir, const char *_command, int _uart_timeout = 5000);

	int SET_CONTROL_MSC(msc_mode _mode, word _DSR_mask, word _DTR_mask, word _RTS_mask, word _CTS_mask, word _RI_mask, word _DCD_mask, int _uart_timeout = 5000);
	
	int SET_LINK_ID_ACTIVE(int _link_id, int _uart_timeout = 5000);

	int SET_LINK_ID_MASTER(int _link_id, int _uart_timeout = 5000);
	
	int SET_LINK_ID_SLAVE(int _link_id, int _uart_timeout = 5000);

	int SET_LINK_ID_PARK(int _link_id, int _max, int _min, int _uart_timeout = 5000);

	int SET_LINK_ID_PARK(int _link_id, int _avg, int _uart_timeout = 5000);

	int SET_LINK_ID_SNIFF(int _link_id, int _max, int _min, int _attempt = -1, int _timeout = -1, int _uart_timeout = 5000);

	int SET_LINK_ID_SNIFF(int _link_id, int _avg, int _uart_timeout = 5000);

	int SET_LINK_ID_MSC(int _link_id, const char* _status, int _uart_timeout = 5000);

	int TESTMODE(int _uart_timeout = 5000);

	int BER(int _link_id, int _uart_timeout = 5000);

	int RSSI(int _link_id, int _uart_timeout = 5000);
	
	int TXPOWER(int _link_id, int _uart_timeout = 5000);

	int SDP(const char *_bd_addr, const char *_Uuid, int _uart_timeout = 5000);

	int SDP_ADD(const char *_Uuid, const char* _name, int _uart_timeout = 5000);

	int SLEEP(int _uart_timeout = 5000);
	
	int SCO_ENABLE(int _uart_timeout = 5000);

	int SCO_OPEN(int _link_id, int _uart_timeout = 5000);

	int BOOT(boot_mode _mode, int _uart_timeout = 5000);

	int ECHO(int _link_id, const char* _string, int _uart_timeout = 5000);

	int PING(int _link_id, int _uart_timeout = 5000);

	int TEST(const char* _test_cmd, int _uart_timeout = 5000);

	int AT(int _uart_timeout = 5000);

	int ESCAPE_SEQUENCE(int _uart_timeout = 0);

	void clearTx(){stdUartProtocolAbstraction::clearTx();}
	void clearRx(){stdUartProtocolAbstraction::clearRx();}
	int popBlueError(){return stdUartProtocolAbstraction::popError();}
	
protected:
	virtual bool DataAnalyse();

	virtual int sendMsg(iWRAP_msg_struct msg);
	virtual int sendMsg(int i);

	void waitSyntaxErrorSyncroError(int i);
	void checkSyntaxErrorTimer();
	
private:
	/**
	Messages' fifo
	*/
	CSmallRingBuf<iWRAP_msg_struct , _stdUART_FIFOS_DEPTH_> m_message;

	CSmallRingBuf<iWRAP_msg_struct , _stdUART_FIFOS_DEPTH_> m_waitingMessage;

	byte m_messageReceived[_iWRAP_BUFFER_LENGHT_];

	int m_messageReceivedLenght;

	iWRAP_msg m_decodingMsg;
	
	iWRAP_cmd m_lastCmdSent;

	byte m_lastHwError;

	RingBuffer *m_ringBuffer;
	
	enum defaults
	{
		_WAIT_SYNTAX_ERROR_TIME_ = 100,
	};
	
	DecTimer m_waitSyntaxErrorTimer;
	int m_numBypassByteTransmittedDataAnalyse;
};



#endif
















