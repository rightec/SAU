/**
@file		KeyAbstract.h
@brief		Abstract key - keyboard
					
@author		
@date		27/09/2011
@version	01.00

@see KeyAbstract
@see KeyboardAbstract
*/



#ifndef __KEY_ABSTRACT_H__
#define __KEY_ABSTRACT_H__

#include "global.h"


class KeyAbstract
{
	public:
		/**
		Key State. This can be different from the key read values.
		*/
		typedef enum
		{
			KEY_RELEASED,
			KEY_PRESSED,
			KEY_IDLE,
			
		}keyStates;
		/**
		Key Value. It's the value pushed in the reading buffer.
		*/
		typedef enum
		{
			KEY_PRESS,
			KEY_RELEASE,
		}keyReadingValues;
		
		KeyAbstract(){setEnable(false); setBeepEnable(false); setCountinuousReading(false); setKeyNumber(0); clear();};
		~KeyAbstract(){};

		virtual void setEnable(bool en, int time = 200){m_enable = en; m_readTime = time;}
		virtual int getReadTime(){return m_readTime;}
		virtual void setReadTime(int time){m_readTime = time;}
		
		virtual bool isEnable(){return m_enable;}

		virtual void setBeepEnable(bool en){m_beepEnable = en;}
		virtual bool beepIsEnable(){return m_beepEnable;}

		virtual void setCountinuousReading(bool en, int time = 100) { m_continuousReading = en; if (en) m_readTime = time;}
		virtual bool continuousReadingIsEnable(){return m_continuousReading;}
		virtual int continuousReadingTime(){return m_readTime;}

		virtual void setKeyNumber(int number){m_keyNumber = number;}
		virtual int keyNumber(){return m_keyNumber;}

		virtual keyStates keyState(){ return m_keyState;};
		virtual void setKeyState(keyStates key){ m_keyState = key;};
		
		virtual bool pop(keyReadingValues &k){return false;};

		virtual bool push(keyReadingValues k){return false;};
		
		virtual void clear(){ m_keyFifo.clear(); m_keyState = KEY_RELEASED;};
		
		virtual void manager(){};
		
	protected:
		CSmallRingBuf <keyReadingValues, 10> m_keyFifo;
		
	private:
		bool m_enable;
		int m_readTime;
		
		bool m_beepEnable;

		bool m_continuousReading;

		int m_keyNumber;

		keyStates m_keyState;
		
		
};

/**
Manages the board's keyboard 
*/
class KeyboardAbstract
{
	public:
		
		KeyboardAbstract(int num_keys);
		~KeyboardAbstract();

		
		virtual void manager(){};
		
		virtual bool pop(int &key);

		virtual void setKeyName(int i, int name);
		virtual int keyName(int i);

		virtual bool isEnable(int i);
		virtual void setEnable(int i, bool a, int time = 200);

		virtual bool continuousReadingIsEnable(int i);
		virtual void setCountinuousReading(int i, bool c, int time = 200);

		virtual bool beepIsEnable(int i);
		virtual void setBeepEnable(int i, bool s);

		virtual void clear();
		
	protected:
		virtual bool push(int key);

		virtual  KeyAbstract::keyStates keyState(int i);
		virtual  bool popKey(int i, KeyAbstract::keyReadingValues &k);
		virtual void clearKey(int i);

		virtual int  getKeysNumber(){return m_numKeys;}
		/**
		Keys pointer
		*/
		KeyAbstract **m_keys;
		
	private:
		/**
		Number of keys of the keyboard
		*/
		int m_numKeys;

		/**
		King of key pressed buffer
		*/
		CSmallRingBuf <int, 10> m_keysFifo;
		
};


#endif
































