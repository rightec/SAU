//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//
//  Progetto:                TBO212
//  Nome File:               APPMENU.H
//  Descrizione:             Definizioni per gestione menu
//  Versione:                $Revision: 1.18 $
//  Data Versione:           $Date: 2007/04/03 14:51:14 $
//
//---------------------------------------------------------------------------//

#ifndef __APPMENU_H__
#define __APPMENU_H__

#include "global.h"
#include "error.h"
#include "alloc.h"
#include "dpydrv.h"
#include "graph.h"
#include "chrset.h"
#include "swtim.h"
#include "icons.h"


//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define NMAX_MENU_PAGE	40
#define MAX_MENU_ITEMS	18

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

						// modalità formattazione campi per dati
struct FmtData{
    byte    xpos;
    byte    ypos;
    void    *data;
    byte    type;
    byte    len;
    byte    point;
    byte    zeroFill;
    long    min;
    long    max;
};



// Paginetta menu abbastanza generica
class CMenuPage{

public:

    CMenuPage(void);
	virtual ~CMenuPage();

	virtual void Draw() {};						// ridisegna tutta la pagina di menu
	virtual void Refresh() {};					// rinfresca (c'è caldo) la pagina di menu
protected:

    short   	m_curFrame;           			// frame in editing attivo
    short		m_refreshTime;					// tempo di rinfresco per il menu
};



// pagina generale con gestione orologio
class CMenuGeneralInfo: public CMenuPage{

public:
	enum COMM_icoType
	{
		comm_usb = 0,
		comm_noUsb,
		comm_BT,
	};
	
    CMenuGeneralInfo();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void setCOMMico(COMM_icoType type, bool blink);
	COMM_icoType getCOMMico(){return m_COMM_icoType;}

	void setIcoBlinking(bool blink){setCOMMico(m_COMM_icoType, blink);}	
	bool getIcoIsBlinking(){return m_icoBlink;}
	
	void setSAUstate( const char* str);
	
protected:
	enum defualts
	{
		STR_SAU_STATE_LEN = 13,
	};
	
	static byte 	m_refcnt;
	
	static const byte *m_icoCOM;
	static const byte *m_drawnIco;
	static COMM_icoType m_COMM_icoType;
	static bool m_icoBlink;
	static byte m_blinkCount;
	
	static char m_strSAUstate[STR_SAU_STATE_LEN + 1];
};


// pagina presentazione
class CMenuLogo: public CMenuPage{

public:
    CMenuLogo();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

protected:
	byte *m_logo;
	DecTimer	m_timLogo;
};

// pagina menu principale
class CMenuMain: public CMenuGeneralInfo{

public:

	enum LmtSwStatus
	{
		lmtsw_reached,
		lmtsw_notReached,
		lmtsw_error,
	};
	
    CMenuMain();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	//void setUSBconnected(bool connect);
	//bool getUSBconnectedFlag(){return m_USBconnect;};

	void setWTPisMoving(bool move);
	bool getWTPisMoving(){return m_wtpIsMoving;};
	void setWTPspeed(int speed);

	void setLimitSwitch(LmtSwStatus low, LmtSwStatus high);

	void setWTHisMoving(bool move);
	bool getWTHisMoving(){return m_wtpIsMoving;};
	void setWTHspeed(int speed);
	
protected:

	void writeWtpSpeed();
	
	void writeWthSpeed();
	void drawWthLimitSwitches();
	
	enum defualts
	{
		WTP_ICONS_NUM = 6,
		WTH_ICONS_NUM = 10,	
	};
	
	byte *m_icoWTP[WTP_ICONS_NUM];
	byte m_wtpIcoIndex;
	bool m_wtpIsMoving;
	char m_strWtpSpeed[4];
	int m_wtpSpeed;
	DecTimer m_wtpTimer;
	int m_wtpUpgradeTime;

	byte *m_icoLMTSW_low;
	byte *m_icoLMTSW_high;
	LmtSwStatus m_lmtswLowStatus;
	LmtSwStatus m_lmtSwHighStatus;
	
