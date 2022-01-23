/**
@file		iWRAP_BLUEGIGA_SPP.CPP
@brief		Class to manage iWRAP bluegiga SSP profile
					
@author		Nicola
@date		19/11/2011
@version	01.00
*/



#include "iWRAP_BLUEGIGA_SPP.h"



iWRAP_BLUEGIGA_SPP::iWRAP_BLUEGIGA_SPP(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize ): iWRAP_BLUEGIGA_Protocol(__pUartDevice, __rxProtoBufSize)
{
	
	m_currentState = _idle;
	m_previousState = _idle;

	m_sbaudState = sbaud_send_AT;
	m_askingInfoState = askingInfo_ask;
	m_sBT_name_state = sBTname_send_NAME;
	m_sBT_auth_state = sBTauth_send_AUTH;
	m_sBT_ssp_state = sBTssp_send_SSP;
	m_sBT_MSC_state = sBTmsc_send_MSC;
	m_calling_state = calling_LIST;
	m_closing_state = closing_LIST;
	m_settingProfile_state = sprofile_setting;
	
	m_LIST_num = 0;
	m_LIST_count = 0;
	m_timeoutErrors = 0;
	m_listedLinkId.clear();
	m_timeout.Stop();
}

iWRAP_BLUEGIGA_SPP::~iWRAP_BLUEGIGA_SPP()
{

}

