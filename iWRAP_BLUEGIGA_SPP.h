/**
@file		iWRAP_BLUEGIGA_SPP.h
@brief		Class to manage iWRAP bluegiga SSP profile
					
@author		Nicola
@date		19/11/2011
@version	01.00
*/



#ifndef _iWRAP_BLUEGIGA_SPP_H_
#define _iWRAP_BLUEGIGA_SPP_H_




#include "iWRAP_BLUEGIGA.h"


class iWRAP_BLUEGIGA_SPP: protected iWRAP_BLUEGIGA_Protocol
{
public:
	typedef struct
	{
		int code;
		char par[_iWRAP_BUFFER_LENGHT_];
		int len;
		
	}iWRAP_SSP_msg_struct;
	
	enum iWRAP_SSP_msg
	{
		msg_noMsg = 0,
		msg_CONNECTED,
		msg_DISCONNECTED,

		msg_INFO,
		msg_SET_BT_NAME,
		msg_SET_BT_PROFILE,
		msg_SET_BT_AUTH,
		msg_SET_BT_SSP,
		msg_SET_BT_BAUD,
		msg_SET_CONTROL_MSC,
		msg_RING,
		msg_CALL,

		msg_moduleNotResponding,
		msg_syntaxErrorSyncroError,
		msg_timoutRx,						/**< timeout RX elapsed*/
		msg_timoutTx,						/**< timeout TX elapsed*/
		msg_HwError,						/**< uart hardware error @see HwErrors*/
		msg_transmitBufferOverflowError,	/**< transmission buffer overflow error*/
		msg_receptionBufferOverflowError,	/**< reception buffer overflow error*/
		msg_dataReceivedBufferOverflow,		/**< data received buffer overflow errror*/	
		msg_unknownError					/**< unknown error*/
		
		
	};
	iWRAP_BLUEGIGA_SPP(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = 200);
	~iWRAP_BLUEGIGA_SPP();

	virtual void manager();

	virtual void TurnOn(){iWRAP_BLUEGIGA_Protocol::TurnOn();}
	virtual void TurnOff(){iWRAP_BLUEGIGA_Protocol::TurnOff();}

	void setUartDevice(UartDeviceAbstract *__pUartDevice){iWRAP_BLUEGIGA_Protocol::setUartDevice(__pUartDevice);}
	void SetBaudRate(UartDeviceAbstract::UARTBaudRate _br){iWRAP_BLUEGIGA_Protocol::SetBaudRate(_br);}
	void SetHwFlwCtl(UartDeviceAbstract::UARThwFlwCnt flwCtl){iWRAP_BLUEGIGA_Protocol::SetHwFlwCtl(flwCtl);}
	void setBypassMode(bool active , UartDeviceAbstract *__pUartDevice){ stdUartProtocolAbstraction::setBypassMode(active, __pUartDevice);}
	
	bool getMsg(iWRAP_SSP_msg_struct &msg);

	int VERIFY_BAUD_RATE();

	int CALL(char *_address);

	int CLOSE();

	int RESET();

	int INFO();

	int SET_BT_NAME();

	int SET_PROFILE();
	
	int SET_BT_AUTH();

	int SET_BT_SSP();
	
	int SET_BT_POWER();

	int SET_CONTROL_MSC();

	int BER();

	int RSSI();
	
	int TXPOWER();

	bool strMsg(char *msg, int &len);

	void clearTx(){iWRAP_BLUEGIGA_Protocol::clearTx();}
	void clearRx(){iWRAP_BLUEGIGA_Protocol::clearRx();}
	
protected:
	enum states 
	{
		_idle,
		_setting_baud,
		_setting_profile,
		_setting_bt_name,
		_setting_auth,
		_setting_msc,
		_setting_ssp,
		_asking_info,
		_calling,
		_closing,
	};

	enum setting_baud_states
	{
		sbaud_send_AT,
		sbaud_waiting_AT,
		sbaud_send_AT_2,
		sbaud_waiting_AT_2,
		sbaud_set_BAUD,
		sbaud_waiting_BAUD,
		sbaud_send_escapeSequence,
		sbaud_waiting_escapeSequence,
		sbaud_BAUD_set,
	};

	enum setting_profile_states
	{
		sprofile_setting = 0,
		sprofile_waiting,
		sprofile_set,
	};
	
	enum asking_info_states
	{
		askingInfo_ask,
		askingInfo_waiting,
		askingInfo_received,
	};

	enum sBT_name_states
	{
		sBTname_send_NAME,
		sBTname_waiting,
		sBTname_NAME_set,
	};

	enum sBT_auth_states
	{
		sBTauth_send_AUTH,
		sBTauth_waiting,
		sBTauth_AUTH_set,
	};

	enum sBT_MSC_states
	{
		sBTmsc_send_MSC,
		sBTmsc_waiting,
		sBTmsc_MSC_set,
	};
	
	enum sBT_ssp_states
	{
		sBTssp_send_SSP,
		sBTssp_waiting,
		sBTssp_SSP_set,
	};

