/**
@file		Sound.h
@brief		Functions to manage board's buzzer.

@author		
@date		18/01/2011
@version	01.0
*/


#ifndef _SOUND_H_
#define _SOUND_H_


#include "global.h"				
#include "swtim.h"			// sw-service da non includere perihè c'è già nella time_cls
#include "alloc.h"


/**
Kind of note to play.
*/
enum KindOfNote {_DO, _REb, _RE, _MIb, _MI, _FA, _SOLb, _SOL, _LAb, _LA, _SIb, _SI};

/**
Kind of sound to play
*/
enum KindOfSound {NO_SOUND, KEY_LIKE, LONG_LIKE, ALARM_LIKE};

/**
Buzzer struct
*/
typedef struct{
bool				isPlaying;  /**< true if the buzzer is playing*/
KindOfSound			sound;		/**< kind of sounf to play*/
KindOfNote			nota;		/**< kind of note to play*/
int 				length;		/**< playing time in msec*/
DecTimer			buzTimer;	/**< playing timer. The buzzer is playying till this timer DecTimer::Match()*/
word				pwmFrq;		/**< PWM wave frequency*/
byte				pwmCycle;	/**< PWM wave duty cycle*/	
double 				frqBase;	/**< base time to calculate the PWM frequency*/
DecTimer			alarmTimer; /**< playing time for alarm.*/
int 				numBeepTime;
int 				numBeepTimeCount;
}CICALINO;

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

void 			buzzer_init(void);
void 			buzzerManager(void);
void 			goBuzzer(KindOfSound SoundToPlay);
void 			goBuzzerTimes(KindOfSound SoundToPlay, int times);
KindOfSound		buzzerSound();
void 			shutBuzzer();
bool			buzzer_isPlaying();

#ifdef __cplusplus
}
#endif

#endif
//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//






//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source:  $
//@@  Note di revisione:                
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: SOUND.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2003/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            M E N F I S                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//


