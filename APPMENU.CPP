//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                TBO212
//  Nome File:               APPMENU.CPP
//  Descrizione:             Routines gestione menu applicazione
//  Note:
//
//  Supporto Fisico:         LPC213x
//  Versione:                $Revision: 1.34 $
//  Data Versione:           $Date: 2007/08/30 14:58:09 $
//  Descrizione Versione:    Stesura iniziale
//
//---------------------------------------------------------------------------//

#include "appmenu.h"

//-------------------------------------------------------
// Variabili extern
//-------------------------------------------------------v
extern const char sAppRevision[];
//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------


//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

static void reverse(int __revcolor);


//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------




//-------------------------------------------------------
// Corpo funzioni e metodi di questo modulo
//-------------------------------------------------------

void reverse(int __revcolor)
{
	if( __revcolor )
	{
		setcolor(WHITE);
		setbkcolor(BLACK);
	}
	else
	{
		setcolor(BLACK);
		setbkcolor(WHITE);
	}
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                 CMenuList
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
CMenuList::CMenuList()
{

	m_timRefresh.Preset(200);
	
	m_current =  NULL;					// partiamo dal logo
	m_previous = NULL;

	m_bMenuChanged = True;					// comunque va ridisegnato

}


void CMenuList::ChangePage(CMenuPage *__newpage)
{
	if( __newpage != m_current && __newpage )	// se si puo' fare....
	{
		cleardevice();
		m_previous = m_current;					// salva la pagina attuale
		m_current = __newpage;				// imposta la nuova pagina da visualizzare
		m_bMenuChanged = True;				// comunque va ridisegnato
	}
}

void CMenuList::BackPage()
{
	if( m_previous != m_current )				// se si puo' fare....
	{
		m_current = m_previous;					// imposta la nuova pagina da visualizzare (la precedente)
		m_bMenuChanged = True;				// comunque va ridisegnato
	}
}


// entry point
bool CMenuList::Entry(void)
{
int r;

	if( !m_current )
		return(False);

	if( m_bMenuChanged )
	{
		m_bMenuChanged = False;

		m_current->Draw();
		m_current->Refresh();

		display_update();
	}
	else
	{
		if( m_timRefresh.Match() )
		{
			m_current->Refresh();
			display_update();
			
			m_timRefresh.Preset(200);
		}
		else
		{
			
		}
	}
	return(True);

}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                 CMenuPage
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Paginetta menu abbastanza generica
CMenuPage::CMenuPage(void)
{
    m_curFrame = 0;           					// frame in editing attivo
    m_refreshTime = 200;							// tempo di rinfresco per il menu
}


CMenuPage::~CMenuPage()
{
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                 CMenuGeneralInfo
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

byte CMenuGeneralInfo::m_refcnt = 0;
const byte *CMenuGeneralInfo::m_icoCOM = NULL;
const byte *CMenuGeneralInfo::m_drawnIco = NULL;
CMenuGeneralInfo::COMM_icoType CMenuGeneralInfo::m_COMM_icoType = CMenuGeneralInfo::comm_noUsb;
bool CMenuGeneralInfo::m_icoBlink = false;
byte CMenuGeneralInfo::m_blinkCount = 0;
char CMenuGeneralInfo::m_strSAUstate[] = " ";


CMenuGeneralInfo::CMenuGeneralInfo()
{
	m_refcnt = 0;
	m_icoBlink = false;
	m_blinkCount = 0;
	setCOMMico(comm_noUsb, true);
}

// pagina generale
void CMenuGeneralInfo::Draw()
{
	setbkcolor(WHITE);
	cleardevice();

	setcolor(BLACK);
	putimage(2, 0, m_drawnIco, 0);

	line(0, 12, 127, 12);

	setcolor(BLACK);
	charset_select("pc6x8");
	gputsxy(36, 2, m_strSAUstate);
}

void CMenuGeneralInfo::Refresh()
{

	/*static const char anim[] = "-+<>";
	static byte animIdx;
	char s[24];
	bool batteryOk = True;

	charset_select("xm5x6");

	s[0] = anim[animIdx++];
	s[1] = 0;

	if( animIdx > 3 )
		animIdx = 0;

	gputsxy(0, 0, s);
	*/
	m_refcnt++;

	if( m_refcnt > 10 )
		m_refcnt = 0;

	if(m_icoBlink)
	{
		if(m_blinkCount == 3)
		{
			m_drawnIco = icoBLANK_24x10;
		}
		if(m_blinkCount == 6)
		{
			m_drawnIco = m_icoCOM;
			m_blinkCount = 0;
		}
		m_blinkCount++;

		setcolor(BLACK); 
		putimage(2, 0, m_drawnIco, 0);
	}
}

void CMenuGeneralInfo::setCOMMico(COMM_icoType type, bool blink)
{
	m_blinkCount = 0;
	m_COMM_icoType = type;
	m_icoBlink = blink;
	
	switch(type)
	{
		case comm_usb:
			m_icoCOM = icoUSB;
			break;
		case comm_noUsb:
			m_icoCOM = icoNO_USB;
			break;
		case comm_BT:
			m_icoCOM = icoBT;
			break;
	}
	
	setcolor(BLACK);
	m_drawnIco = m_icoCOM; 
	putimage(2, 0, m_drawnIco, 0);
		
}

void CMenuGeneralInfo::setSAUstate(const char* str)
{
	int i;
	for (i = 0; i < STR_SAU_STATE_LEN; i++)
		m_strSAUstate[i] = ' ';

	m_strSAUstate[STR_SAU_STATE_LEN] = '\0';

	setcolor(BLACK);
	charset_select("pc6x8");

	gputsxy(36, 2, m_strSAUstate);

 	if (strlen(str) > STR_SAU_STATE_LEN)
 	{
 		strncpy(m_strSAUstate, str, STR_SAU_STATE_LEN);
		m_strSAUstate[STR_SAU_STATE_LEN] = '\0';
	}else
	{
		strcpy(m_strSAUstate, str);
	}
	gputsxy(36, 2, m_strSAUstate);
	
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                 CMenuLogo
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

CMenuLogo::CMenuLogo()
{
	m_logo = (byte*)pictLogo;
}


void CMenuLogo::Draw()
{			
	cleardevice();	// conviene pulire

	putimage(2, 0, m_logo, 0);
	m_timLogo.Preset(3000);			// 6 secondi di attesa logo

	charset_select("xm6x10");
	gprintfxy(22, 42, "WWW.MENFIS.IT");
	charset_select("xm5x6");
	gputsxy(80, 58, sAppRevision);
}


void CMenuLogo::Refresh()
{

	/*if( m_timLogo.Match() )					// tempo scaduto ?
	{
							// se l'orologio è configurato ...
		pageMenu->ChangePage(pADuCstatus);					// avanti pure
		m_timLogo.Stop();
	}*/

}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                 CMenuMain
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

CMenuMain::CMenuMain()
{
	m_icoWTP[0] = (byte*)icoWTP_00;
	m_icoWTP[1] = (byte*)icoWTP_01;
	m_icoWTP[2] = (byte*)icoWTP_02;
	m_icoWTP[3] = (byte*)icoWTP_03;
	m_icoWTP[4] = (byte*)icoWTP_04;
	m_icoWTP[5] = (byte*)icoWTP_05;
	
	m_icoWTH[0] = (byte*)icoWTH_00;
	m_icoWTH[1] = (byte*)icoWTH_01;
	m_icoWTH[2] = (byte*)icoWTH_02;
	m_icoWTH[3] = (byte*)icoWTH_03;
	m_icoWTH[4] = (byte*)icoWTH_04;
	m_icoWTH[5] = (byte*)icoWTH_05;
	m_icoWTH[6] = (byte*)icoWTH_06;
	m_icoWTH[7] = (byte*)icoWTH_07;	
	m_icoWTH[8] = (byte*)icoWTH_08;
	m_icoWTH[9] = (byte*)icoWTH_09;
	
	m_wtpIcoIndex = 0;
	m_wtpIsMoving = false;
	m_wtpSpeed = 0;
	
	m_wthIcoIndex = 0;
	m_wthIsMoving = true;
	m_wthSpeed = 0;
	
	m_lmtSwHighStatus = lmtsw_reached;
	m_lmtswLowStatus = lmtsw_reached;
	setLimitSwitch(lmtsw_notReached, lmtsw_notReached);

	setWTPisMoving(false);
	setWTHisMoving(false);
	
}

void CMenuMain::Draw()
{
	
	CMenuGeneralInfo::Draw();
	
	charset_select("pc6x8");
	gputsxy(10, 16, "WTP");
	putimage(2, 24, m_icoWTP[0], 0);

	charset_select("pc6x8");
	gputsxy(2, 56, "ml/min");
	setWTPspeed(0);

	charset_select("pc6x8");
	gputsxy(76, 16, "WTH");
	setWTHspeed(0);
	gputsxy(68, 56, "mm/sec");
	putimage(50, 36, m_icoWTH[0], 0);

	drawWthLimitSwitches();
}

void CMenuMain::Refresh()
{
	int x;
	const byte *pIco = NULL;
	
	CMenuGeneralInfo::Refresh();			// refresh data+ora ?
	
	if(m_wtpIsMoving && m_wtpTimer.Match())
	{
		m_wtpTimer.Preset(m_wtpUpgradeTime);
		putimage(2, 24, m_icoWTP[m_wtpIcoIndex], 0);
		writeWtpSpeed();
		m_wtpIcoIndex++;
		if(m_wtpIcoIndex >= WTP_ICONS_NUM)
		{
			m_wtpIcoIndex = 0;
		}
	}

	if(m_wthIsMoving && m_wthTimer.Match())
	{
		if (m_wthSpeed > 0)
		{
			m_wthTimer.Preset(m_wthUpgradeTime);
			putimage(50, 36, m_icoWTH[m_wthIcoIndex], 0);
			m_wthIcoIndex++;
			if(m_wthIcoIndex >= WTH_ICONS_NUM)
			{
				m_wthIcoIndex = 0;
			}
		}else
		{
			m_wthTimer.Preset(m_wthUpgradeTime);
			putimage(50, 36, m_icoWTH[m_wthIcoIndex], 0);
			m_wthIcoIndex--;
			if(m_wthIcoIndex < 0)
			{
				m_wthIcoIndex = WTH_ICONS_NUM - 1;
			}
		}
	}
	
	if (m_lmtSwHighStatus == lmtsw_error)
	{
		if (m_icoLMTSW_high == (byte*)icoLMTSW_NOT_REACHED)
			m_icoLMTSW_high = (byte*)icoLMTSW_REACHED;
		else
			m_icoLMTSW_high = (byte*)icoLMTSW_NOT_REACHED;

		drawWthLimitSwitches();
	}
	
	if (m_lmtswLowStatus == lmtsw_error)
	{
		if (m_icoLMTSW_low == (byte*)icoLMTSW_NOT_REACHED)
			m_icoLMTSW_low = (byte*)icoLMTSW_REACHED;
		else
			m_icoLMTSW_low = (byte*)icoLMTSW_NOT_REACHED;

		drawWthLimitSwitches();
	}
}



void CMenuMain::setWTPisMoving(bool move)
{
	if (m_wtpIsMoving == move)
		return;
	
	m_wtpIsMoving = move;
	m_wtpIcoIndex = 0;

	putimage(2, 24, m_icoWTP[m_wtpIcoIndex], 0);
	setWTPspeed(m_wtpSpeed);
}

void CMenuMain::setWTPspeed(int speed)
{
	int i;

	for (i = 0; i < 3; i++)
		m_strWtpSpeed[i] = ' ';
	
	m_strWtpSpeed[3] = '\0';
	
	if (m_wtpSpeed < 10)
		m_strWtpSpeed[1] = '\0';
	else if(m_wtpSpeed < 100)
		m_strWtpSpeed[2] = '\0';
	
	writeWtpSpeed(); // clean previous speed
	
	m_wtpSpeed = speed;
	
	if(m_wtpSpeed > 999)
		m_wtpSpeed = 999;
	else if (m_wtpSpeed < -99)
		m_wtpSpeed = -99;
	
	if (m_wtpSpeed == 0 || !m_wtpIsMoving)
	{
		m_wtpTimer.Stop();
	}else
	{
		m_wtpUpgradeTime = 150 + (200 / m_wtpSpeed)*3;

		if(m_wtpUpgradeTime > 1000)
			m_wtpUpgradeTime = 1000;

		if (m_wtpUpgradeTime < 150)
			m_wtpUpgradeTime = 150;

		if (!m_wtpTimer.getStatus())
			m_wtpTimer.Preset(m_wtpUpgradeTime);
	}
	sprintf(m_strWtpSpeed, "%d", m_wtpSpeed);
	writeWtpSpeed();			//write new speed
}

void CMenuMain::writeWtpSpeed()
{
	int x, y;
	
	
	y = 37;
	if(m_wtpSpeed < 10)
	{
		x = 16;
	}else if (m_wtpSpeed < 100)
	{
		x = 12;
	}else
	{
		x = 10;
	}
	setcolor(BLACK);
	charset_select("pc6x8");
	gputsxy(x, y, m_strWtpSpeed);
}

void CMenuMain::setLimitSwitch(LmtSwStatus low, LmtSwStatus high)
{
	if(m_lmtSwHighStatus != high)
	{
		m_lmtSwHighStatus = high;
		switch(m_lmtSwHighStatus)
		{
			case lmtsw_reached:
				m_icoLMTSW_high = (byte*)icoLMTSW_REACHED;
				break;
			case lmtsw_notReached:
				m_icoLMTSW_high = (byte*)icoLMTSW_NOT_REACHED;
				break;
		}
	}

	if(m_lmtswLowStatus != low)
	{
		m_lmtswLowStatus = low;
		switch(m_lmtswLowStatus)
		{
			case lmtsw_reached:
				m_icoLMTSW_low = (byte*)icoLMTSW_REACHED;
				break;
			case lmtsw_notReached:
				m_icoLMTSW_low = (byte*)icoLMTSW_NOT_REACHED;
				break;
		}
	}
	
	if (m_lmtswLowStatus == lmtsw_notReached && m_lmtSwHighStatus == lmtsw_reached)
		m_wthIcoIndex = 0;
	else if (m_lmtswLowStatus == lmtsw_reached && m_lmtSwHighStatus == lmtsw_notReached)
		m_wthIcoIndex = WTH_ICONS_NUM - 1;
	else if (m_lmtswLowStatus == lmtsw_reached && m_lmtSwHighStatus == lmtsw_reached)
		m_wthIcoIndex = (WTH_ICONS_NUM - 1)/2;
	
	
	putimage(50, 36, m_icoWTH[m_wthIcoIndex], 0);
	drawWthLimitSwitches();
}

void CMenuMain::drawWthLimitSwitches()
{
	putimage(102, 28, m_icoLMTSW_high, 0);
	putimage(52, 28, m_icoLMTSW_low, 0);
}

void CMenuMain::setWTHisMoving(bool move)
{
	if (move == m_wthIsMoving)
		return;
	
	m_wthIsMoving = move;
	if (!move)
	{
		if (m_lmtswLowStatus == lmtsw_notReached && m_lmtSwHighStatus == lmtsw_reached)
		m_wthIcoIndex = 0;
		else if (m_lmtswLowStatus == lmtsw_reached && m_lmtSwHighStatus == lmtsw_notReached)
			m_wthIcoIndex = WTH_ICONS_NUM - 1;
		else
			m_wthIcoIndex = (WTH_ICONS_NUM - 1)/2;
	}		
	putimage(50, 36, m_icoWTH[m_wthIcoIndex], 0);
	setWTHspeed(m_wthSpeed);

}

void CMenuMain::setWTHspeed(int speed)
{
	int i;
	for (i = 0; i < 5; i++)
		m_strWthSpeed[i] = ' ';
	
	m_strWthSpeed[5] = '\0';

	if (m_wthSpeed < -99)
		m_strWthSpeed[5] = '\0';
	else if (m_wthSpeed < -9)
		m_strWthSpeed[4] = '\0';
	else if (m_wthSpeed < 0)
		m_strWthSpeed[4] = '\0';	
	else if (m_wthSpeed < 10)
		m_strWthSpeed[4] = '\0';
	else if(m_wthSpeed < 100)
		m_strWthSpeed[5] = '\0';
	
	writeWthSpeed(); // clean previous speed
	
	m_wthSpeed = speed;
	if(m_wthSpeed > 999)
		m_wthSpeed = 999;
	else if (m_wthSpeed < -999)
		m_wthSpeed = -999;
		
	if (m_wthSpeed == 0 || !m_wthIsMoving)
	{
		m_wthTimer.Stop();
	}else
	{
		m_wthUpgradeTime = 150 + (400 / __ABS(m_wthSpeed))*15;

		if(m_wthUpgradeTime > 1000)
			m_wthUpgradeTime = 1000;

		if (m_wthUpgradeTime < 150)
			m_wthUpgradeTime = 150;

		if (!m_wthTimer.getStatus())
			m_wthTimer.Preset(m_wthUpgradeTime);
	}
	
	if(m_wthSpeed < 0 && m_wthSpeed > -10)
		sprintf(m_strWthSpeed, "-0.%d", __ABS(m_wthSpeed));
	else
		sprintf(m_strWthSpeed, "%d.%d", (m_wthSpeed/10), __ABS(m_wthSpeed - ((m_wthSpeed/10)*10)));

	writeWthSpeed();			//write new speed
}

void CMenuMain::writeWthSpeed()
{
	int x, y;
	
	y = 28;
	if(m_wthSpeed < -99)
	{
		x = 64;
	}else if(m_wthSpeed < -9)
	{
		x = 70;
	}else if(m_wthSpeed < 0)
	{
		x = 72;
	}else if(m_wthSpeed < 10)
	{
		x = 74;
	}else if (m_wthSpeed < 100)
	{
		x = 72;
	}else
	{
		x = 70;
	}
	
	setcolor(BLACK);
	charset_select("pc6x8");
	gputsxy(x, y, m_strWthSpeed);
}


//pagine ADuC status
CMenuADuCstatus::CMenuADuCstatus()
{

}

void CMenuADuCstatus::Draw()
{
	CMenuGeneralInfo::Draw();

	setcolor(BLACK);
	charset_select("pc6x8");

	gputsxy(0, 14, "ADuC1:");
	gputsxy(0, 26, "ADuC2:");
	gputsxy(0, 38, "ADuC3:");
	gputsxy(0, 50, "ADuC4:");
	gputsxy(64, 14, "ADuC5:");
	gputsxy(64, 26, "ADuC6:");
	gputsxy(64, 38, "ADuC7:");
	
}

void CMenuADuCstatus::Refresh()
{
	CMenuGeneralInfo::Refresh();
}

void CMenuADuCstatus::setADuCstate(int ADuC, const char* str)
{
	char clr[STR_ADUC_LEN];
	int i;

	for (i = 0; i < STR_ADUC_LEN; i++)
		clr[i] = ' ';

	clr[STR_ADUC_LEN - 1] = '\0';
	
	if(ADuC >= ADuC_NUM)
		return;

	if (strlen(str) >= STR_ADUC_LEN)
	{
		strncpy(m_strADuCstate[ADuC], str, STR_ADUC_LEN-1);
		m_strADuCstate[ADuC][STR_ADUC_LEN]='\0';
	}else
	{
		strcpy(m_strADuCstate[ADuC], str);
	}
	
	setcolor(BLACK);
	charset_select("pc6x8");

	switch (ADuC)
	{
		case ADuC_VINF:
			gputsxy(38, 14, clr);
			gputsxy(38, 14, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_VV:
			gputsxy(38, 26, clr);
			gputsxy(38, 26, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P12:
			gputsxy(38, 38, clr);
			gputsxy(38, 38, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P34:
			gputsxy(38, 50, clr);
			gputsxy(38, 50, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P56:
			gputsxy(102, 14, clr);
			gputsxy(102, 14, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P78:
			gputsxy(102, 26, clr);
			gputsxy(102, 26, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_GEN:
			gputsxy(102, 38, clr);
			gputsxy(102, 38, m_strADuCstate[ADuC]);
			
			break; 
	}
}

//pagine ADuC status
CMenuADuCfrqCal::CMenuADuCfrqCal()
{

}

void CMenuADuCfrqCal::Draw()
{
	CMenuGeneralInfo::Draw();

	setcolor(BLACK);
	charset_select("xm5x6");

	gputsxy(0, 14, "ADuC1:");
	gputsxy(0, 21, "ADuC2:");
	gputsxy(0, 28, "ADuC3:");
	gputsxy(0, 35, "ADuC4:");
	gputsxy(0, 42, "ADuC5:");
	gputsxy(0, 49, "ADuC6:");
	gputsxy(0, 56, "ADuC7:");
	
}

void CMenuADuCfrqCal::Refresh()
{
	CMenuGeneralInfo::Refresh();
}

void CMenuADuCfrqCal::setADuCinfo(int ADuC, const char* str)
{
	char clr[STR_ADUC_LEN];
	int i;

	for (i = 0; i < STR_ADUC_LEN; i++)
		clr[i] = ' ';

	clr[STR_ADUC_LEN - 1] = '\0';
	
	if(ADuC >= ADuC_NUM)
		return;

	if (strlen(str) >= STR_ADUC_LEN)
	{
		strncpy(m_strADuCstate[ADuC], str, STR_ADUC_LEN-1);
		m_strADuCstate[ADuC][STR_ADUC_LEN]='\0';
	}else
	{
		strcpy(m_strADuCstate[ADuC], str);
	}
	
	setcolor(BLACK);
	charset_select("xm5x6");

	switch (ADuC)
	{
		case ADuC_VINF:
			gputsxy(32, 14, clr);
			gputsxy(32, 14, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_VV:
			gputsxy(32, 21, clr);
			gputsxy(32, 21, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P12:
			gputsxy(32, 28, clr);
			gputsxy(32, 28, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P34:
			gputsxy(32, 35, clr);
			gputsxy(32, 35, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P56:
			gputsxy(32, 42, clr);
			gputsxy(32, 42, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_P78:
			gputsxy(32, 49, clr);
			gputsxy(32, 49, m_strADuCstate[ADuC]);
			
			break;
		case ADuC_GEN:
			gputsxy(32, 56, clr);
			gputsxy(32, 56, m_strADuCstate[ADuC]);
			
			break; 
	}
}


CMenuInfo::CMenuInfo()
{
	int i, k;
	
	m_strToDraw = 0;
	m_currentStrIndex = 0;
	m_lastDrawedIndex = 0;
	for (i = 0; i < STR_NUM; i++)
	{
		for (k = 0; k < STR_BT_LEN; k++)
		{
			m_strCLEAR[k] = ' ';
			m_strBT[i][k] = ' ';
		}
		m_strBT[i][STR_BT_LEN ] = '\0';
	}
	m_strCLEAR[STR_BT_LEN] = '\0';

	m_refershTimer.Preset(1000);
}

void CMenuInfo::Draw()
{
	CMenuGeneralInfo::Draw();
}

void CMenuInfo::Refresh()
{
	int i;
	int drawingIndex;
	int startIndex;
	
	CMenuGeneralInfo::Refresh();

	if (m_refershTimer.Match() && (m_strToDraw != 0))
	{
		setcolor(BLACK);
		charset_select("xm5x6");

		startIndex = m_lastDrawedIndex;
		if(startIndex < 0)
			startIndex = 0;
		
		for(i = 0; i < STR_DISPLAY_NUM; i++)
		{
			drawingIndex = startIndex - i;
			if(drawingIndex < 0)
				drawingIndex += STR_NUM;
			
			gputsxy(0, 14 + (i * 6), m_strCLEAR);
			gputsxy(0, 14 + (i * 6), m_strBT[drawingIndex]);
		}

		m_lastDrawedIndex++;
		if(m_lastDrawedIndex >= STR_NUM)
			m_lastDrawedIndex = 0;
		
		m_strToDraw--;
		if(m_strToDraw  < 0)
		{
			m_strToDraw = 0;
			m_lastDrawedIndex = m_currentStrIndex;
		}	

		m_refershTimer.Preset(100);
	}
}
	
void CMenuInfo::addStr(const char* str)
{
	if (strlen(str) > STR_BT_LEN)
	{
		strncpy(m_strBT[m_currentStrIndex], str, STR_BT_LEN);	
		m_strBT[m_currentStrIndex][STR_BT_LEN] = '\0';
	}else
	{
		strcpy(m_strBT[m_currentStrIndex], str);
	}
	
	m_currentStrIndex++;
	if(m_currentStrIndex >= STR_NUM)
		m_currentStrIndex = 0;

	m_strToDraw++;
	if(m_strToDraw > STR_NUM)
		m_strToDraw = STR_NUM;
}

void CMenuInfo::strClear()
{
	int i,k;
	
	for (i = 0; i < STR_NUM; i++)
	{
		for (k = 0; k < STR_BT_LEN; k++)
		{
			m_strBT[i][k] = ' ';
		}
		m_strBT[i][STR_BT_LEN ] = '\0';
	}
	m_strToDraw = 0;
	m_currentStrIndex = 0;
	m_lastDrawedIndex = 0;
}


CMenuMessages::CMenuMessages()
{
	int i;
	for(i = 0; i < MESSAGES_NUM; i++)
	{
		strcpy(m_message[i], "No Message");
	}
}

void CMenuMessages::Draw()
{
	CMenuGeneralInfo::Draw();

	setcolor(BLACK);
	charset_select("pc6x8");

	gputsxy(0, 14, m_message[0]);
	gputsxy(0, 26, m_message[1]);
	gputsxy(0, 38, m_message[2]);
	gputsxy(0, 50, m_message[3]);
	
}

void CMenuMessages::Refresh()
{
	CMenuGeneralInfo::Refresh();
}

void CMenuMessages::setMessage(int line, const char* str)
{
	char clr[MESSAGE_LENGHT];
	int i;
	
	if ((line < 0) || (line >= MESSAGES_NUM))
		return;

	for (i = 0; i < MESSAGE_LENGHT; i++)
		clr[i] = ' ';

	clr[MESSAGE_LENGHT - 1] = '\0';
	
	setcolor(BLACK);
	charset_select("pc6x8");

	if(strlen(str) > MESSAGE_LENGHT - 1)
	{
		strcpy(m_message[line], "str too long");
	}else
	{
		strcpy(m_message[line], str);
	}

	switch (line)
	{
		case 0:
			gputsxy(0, 14, clr);
			gputsxy(0, 14, m_message[line]);
			break;
		case 1:
			gputsxy(0, 26, clr);
			gputsxy(0, 26, m_message[line]);
			break;
		case 2:
			gputsxy(0, 38, clr);
			gputsxy(0, 38, m_message[line]);
			break;
		case 3:
			gputsxy(0, 50, clr);
			gputsxy(0, 50, m_message[line]);
			break;
	}
}

CMenuSwitchOff::CMenuSwitchOff()
{
	int i;
	
	m_switchOffTimer.Stop();
	
	for(i = 0; i < MESSAGES_NUM; i++)
	{
		strcpy(m_message[i], " ");
	}
	
	strcpy(m_message[0], "Switching off...");
 
}

void CMenuSwitchOff::Draw()
{
	CMenuGeneralInfo::Draw();

	setcolor(BLACK);
	charset_select("pc6x8");

	gputsxy(0, 14, m_message[0]);
	gputsxy(0, 26, m_message[1]);
	gputsxy(0, 38, m_message[2]);
	gputsxy(0, 50, m_message[3]);
}

void CMenuSwitchOff::Refresh()
{
	long time;
	char str[MESSAGE_LENGHT];
	
	CMenuGeneralInfo::Refresh();
	
	if(m_switchOffTimer.getStatus())
	{
		time = ((long)m_time - (long)m_switchOffTimer.getMsec()) / 1000;

		if ((time < 0) && (PLD_GEN_REG->KEY == 1))
		{
			PinEN_POWER = 1;
			PinEN_POWER = 1;
		}
		
		if (time < 0)
			time = 0;

		sprintf(str, "...... %d", time);
		
		setMessage(1, str);
	}
}

void CMenuSwitchOff::setMessage(int line, const char* str)
{
	char clr[MESSAGE_LENGHT];
	int i;
	
	if ((line < 0) || (line >= MESSAGES_NUM))
		return;

	for (i = 0; i < MESSAGE_LENGHT; i++)
		clr[i] = ' ';

	clr[MESSAGE_LENGHT - 1] = '\0';
	
	setcolor(BLACK);
	charset_select("pc6x8");

	if(strlen(str) > MESSAGE_LENGHT - 1)
	{
		strcpy(m_message[line], "str too long");
	}else
	{
		strcpy(m_message[line], str);
	}

	switch (line)
	{
		case 0:
			gputsxy(0, 14, clr);
			gputsxy(0, 14, m_message[line]);
			break;
		case 1:
			gputsxy(0, 26, clr);
			gputsxy(0, 26, m_message[line]);
			break;
		case 2:
			gputsxy(0, 38, clr);
			gputsxy(0, 38, m_message[line]);
			break;
		case 3:
			gputsxy(0, 50, clr);
			gputsxy(0, 50, m_message[line]);
			break;
	}
}



//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source: /root/Confer/APPMENU.CPP,v $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.1 $
//@@  $Date: 2007/08/30 14:58:09 $
//@@
//@@
//@@  $Log: APPMENU.CPP,v $
//@@  Revision 1.1  2005/09/03 20:54:37  enrico
//@@  Prima versione funzionante sulle schede tbo212-02
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: APPMENU.CPP,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2007/08/30 14:58:09 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