void iWRAP_BLUEGIGA_SPP::manager()
{
	/*static bool first = true;

	if(first)
	{
		iWRAP_BLUEGIGA_Protocol::SET_CONTROL_BAUD(br_115200, parity_none, stop_bits_1, 2000);
		first = false;
	}*/
	iWRAP_BLUEGIGA_Protocol::manager();

	iWRAP_BLUEGIGA_Protocol::iWRAP_msg_struct msg;
			
	while (iWRAP_BLUEGIGA_Protocol::getMsg(msg))
	{
		
		switch (msg.code)
		{
			case iWRAP_BLUEGIGA_Protocol::evt_CONNECT:
				iWRAP_evt_CONNECT_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_INQUIRY_PARTIAL:
				iWRAP_evt_INQUIRY_PARTIAL_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_NO_CARRIER:
				iWRAP_evt_NO_CARRIER_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_READY:
				iWRAP_evt_READY_handler(msg.par, msg.len);
				break;					
			case iWRAP_BLUEGIGA_Protocol::evt_NAME:
				iWRAP_evt_NAME_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_RING:
				iWRAP_evt_RING_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_NAME_ERROR:
				iWRAP_evt_NAME_ERROR_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_PAIR:
				iWRAP_evt_PAIR_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_SYNTAX_ERROR:
				iWRAP_evt_SYNTAX_ERROR_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::evt_AUTH:
				iWRAP_evt_AUTH_handler(msg.par, msg.len);
				break;

			case iWRAP_BLUEGIGA_Protocol::rsp_CALL:
				iWRAP_rsp_CALL_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_INQUIRY:
				iWRAP_rsp_INQUIRY_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_LIST:
				iWRAP_rsp_LIST_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_PROFILE:
				iWRAP_rsp_SET_PROFILE_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_BADDR:
				iWRAP_rsp_SET_BT_BADDR_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_NAME:
				iWRAP_rsp_SET_BT_NAME_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_CLASS:
				iWRAP_rsp_SET_BT_CLASS_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_LAP:
				iWRAP_rsp_SET_BT_LAP_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_AUTH:
				iWRAP_rsp_SET_BT_AUTH_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_PAIR:
				iWRAP_rsp_SET_BT_PAIR_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_ROLE:
				iWRAP_rsp_SET_BT_ROLE_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_SNIFF:
				iWRAP_rsp_SET_BT_SNIFF_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_BT_SSP:
				iWRAP_rsp_SET_BT_SSP_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_CONTROL_BAUD:
				iWRAP_rsp_SET_CONTROL_BAUD_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_CONTROL_CD:
				iWRAP_rsp_SET_CONTROL_CD_handler(msg.par, msg.len);
				break;	
			case iWRAP_BLUEGIGA_Protocol::rsp_CONTROL_ECHO:
				iWRAP_rsp_CONTROL_ECHO_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_CONTROL_ESCAPE:
				iWRAP_rsp_SET_CONTROL_ESCAPE_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_CONTROL_BIND:
				iWRAP_rsp_SET_CONTROL_BIND_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SET_CONTROL_MSC:
				iWRAP_rsp_SET_CONTROL_MSC_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_TEST:
				iWRAP_rsp_TEST_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_BER:
				iWRAP_rsp_BER_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_RSSI:
				iWRAP_rsp_RSSI_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_TXPOWER:
				iWRAP_rsp_TXPOWER_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_SDP:
				iWRAP_rsp_SDP_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_PING:
				iWRAP_rsp_PING_handler(msg.par, msg.len);
				break;
			case  iWRAP_BLUEGIGA_Protocol::rsp_UNKNOWN:
				iWRAP_rsp_UNKNOWN_handler(msg.par, msg.len);
				break;
				
			case iWRAP_BLUEGIGA_Protocol::msg_timoutRx:
				iWRAP_msg_timoutRx_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_timoutTx:
				iWRAP_msg_timoutTx_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_HwError:
				iWRAP_msg_HwError_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_transmitBufferOverflowError:
				iWRAP_msg_transmitBufferOverflowError_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_receptionBufferOverflowError:
				iWRAP_msg_receptionBufferOverflowError_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_dataReceivedBufferOverflow:
				iWRAP_msg_dataReceivedBufferOverflow_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::rsp_OK:
				iWRAP_rsp_OK_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_unknownError:
				iWRAP_msg_unknownError_handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_Protocol::msg_syntaxErrorSyncroError:
				iWRAP_msg_syntaxErrorSyncroError_handler(msg.par, msg.len);
				break;
		}
		if(!m_strMsg.full())
		{
			m_strMsg.push_back(msg);
		}
	}

	if (m_timeout.Match())
	{
		m_timeout.Stop();
		reset_machinesState();

		sendMsg(msg_moduleNotResponding);
	}
	
	//--- machine state ---
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			_setting_baud_handler();
			break;
		case _setting_profile:
			_setting_profile_handler();
			break;
		case _setting_bt_name:
			_setting_bt_name_handler();
			break;
		case _setting_auth:
			_setting_auth_handler();
			break;
		case _setting_ssp:
			_setting_ssp_handler();
			break;
		case _setting_msc:
			_setting_msc_handler();
			break;
		case _asking_info:
			_asking_info_handler();
			break;
		case _calling:
			_calling_handler();
			break;
		case _closing:
			_closing_handler();
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::reset_machinesState()
{
	m_currentState = _idle;
	m_sbaudState = sbaud_send_AT;
	m_askingInfoState = askingInfo_ask;
	m_sBT_name_state = sBTname_send_NAME;
	m_sBT_auth_state = sBTauth_send_AUTH;
	m_sBT_ssp_state = sBTssp_send_SSP;
	m_sBT_MSC_state = sBTmsc_send_MSC;
	m_calling_state = calling_LIST;
	m_closing_state = closing_LIST;
	m_settingProfile_state = sprofile_setting;
	m_timeoutErrors = 0;
	m_timeout.Stop();
	
}

void iWRAP_BLUEGIGA_SPP::_idle_handler()
{
	
}

void iWRAP_BLUEGIGA_SPP::_setting_baud_handler()
{
	switch(m_sbaudState)
	{
		case sbaud_send_AT:
			m_timeout.Preset(5000);
			//SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
			SetBaudRate(UartDeviceAbstract::BR460800);
			iWRAP_BLUEGIGA_Protocol::AT(500);
			m_sbaudState = sbaud_waiting_AT;
			break;
		case sbaud_waiting_AT:
			break;

		case sbaud_send_AT_2:
			m_timeout.Preset(5000);
			//SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
			SetBaudRate(UartDeviceAbstract::BR115200);
			iWRAP_BLUEGIGA_Protocol::AT(500);
			goBuzzer(KEY_LIKE);
			m_sbaudState = sbaud_waiting_AT_2;
			break;
		case sbaud_waiting_AT_2:
			break;	
			
		case sbaud_set_BAUD:
			m_timeout.Preset(10000);
			iWRAP_BLUEGIGA_Protocol::SET_CONTROL_BAUD(br_460800, parity_none, stop_bits_1, 2000);
			m_sbaudState = sbaud_waiting_BAUD;
			break;
		case sbaud_waiting_BAUD:
			break;
			
		case sbaud_send_escapeSequence:
			m_timeout.Preset(15000);
			iWRAP_BLUEGIGA_Protocol::ESCAPE_SEQUENCE(5000);
			m_sbaudState = sbaud_waiting_escapeSequence;
			break;
		case sbaud_waiting_escapeSequence:
			break;
			
		case sbaud_BAUD_set:
			m_timeout.Stop();
			sendMsg(msg_SET_BT_BAUD);
			m_sbaudState = sbaud_send_AT;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_setting_profile_handler()
{
	switch(m_settingProfile_state)
	{
		case sprofile_setting:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::SET_PROFILE(pn_SPP, "ON", 1000);
			m_settingProfile_state = sprofile_waiting;
			break;
		case sprofile_waiting:
			break;
		case sprofile_set:
			m_timeout.Stop();
			sendMsg(msg_SET_BT_PROFILE);
			m_settingProfile_state = sprofile_setting;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_setting_bt_name_handler()
{
	switch(m_sBT_name_state)
	{
		case sBTname_send_NAME:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::SET_BT_NAME("BT-Menfis SAU", 1000);
			m_sBT_name_state = sBTname_waiting;
			break;
		case sBTname_waiting:
			break;
		case sBTname_NAME_set:
			m_timeout.Stop();
			sendMsg(msg_SET_BT_NAME);
			m_sBT_name_state = sBTname_send_NAME;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_setting_auth_handler()
{
	switch(m_sBT_auth_state)
	{
		case sBTauth_send_AUTH:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::SET_BT_AUTH("menfis", 1000);
			m_sBT_auth_state = sBTauth_waiting;
			break;
		case sBTauth_waiting:
			break;
		case sBTauth_AUTH_set:
			m_timeout.Stop();
			sendMsg(msg_SET_BT_AUTH);
			m_sBT_auth_state = sBTauth_send_AUTH;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_asking_info_handler()
{
	switch (m_askingInfoState)
	{
		case askingInfo_ask:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::INFO(1000);
			m_askingInfoState = askingInfo_waiting;
			break;
		case askingInfo_waiting:

			break;
		case askingInfo_received:
			m_timeout.Stop();
			sendMsg(msg_INFO);
			m_askingInfoState = askingInfo_ask;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_setting_ssp_handler()
{
	switch(m_sBT_ssp_state)
	{
		case sBTssp_send_SSP:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::SET_BT_SSP(iWRAP_BLUEGIGA_Protocol::spp_cap_none, iWRAP_BLUEGIGA_Protocol::mitm_protectionDisable, 1000);
			m_sBT_ssp_state = sBTssp_SSP_set;//sBTssp_waiting;
			break;
		case sBTssp_waiting:
			break;
		case sBTssp_SSP_set:
			m_timeout.Stop();
			sendMsg(msg_SET_BT_SSP);
			m_sBT_ssp_state = sBTssp_send_SSP;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_setting_msc_handler()
{
	switch(m_sBT_MSC_state)
	{
		case sBTmsc_send_MSC:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::SET_CONTROL_MSC(msc_nDTE, 0x00, 0x00, /*_RTS_mask*/0x04 , /*_CTS_mask*/ 0x08, 0x00, 0x00, 1000);
			m_sBT_MSC_state = sBTmsc_waiting;
			break;
		case sBTmsc_waiting:
			break;
		case sBTmsc_MSC_set:
			m_timeout.Stop();
			sendMsg(msg_SET_CONTROL_MSC);
			m_sBT_MSC_state = sBTmsc_send_MSC;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_calling_handler()
{
	int link_id;
	switch(m_calling_state)
	{
		case calling_LIST:
			m_timeout.Preset(10000);
			m_listedLinkId.clear();
			m_LIST_count = 0;
			m_LIST_num = 0;
			iWRAP_BLUEGIGA_Protocol::LIST(1000);
			m_calling_state = calling_waiting_LIST;
			break;
		case calling_waiting_LIST:

			break;
		case calling_escapeSequence:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::ESCAPE_SEQUENCE(5000);
			m_calling_state = calling_waiting_escapeSequence;
			break;
		case calling_waiting_escapeSequence:

			break;
		case calling_CLOSE:
			m_timeout.Preset(5000);
			if(!m_listedLinkId.empty())
			{
				m_listedLinkId.pop_front(link_id);
				iWRAP_BLUEGIGA_Protocol::CLOSE(link_id, 2000);
				m_calling_state =  calling_waiting_CLOSE;
			}else
			{
				m_calling_state = calling_CALL;
			}
			break;
		case calling_waiting_CLOSE:

			break;
		case calling_CALL:
			m_timeout.Preset(20000);
			iWRAP_BLUEGIGA_Protocol::CALL(m_callingAddress, "1101", iWRAP_BLUEGIGA_Protocol::cm_RFCOMM, -1, 15000);
			m_calling_state = calling_waiting_CALL;
			break;
		case calling_waiting_CALL:

			break;
		case calling_CALLED:
			m_timeout.Stop();
			sendMsg(msg_CONNECTED);
			m_calling_state = calling_LIST;
			changeStatus(_idle);
			break;
			
		case calling_NO_CARRIER:
			m_timeout.Stop();
			sendMsg(msg_DISCONNECTED);
			m_calling_state = calling_LIST;
			changeStatus(_idle);
			break;
	}
}

void iWRAP_BLUEGIGA_SPP::_closing_handler()
{
	int link_id;
	
	switch(m_closing_state)
	{
		case closing_LIST:
			m_timeout.Preset(10000);
			m_listedLinkId.clear();
			m_LIST_count = 0;
			m_LIST_num = 0;
			iWRAP_BLUEGIGA_Protocol::LIST(1000);
			m_closing_state = closing_waiting_LIST;
			break;
		case closing_waiting_LIST:
			break;
		case closing_escapeSequence:
			m_timeout.Preset(5000);
			iWRAP_BLUEGIGA_Protocol::ESCAPE_SEQUENCE(5000);
			m_closing_state = closing_waiting_escapeSequence;
			break;
		case closing_waiting_escapeSequence:
			break;
		case closing_CLOSE:
			m_timeout.Preset(5000);
			if(!m_listedLinkId.empty())
			{
				m_listedLinkId.pop_front(link_id);
				iWRAP_BLUEGIGA_Protocol::CLOSE(link_id, 2000);
				m_closing_state =  closing_waiting_CLOSE;
			}else
			{
				m_closing_state = closing_LIST;
			}
			break;
		case closing_waiting_CLOSE:
			break;
		case closing_CLOSED:
			m_timeout.Stop();
			sendMsg(msg_DISCONNECTED);
			m_closing_state = closing_LIST;
			changeStatus(_idle);
			break;
	}
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_CONNECT_handler(char *par,  int len)
{
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					break;
					
				case calling_CLOSE:
					break;
				case calling_waiting_CLOSE:
					break;
					
				case calling_CALL:
					break;
				case calling_waiting_CALL:
					m_calling_state = calling_CALLED;
					break;
					
				case calling_CALLED:
					break;

				case calling_NO_CARRIER:
					break;
			}
			break;
		case _closing:
			
			break;
	}
	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_evt_INQUIRY_PARTIAL_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP:: iWRAP_evt_NO_CARRIER_handler(char *par,  int len)
{
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					break;
					
				case calling_CLOSE:
					break;
				case calling_waiting_CLOSE:
					m_calling_state = calling_CLOSE;
					break;
				
				case calling_CALL:
					break;
				case calling_waiting_CALL:
					m_calling_state = calling_NO_CARRIER;
					break;
					
				case calling_CALLED:
					break;

				case calling_NO_CARRIER:
					break;
			}
			break;
		case _closing:
			switch(m_closing_state)
			{
				case closing_LIST:
					break;
				case closing_waiting_LIST:
					break;
				case closing_escapeSequence:
					break;
				case closing_waiting_escapeSequence:
					break;
				case closing_CLOSE:
					break;
				case closing_waiting_CLOSE:
					m_closing_state = closing_CLOSE;
					break;
				case closing_CLOSED:
					break;
			}
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_READY_handler(char *par,  int len)
{
	
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			m_sbaudState = sbaud_send_AT;
		case _asking_info:
			switch (m_askingInfoState)
			{
				case askingInfo_ask:
					break;
				case askingInfo_waiting:
					m_askingInfoState = askingInfo_received;
					break;
				case askingInfo_received:
					break;
			}
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			m_calling_state = calling_LIST;
			break;
		case _closing:
			m_closing_state = closing_LIST;
			break;
		
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_NAME_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_evt_RING_handler(char *par,  int len)
{
	iWRAP_SSP_msg_struct msg;

	msg.len = len;
	msg.code = msg_RING;
	strcpy(msg.par, par);
	sendMsg(msg);
	
	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_evt_NAME_ERROR_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_PAIR_handler(char *par,  int len)
{
	bool paired = false;

	if(len > 2)
	{
		if ( (par[len-1] == 'K' || par[len-1] == 'k') && (par[len-2] == 'O' || par[len-2] == 'o'))
			paired = true;
	}

	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					break;
					
				case calling_CLOSE:
					break;
				case calling_waiting_CLOSE:
					break;

				case calling_CALL:
					break;					
				case calling_waiting_CALL:
					break;
					
				case calling_CALLED:
					break;

				case calling_NO_CARRIER:
					break;
			}
			break;
		case _closing:
			switch(m_closing_state)
			{
				case closing_LIST:
					break;
				case closing_waiting_LIST:
					break;
				case closing_escapeSequence:
					break;
				case closing_waiting_escapeSequence:
					break;
				case closing_CLOSE:
					break;
				case closing_waiting_CLOSE:
					break;
				case closing_CLOSED:
					break;
			}
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_SYNTAX_ERROR_handler(char *par,  int len)
{

	return iWRAP_timeout_and_error_handler(par, len, true);
}


int iWRAP_BLUEGIGA_SPP::iWRAP_evt_AUTH_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_CALL_handler(char *par,  int len)
{
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					break;
					
				case calling_CLOSE:
					break;
				case calling_waiting_CLOSE:
					break;
				
				case calling_CALL:
					break;	
				case calling_waiting_CALL:
					iWRAP_SSP_msg_struct cmd;

					memcpy(cmd.par, par, len);
					cmd.len = len;
					cmd.code = msg_CALL;
					sendMsg(cmd);
					break;
					
				case calling_CALLED:
					break;
					
				case calling_NO_CARRIER:
					break;
			}
			break;
		case _closing:
			
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_INQUIRY_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_LIST_handler(char *par,  int len)
{
	int link_id;
	m_timeoutErrors = 0;

	if(len >= 1)
	{
		if (len <= 3 )
		{
			m_LIST_num = par[0] - '0';
			m_LIST_count = 0;
		}else
		{
			link_id = par[0] - '0';
			if(!m_listedLinkId.full())
			{
				m_listedLinkId.push_back(link_id);
			}
			m_LIST_count++;
		}
	}
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					/*if(len >= 1)
					{
						if (len <= 3 )
						{
							m_LIST_num = par[0] - '0';
							m_LIST_count = 0;
							if (m_LIST_num == 0)
							{
								m_calling_state = calling_CALL;
							}
						}else
						{
							link_id = par[0] - '0';
							if(!m_listedLinkId.full())
							{
								m_listedLinkId.push_back(link_id);
							}
							m_LIST_count++;
							if(m_LIST_count >= m_LIST_num)
							{
								m_calling_state = calling_CLOSE;
							}
						}
					}*/
					if(len >= 1)
					{
						if (len <= 3 )
						{
							if (m_LIST_num == 0)
							{
								m_calling_state = calling_CALL;
							}
						}else
						{
							if(m_LIST_count >= m_LIST_num)
							{
								m_calling_state = calling_CLOSE;
							}
						}
					}
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					break;
					
				case calling_CLOSE:
					break;
					
				case calling_waiting_CLOSE:
					break;
				case calling_CALL:
					break;
					
				case calling_waiting_CALL:
					break;
					
				case calling_CALLED:
					break;

				case calling_NO_CARRIER:
					break;
			}
			break;
		case _closing:
			switch(m_closing_state)
			{
				case closing_LIST:
					break;
				case closing_waiting_LIST:
					if(len >= 1)
					{
						if (len <= 3 )
						{
							if (m_LIST_num == 0)
							{
								m_closing_state = closing_CLOSED;
							}
						}else
						{
							if(m_LIST_count >= m_LIST_num)
							{
								m_closing_state = closing_CLOSE;
							}
						}
					}
					break;
				case closing_escapeSequence:
					break;
				case closing_waiting_escapeSequence:
					break;
				case closing_CLOSE:
					break;
				case closing_waiting_CLOSE:
					break;
				case closing_CLOSED:
					break;
			}
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_PROFILE_handler(char *par,  int len)
{
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;

		case _asking_info:
			
			break;
		case _setting_profile:
			m_settingProfile_state = sprofile_set;
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_BADDR_handler(char *par,  int len)

{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_NAME_handler(char *par,  int len)
{
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
		
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			switch(m_sBT_name_state)
			{
				case sBTname_send_NAME:
					break;
				case sBTname_waiting:
					m_sBT_name_state = sBTname_NAME_set;
					break;
				case sBTname_NAME_set:
				break;
			}
			break;
		case _setting_auth:
			
			break;
		
		case _setting_ssp:

			break;
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_CLASS_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_LAP_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_AUTH_handler(char *par,  int len)
{

	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
		
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			switch(m_sBT_auth_state)
			{
				case sBTauth_send_AUTH:
					break;
				case sBTauth_waiting:
					m_sBT_auth_state = sBTauth_AUTH_set;
					break;
				case sBTauth_AUTH_set:
					break;
			}
			break;
		
		case _setting_ssp:

			break;
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_PAIR_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_ROLE_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_SNIFF_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_BT_SSP_handler(char *par,  int len)
{

	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
		
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _setting_ssp:
			switch(m_sBT_ssp_state)
			{
				case sBTssp_send_SSP:
					break;
				case sBTssp_waiting:
					m_sBT_ssp_state = sBTssp_SSP_set;
					break;
				case sBTssp_SSP_set:
					break;
			}
			break;
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_CONTROL_BAUD_handler(char *par,  int len)
{
	
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			m_sbaudState = sbaud_send_AT;
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_CONTROL_CD_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP:: iWRAP_rsp_CONTROL_ECHO_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_CONTROL_ESCAPE_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_CONTROL_BIND_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SET_CONTROL_MSC_handler(char *par,  int len)
{
	
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;
		case _setting_msc:
			m_sBT_MSC_state = sBTmsc_MSC_set;
			break;
		case _setting_ssp:

			break;
		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_TEST_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_BER_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_RSSI_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_TXPOWER_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_SDP_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_PING_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_UNKNOWN_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_msg_timoutRx_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP:: iWRAP_msg_timoutTx_handler(char *par,  int len)
{
	return iWRAP_timeout_and_error_handler(par, len, false);
}


int iWRAP_BLUEGIGA_SPP::iWRAP_msg_HwError_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_msg_transmitBufferOverflowError_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_msg_receptionBufferOverflowError_handler(char *par,  int len)
{

	return 1;
}


int iWRAP_BLUEGIGA_SPP::iWRAP_msg_dataReceivedBufferOverflow_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_rsp_OK_handler(char *par,  int len)
{
	m_timeoutErrors = 0;
	
	switch(m_currentState)
	{
		case _idle:
			
			break;
		case _setting_baud:
			switch(m_sbaudState)
			{
				case sbaud_send_AT:
					break;
				case sbaud_waiting_AT:
					m_sbaudState = sbaud_BAUD_set;
					break;
					
				case sbaud_send_AT_2:
					break;
				case sbaud_waiting_AT_2:
					m_sbaudState = sbaud_set_BAUD;
					break;	
					
				case sbaud_set_BAUD:
					break;
				case sbaud_waiting_BAUD:
					break;
					
				case sbaud_send_escapeSequence:
					break;
				case sbaud_waiting_escapeSequence:
					break;
					
				case sbaud_BAUD_set:
					break;
			}
			break;
		case _asking_info:
			
			break;
		case _setting_bt_name:
			
			break;
		case _setting_auth:
			
			break;

		case _calling:
			
			break;
		case _closing:
			
			break;
	}
	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_msg_unknownError_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_msg_syntaxErrorSyncroError_handler(char *par,  int len)
{

	return 1;
}

int iWRAP_BLUEGIGA_SPP::iWRAP_timeout_and_error_handler(char *par,  int len, bool isSyntaxError)
{
	m_timeoutErrors++;
	switch(m_currentState)
	{
		case _idle:
			
			break;
			
		case _setting_baud:
			switch(m_sbaudState)
			{
				case sbaud_send_AT:
					break;
				case sbaud_waiting_AT:
					if(!isSyntaxError)
					{
						m_sbaudState = sbaud_send_escapeSequence;
					}else
					{
						m_sbaudState = sbaud_send_AT;
					}
					break;
					
				case sbaud_send_AT_2:
					break;
				case sbaud_waiting_AT_2:
					if(!isSyntaxError)
					{
						m_sbaudState = sbaud_send_AT;
					}else
					{
						m_sbaudState = sbaud_send_AT_2;
					}
					break;	

					
				case sbaud_set_BAUD:
					break;
				case sbaud_waiting_BAUD:
					m_sbaudState = sbaud_set_BAUD;
					break;
			
				case sbaud_send_escapeSequence:
					break;
				case sbaud_waiting_escapeSequence:
					m_sbaudState = sbaud_send_AT_2;
					break;
					
				case sbaud_BAUD_set:
					break;
			}
			break;
			
		case _asking_info:
			switch (m_askingInfoState)
			{
				case askingInfo_ask:
					break;
				case askingInfo_waiting:
					m_askingInfoState = askingInfo_ask;
					break;
				case askingInfo_received:
					break;
			}
			break;

		case _setting_profile:
			switch (m_settingProfile_state)
			{
				case sprofile_setting:
					break;
				case sprofile_waiting:
					m_settingProfile_state = sprofile_setting;
					break;
				case sprofile_set:
					break;
			}
			
			break;
		
		case _setting_bt_name:
			switch(m_sBT_name_state)
			{
				case sBTname_send_NAME:
					break;
				case sBTname_waiting:
					m_sBT_name_state = sBTname_send_NAME;
					break;
				case sBTname_NAME_set:
					break;
			}
			break;
			
		case _setting_auth:
			switch(m_sBT_auth_state)
			{
				case sBTauth_send_AUTH:
					break;
				case sBTauth_waiting:
					m_sBT_auth_state = sBTauth_send_AUTH;
					break;
				case sBTauth_AUTH_set:
					break;
			}
			break;
			
		case _setting_ssp:
			switch(m_sBT_ssp_state)
			{
				case sBTssp_send_SSP:
					break;
				case sBTssp_waiting:
					m_sBT_ssp_state = sBTssp_send_SSP;
					break;
				case sBTssp_SSP_set:
					break;
			}
			break;
			
		case _setting_msc:
			m_sBT_MSC_state = sBTmsc_send_MSC;
			break;	
			
		case _calling:
			switch(m_calling_state)
			{
				case calling_LIST:
					break;
				case calling_waiting_LIST:
					m_calling_state = calling_escapeSequence;
					break;
					
				case calling_escapeSequence:
					break;
				case calling_waiting_escapeSequence:
					m_calling_state = calling_LIST;
					break;
					
				case calling_CLOSE:
					break;
				case calling_waiting_CLOSE:
					break;
					
				case calling_CALL:
					break;
				case calling_waiting_CALL:
					break;
					
				case calling_CALLED:
					break;

				case calling_NO_CARRIER:
					break;
			}
			break;
			
		case _closing:
			switch(m_closing_state)
			{
				case closing_LIST:
					break;
				case closing_waiting_LIST:
					m_closing_state = closing_escapeSequence;
					break;
				case closing_escapeSequence:
					break;
				case closing_waiting_escapeSequence:
					m_closing_state = closing_LIST;
					break;
				case closing_CLOSE:
					break;
				case closing_waiting_CLOSE:
					break;
				case closing_CLOSED:
					break;
			}
			break;
	}

	if (m_timeoutErrors > 6)
	{
		m_timeoutErrors = 0;
		reset_machinesState();
		sendMsg(msg_moduleNotResponding);
	}
	return 1;
}

int iWRAP_BLUEGIGA_SPP::sendMsg(iWRAP_SSP_msg_struct msg)
{
	if(!m_message.full())
	{
		m_message.push_back(msg);
		return 0;
	}	
	return -1;
}

int iWRAP_BLUEGIGA_SPP::sendMsg(int i)
{
	iWRAP_SSP_msg_struct msg;

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


bool iWRAP_BLUEGIGA_SPP::getMsg(iWRAP_SSP_msg_struct &msg)
{
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
		return true;
	}
	return false;

}

byte iWRAP_BLUEGIGA_SPP::changeStatus(states state)
{
	char str[20];

	if(state != m_currentState)
	{
		m_previousState = m_currentState;
		m_currentState = state;
	}
	return 0;
}


int iWRAP_BLUEGIGA_SPP::VERIFY_BAUD_RATE()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_baud);
		m_sbaudState = sbaud_send_AT;
		return 1;
	}
	
	return -1;
}



int iWRAP_BLUEGIGA_SPP::CALL(char *_address)
{
	if(m_currentState == _idle)
	{
		strncpy(m_callingAddress, _address, 17);
		m_callingAddress[17] = '\0';
		
		changeStatus(_calling);
		m_calling_state = calling_LIST;
		return 1;
	}
	
	return -1;
}

int iWRAP_BLUEGIGA_SPP::CLOSE()
{
	if(m_currentState == _idle)
	{
		m_closing_state = closing_LIST;
		changeStatus(_closing);
		return 1;
	}
	
	return -1;
}



int iWRAP_BLUEGIGA_SPP::RESET()
{
	return -1;
}



int iWRAP_BLUEGIGA_SPP::INFO()
{
	if(m_currentState == _idle)
	{
		changeStatus(_asking_info);
		m_askingInfoState = askingInfo_ask;
		return 1;
	}
	
	return -1;
}



int iWRAP_BLUEGIGA_SPP::SET_BT_NAME()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_bt_name);
		m_sBT_name_state = sBTname_send_NAME;
		return 1;
	}
	
	return -1;
}


int iWRAP_BLUEGIGA_SPP::SET_PROFILE()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_profile);
		m_settingProfile_state= sprofile_setting;
		return 1;
	}
	
	return -1;
}

int iWRAP_BLUEGIGA_SPP::SET_BT_AUTH()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_auth);
		m_sBT_auth_state = sBTauth_send_AUTH;
		return 1;
	}
	
	return -1;
}



int iWRAP_BLUEGIGA_SPP::SET_BT_SSP()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_ssp);
		m_sBT_ssp_state = sBTssp_send_SSP;
		return 1;
	}
	
	return -1;
}


int iWRAP_BLUEGIGA_SPP::SET_CONTROL_MSC()
{
	if(m_currentState == _idle)
	{
		changeStatus(_setting_msc);
		m_sBT_MSC_state = sBTmsc_send_MSC;
		return 1;
	}
	
	return -1;
}

int iWRAP_BLUEGIGA_SPP::SET_BT_POWER()
{
	return -1;	
}

int iWRAP_BLUEGIGA_SPP::BER()
{
	return -1;
}



int iWRAP_BLUEGIGA_SPP::RSSI()
{
	return -1;
}



int iWRAP_BLUEGIGA_SPP::TXPOWER()
{
	return -1;
}


bool iWRAP_BLUEGIGA_SPP::strMsg(char *msg, int &len)
{
	if (m_strMsg.empty())
		return false;

	iWRAP_msg_struct imsg;

	m_strMsg.pop_front(imsg);
	len = imsg.len;
	memcpy(msg, imsg.par, imsg.len);
	
	return true;
}



























