/**
@file		Sound.cpp
@brief		Functions to manage board's buzzer.

@author		
@date		18/01/2011
@version	01.0
*/


#include "sound.h"


static	CICALINO				BUZZERdata;


void setPWM(word frq, byte cycle);	 
void buzzer_start();
void buzzer_stop();

/**
Initializes the buzzer module.
*/
void buzzer_init(void)
{	
	//_TP5CE = 0; // TMP0 stop;
	//-- Counter frequency  --> fc=fxx/32
	//-- Bits in TP5CTL0 register
	_TP5CKS0 = 1;
	_TP5CKS1 = 0;
	_TP5CKS2 = 1;

	//-- Timer Mode --> PWM output mode
	//-- Bits in TP5CTL1 register
	_TP5MD0 = 0;
	_TP5MD1 = 0;
	_TP5MD2 = 1;
	_TP5EEE = 0;
	
	//-- TP5IOC0
	_TP5OL1 = 0;
	_TP5OE1 = 1;
	//-- TP5IOC1 has defualt value: 0x00
	
	//-- TP5IOC2 has defualt value: 0x00
	
	//-- TP5OPT0 has defualt value: 0x00
	
	
	TP5CCR0 = 304; // Frequenza di interrupt --> fi =  fc / (TP0CCR0 + 1)
	TP5CCR1 = 152;
	//-- Pin setting ---
	PinBUZZER = 0;
	_PF914 = 0;
	_PFCE914 = 1;
	_PFC914 = 1;
	_PMC914 = 1;
	ioPinBUZZER = 0;
	
	_TP5CE = 0; // TMP4 stop

	// Il timer viene inizializzato nel main insieme agli altri timer
	BUZZERdata.isPlaying = false;
	BUZZERdata.pwmCycle = 50;
	BUZZERdata.pwmFrq   = 325;//500;//2048;//8200;
	BUZZERdata.sound    = NO_SOUND;
	BUZZERdata.frqBase  = 20e+6/32.0;
	BUZZERdata.buzTimer.Stop();
	BUZZERdata.alarmTimer.Stop();
	BUZZERdata.length = 200;
	BUZZERdata.nota = _DO;
	BUZZERdata.numBeepTime = 1;
	BUZZERdata.numBeepTimeCount = 0;
	setPWM(BUZZERdata.pwmFrq, BUZZERdata.pwmCycle);
}

/**
Starts the buzzer
@param SoundToPlay sound to play
*/
void goBuzzer(KindOfSound SoundToPlay)
{

	if (!BUZZERdata.isPlaying)
	{
		BUZZERdata.numBeepTime = 1;
		BUZZERdata.numBeepTimeCount = 0;
		BUZZERdata.sound = SoundToPlay;
		switch (BUZZERdata.sound)
		{
			case KEY_LIKE :
				BUZZERdata.pwmCycle = 50;
				BUZZERdata.length = 50; 
				break;
			
			case LONG_LIKE :
				BUZZERdata.pwmCycle = 50;
				BUZZERdata.length = 600;	
				break;

			case ALARM_LIKE :
				BUZZERdata.pwmCycle = 3;
				BUZZERdata.length = 200;
				break;
			default :
				BUZZERdata.pwmCycle = 50;
				break;
		}
		buzzer_start();
	}
}

void goBuzzerTimes(KindOfSound SoundToPlay, int times)
{
	if (!BUZZERdata.isPlaying)
	{
		buzzer_stop();
		BUZZERdata.numBeepTime = times;
		BUZZERdata.numBeepTimeCount = 0;
		BUZZERdata.sound = SoundToPlay;
		switch (BUZZERdata.sound)
		{
			case KEY_LIKE :
				BUZZERdata.pwmCycle = 50;
				BUZZERdata.length = 200; 
				break;
			
			case LONG_LIKE :
				BUZZERdata.pwmCycle = 50;
				BUZZERdata.length = 600;	
				break;

			case ALARM_LIKE :
				BUZZERdata.pwmCycle = 50;
				BUZZERdata.length = 200;
				break;
			default :
				BUZZERdata.pwmCycle = 50; 
				break;
		}
		buzzer_start();
	}
}