	enum calling_states
	{
		calling_LIST,
		calling_waiting_LIST,
		calling_escapeSequence,
		calling_waiting_escapeSequence,
		calling_CLOSE,
		calling_waiting_CLOSE,
		calling_CALL,
		calling_waiting_CALL,
		calling_CALLED,
		calling_NO_CARRIER,
	};

	enum closing_states
	{
		closing_LIST,
		closing_waiting_LIST,
		closing_escapeSequence,
		closing_waiting_escapeSequence,
		closing_CLOSE,
		closing_waiting_CLOSE,
		closing_CLOSED,
	};
	
	
	inline int iWRAP_evt_CONNECT_handler(char *par,  int len);
	inline int iWRAP_evt_INQUIRY_PARTIAL_handler(char *par,  int len);
	inline int iWRAP_evt_NO_CARRIER_handler(char *par,  int len);
	inline int iWRAP_evt_READY_handler(char *par,  int len);
	inline int iWRAP_evt_NAME_handler(char *par,  int len);
	inline int iWRAP_evt_NAME_ERROR_handler(char *par,  int len);
	inline int iWRAP_evt_RING_handler(char *par,  int len);
	inline int iWRAP_evt_PAIR_handler(char *par,  int len);
	inline int iWRAP_evt_SYNTAX_ERROR_handler(char *par,  int len);
	inline int iWRAP_evt_AUTH_handler(char *par,  int len);
	inline int iWRAP_rsp_CALL_handler(char *par,  int len);
	inline int iWRAP_rsp_INQUIRY_handler(char *par,  int len);
	inline int iWRAP_rsp_LIST_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_PROFILE_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_BADDR_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_NAME_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_CLASS_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_LAP_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_AUTH_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_PAIR_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_ROLE_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_SNIFF_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_BT_SSP_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_CONTROL_BAUD_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_CONTROL_CD_handler(char *par,  int len);
	inline int iWRAP_rsp_CONTROL_ECHO_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_CONTROL_ESCAPE_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_CONTROL_BIND_handler(char *par,  int len);
	inline int iWRAP_rsp_SET_CONTROL_MSC_handler(char *par,  int len);
	inline int iWRAP_rsp_TEST_handler(char *par,  int len);
	inline int iWRAP_rsp_BER_handler(char *par,  int len);
	inline int iWRAP_rsp_RSSI_handler(char *par,  int len);
	inline int iWRAP_rsp_TXPOWER_handler(char *par,  int len);
	inline int iWRAP_rsp_SDP_handler(char *par,  int len);
	inline int iWRAP_rsp_PING_handler(char *par,  int len);
	inline int iWRAP_rsp_UNKNOWN_handler(char *par,  int len);
	
	inline int iWRAP_msg_timoutRx_handler(char *par,  int len);
	inline int iWRAP_msg_timoutTx_handler(char *par,  int len);
	inline int iWRAP_msg_HwError_handler(char *par,  int len);
	inline int iWRAP_msg_transmitBufferOverflowError_handler(char *par,  int len);
	inline int iWRAP_msg_receptionBufferOverflowError_handler(char *par,  int len);
	inline int iWRAP_msg_dataReceivedBufferOverflow_handler(char *par,  int len);
	inline int iWRAP_rsp_OK_handler(char *par,  int len);
	inline int iWRAP_msg_unknownError_handler(char *par,  int len);
	inline int iWRAP_msg_syntaxErrorSyncroError_handler(char *par,  int len);
	
	int iWRAP_timeout_and_error_handler(char *par,  int len,  bool isSyntaxError);

	inline void _idle_handler();
	inline void _setting_baud_handler();
	inline void _setting_profile_handler();
	inline void _setting_bt_name_handler();
	inline void _setting_auth_handler();
	inline void _setting_ssp_handler();
	inline void _setting_msc_handler();
	inline void _asking_info_handler();
	inline void _calling_handler();
	inline void _closing_handler();

	int sendMsg(iWRAP_SSP_msg_struct msg);
	int sendMsg(int i);

	byte changeStatus(states state);

	void reset_machinesState();
	
private:

	CSmallRingBuf<iWRAP_SSP_msg_struct , _stdUART_FIFOS_DEPTH_> m_message;

	CSmallRingBuf<iWRAP_msg_struct, _stdUART_FIFOS_DEPTH_> m_strMsg;

	CSmallRingBuf<int, 10> m_listedLinkId;
	
	iWRAP_msg_struct m_iWRAP_msg;
	
	states m_currentState;
	states m_previousState;

	setting_baud_states m_sbaudState;
	asking_info_states m_askingInfoState;
	setting_profile_states m_settingProfile_state;
	sBT_name_states m_sBT_name_state;
	sBT_auth_states m_sBT_auth_state;
	sBT_ssp_states m_sBT_ssp_state;
	sBT_MSC_states m_sBT_MSC_state;
	calling_states m_calling_state;
	closing_states m_closing_state;
	
	long m_timeoutErrors;

	int m_LIST_num;
	int m_LIST_count;
	
	char m_callingAddress[25];

	DecTimer m_timeout;
};















#endif



