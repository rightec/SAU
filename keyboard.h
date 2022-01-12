/**
@file		keyboard.h
@brief		Mobule to manage the board's keyboard
					
@author		
@date		18/01/2011
@version	01.00
*/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_


#include "global.h"				
#include "swtim.h"
#include "sioSG2.h"
#include "sound.h"
#include "alloc.h"
#include "keyAbstract.h"

/*#define key_STOP	  	PinKey0
#define key_PAUSE     	PinKey1
#define key_START     	PinKey2
#define key_LES_UES   	PinKey3
#define key_SW		  	PinKey4
#define key_RAIR      	PinKey5
#define key_AS        	PinKey6
#define key_SQ			PinKey7
#define key_PS      	PinKey8
#define key_COMP		PinKey9
#define key_M1			PinKey10
#define key_PLUS      	PinKey11
#define key_MINUS      	PinKey12
*/

/**
Keys read time
*/
#define READ_KEY_TIME	200

/**
Kind of keys
*/
enum kindOfKey
{
	NO_KEY = -1,
	KEY_ON_OFF = 1,
	KEY_NUM_OF_KEYS,
};




/**
Class to manage a single key of a keyboard. 
*/
class Key: public KeyAbstract
{
	public:	
		
		Key(volatile unsigned char *port = NULL, byte pin = 0, kindOfKey key = NO_KEY);
		~Key();

		/**
		Sets the key port and bit within th port.
		@param port pointer to the uC port that read the key
		@param bit pin of the port that read the key
		*/
		void setKeyPin(volatile unsigned char * port, byte bit){ m_port = port; m_bit = bit;};

		virtual void setEnable(bool en, int time = 200);
		virtual void setCountinuousReading(bool en, int time = 200);
			
		void manager();

		virtual bool pop(keyReadingValues &k);

		
	protected:
		
		byte pinValue();
		virtual bool push(keyReadingValues k);
		
	private:
		volatile unsigned char * m_port;
		byte m_bit;
		
		//DecTimer m_continuousReadTimer;
		
		DecTimer m_readTimer;
		
};

class Keyboard: public KeyboardAbstract
{
	public:
		
		Keyboard(int num_keys);
		~Keyboard();

		void manager();

		void setKeyPin(int i, volatile unsigned char *port, byte bit);

	protected:
		
	private:
		
};


extern Keyboard *myKeyboard;




#endif

