/**
Call this function in the main loop. manage the buzzer module.
*/
void buzzerManager(void)
{
	KindOfSound sound;
	switch (BUZZERdata.sound)
	{
		case KEY_LIKE :
		case LONG_LIKE:
			if (BUZZERdata.buzTimer.Match())
			{
				sound = BUZZERdata.sound;
				buzzer_stop();	
				if((BUZZERdata.numBeepTimeCount < BUZZERdata.numBeepTime) )
				{
					BUZZERdata.isPlaying = true;
					BUZZERdata.sound = sound;
					BUZZERdata.alarmTimer.Preset(BUZZERdata.length);
				}
			}
			if(BUZZERdata.alarmTimer.Match())
			{
				buzzer_start();
			}
			break;
		case ALARM_LIKE:
			if (BUZZERdata.buzTimer.Match())
			{
				sound = BUZZERdata.sound;
				buzzer_stop();	
				if((BUZZERdata.numBeepTimeCount < BUZZERdata.numBeepTime) )
				{
					BUZZERdata.isPlaying = true;
					BUZZERdata.sound = sound;
					BUZZERdata.alarmTimer.Preset(BUZZERdata.length);
				}
			}
			
			if(BUZZERdata.alarmTimer.Match())
			{				
				buzzer_start();
			}
			
			break;
		default:
			if (BUZZERdata.buzTimer.Match() || _TP5CE == 1)
			{
				buzzer_stop();	 		
			}
		break;
	}	
}	

/**
stops the buzzer
*/
void buzzer_stop()
{
	BUZZERdata.isPlaying = false;
	BUZZERdata.sound = NO_SOUND;
	BUZZERdata.alarmTimer.Stop();
	BUZZERdata.buzTimer.Stop();
	_TP5CE = 0; // TMP4 stop
}

/**
Starts the buzzer.
*/
void buzzer_start()
{
	BUZZERdata.isPlaying = true;
	BUZZERdata.numBeepTimeCount++;
	setPWM(BUZZERdata.pwmFrq, BUZZERdata.pwmCycle);
	BUZZERdata.alarmTimer.Stop();
	BUZZERdata.buzTimer.Preset(BUZZERdata.length);
	_TP5CE = 1; // TMP4 start
}

/**
@return true if the buzzer is playing, false otherwise
*/
bool buzzer_isPlaying()
{
	return BUZZERdata.isPlaying;
}

/**
@return the last sound played
*/
KindOfSound	buzzerSound()
{
	return BUZZERdata.sound;
}

/**
Stops the buzzer
*/
void shutBuzzer()
{
	buzzer_stop();
}

/**
Sets PWM frequency and duty cycle.
*/
void setPWM(word frq, byte cycle)
{
	dword regVal;
	_TP5CE = 0; // TMP4 start
	regVal = (BUZZERdata.frqBase/((double) BUZZERdata.pwmFrq)) - 1;
	TP5CCR0 = regVal; // Frequenza pwm --> fi =  fc / (TP0CCR0 + 1)
	regVal = (regVal * BUZZERdata.pwmCycle);
	regVal = regVal /100;
	TP5CCR1 = regVal;
}







///////////////////////////////////////////////////////////////////////////////
//	1)
//	A causa della variabilità della lunghezza del giro di programma specie
//	con i cambi delle videate, non e possibile basare lo STOP del suono solo
//	sul giro do programma, ma bisogna usare gli INTERRUPTS associati agli
//	altri TIMERS.
//	2)
//	Per ora vado in PWM. Prossimo obiettivo SQUARE WAVE GENERATION
//
//////////////////////////////////////////////////////////////////////////////

