	byte *m_icoWTH[WTH_ICONS_NUM];
	char m_wthIcoIndex;
	bool m_wthIsMoving;
	char m_strWthSpeed[6];
	int m_wthSpeed;
	DecTimer m_wthTimer;
	int m_wthUpgradeTime;
};


//pagina ADuC status
class CMenuADuCstatus: public CMenuGeneralInfo{

public:
	
    CMenuADuCstatus();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void setADuCstate(int ADuC, const char* str);
	
protected:

	enum defualts
	{
		STR_ADUC_LEN = 4,
	};

	char m_strADuCstate[ADuC_NUM][STR_ADUC_LEN + 1];
};

//pagina ADuC frequancy calibration
class CMenuADuCfrqCal: public CMenuGeneralInfo{

public:
	
    CMenuADuCfrqCal();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void setADuCinfo(int ADuC, const char* str);
	
protected:

	enum defualts
	{
		STR_ADUC_LEN = 19,
	};

	char m_strADuCstate[ADuC_NUM][STR_ADUC_LEN + 1];
};

class CMenuInfo: public CMenuGeneralInfo{

public:
	
    CMenuInfo();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void addStr(const char* str);
	void strClear();
protected:

	enum defualts
	{
		STR_BT_LEN = 25,
		STR_DISPLAY_NUM = 8,	
		STR_NUM = 40,	
	};

	char m_strBT[STR_NUM][STR_BT_LEN + 1];
	char m_strCLEAR[STR_BT_LEN + 1];
	
	int m_currentStrIndex;
	int m_lastDrawedIndex;
	int m_strToDraw;
	DecTimer m_refershTimer;
};

//pagina Messages
class CMenuMessages: public CMenuGeneralInfo
{

public:
	
    CMenuMessages();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void setMessage(int line, const char* str);
	
protected:
	enum defaults
	{
		MESSAGES_NUM = 4,
		MESSAGE_LENGHT = 21,
	};
	char m_message[MESSAGES_NUM][MESSAGE_LENGHT];
};

// pagina menu principale
class CMenuSwitchOff: public CMenuGeneralInfo{

public:
	
    CMenuSwitchOff();
	virtual void Draw();					// ridisegna tutta la pagina di menu
	virtual void Refresh();					// rinfresca (c'è caldo) la pagina di menu

	void startSwitchOffTimer(dword time = 3000){m_switchOffTimer.Start(); m_switchOffTimer.Reset(); m_time = time;};
	
protected:
	enum defaults
	{
		MESSAGES_NUM = 4,
		MESSAGE_LENGHT = 21,
	};

	void setMessage(int line, const char* str);
	
	char m_message[MESSAGES_NUM][MESSAGE_LENGHT];
	
	IncTimer m_switchOffTimer;
	dword m_time;
};

////////////////////////////////////////////
// Rude container lista dei menu + o - flat

class CMenuList{

public:
	CMenuList();

	void 		ChangePage(CMenuPage *__newpage);
	void 		BackPage();
	CMenuPage *GetCurrent() { return m_current; }	// menu attualmente in esecuzione

	bool 		Entry(void);

protected:
	bool		AddMenu(CMenuPage *__pmenu);	// aggiunge un menu all'array volante

	DecTimer	m_timRefresh;						// timer per rinfresco menu
	DecTimer	m_timExitSetup;					// timer per uscita dal menu di setup

	bool		m_bMenuChanged;					// flag menu cambiato
	CMenuPage	*m_current;						// menu attualmente in esecuzione
	CMenuPage	*m_previous;						// menu precedentemente in esecuzione
};




//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern CMenuList *pageMenu;
extern CMenuLogo *pLogo;
extern CMenuMain *pMain;
extern CMenuADuCstatus *pADuCstatus;
extern CMenuMessages *pageMessages;
extern CMenuSwitchOff *pageSwitchOff;
extern CMenuInfo *pageInfo;
extern CMenuADuCfrqCal *pageADuCfrqCal;
#endif

//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source: /root/TBO212/APPMENU.H,v $
//@@  Note di revisione:
//@@
//@@  $Log: APPMENU.H,v $
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: APPMENU.H,v $
//@@  $Revision: 1.18 $
//@@  $State: Exp $
//@@  $Date: 2007/04/03 14:51:14 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
