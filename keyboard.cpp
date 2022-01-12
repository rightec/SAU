/**
@file		keyboard.cpp
@brief		Module to manage the board's keyboard
					
@author		
@date		18/01/2011
@version	01.00

@see Key
@see Keyboard
*/



#include "keyboard.h"




//-------------------------------------------------------
//----------------- CLASS Key ---------------------------
//-------------------------------------------------------
/**
Class contructor.

@param port pointer to the uC port that read the key
@param pin pin of the port that read the key
@param key king of key - key meaning or key name
*/
Key::Key(volatile unsigned char * port, byte bit, kindOfKey key): KeyAbstract()
{
	m_port = port;
	m_bit = bit;
	
	setEnable(false, 200);
	m_readTimer.Preset(getReadTime());
	//m_continuousReadTimer.Stop();
	setKeyState(KEY_RELEASED);
	m_keyFifo.clear();
	
}

/**
Class destructor.
*/
Key::~Key()
{
	
}

/**
Sets the key active according to the value of a
@param a true set the key as active with reading time time, false deactivete the key and the reading timer.
@time key reading time (msec)
*/
void Key::setEnable(bool en, int time)
{
	KeyAbstract::setEnable(en, time);
	if (en)
	{
		m_readTimer.Preset(KeyAbstract::getReadTime());
	}else
	{
		m_readTimer.Stop();
	}
}

/**
Sets the propriety continuous reading mode active according to c value

In countinuous reading mode if the key is kept pressed a KEY_PRESS value is pushed
in the reading buffer each time the time "time" elapsed. If not in contiunuous mode only the first time.
@param c true  key continuous mode reading is activate, false deactivate.
@param time countinuos mode read time (msec)
@see push
@see pop
*/
void Key::setCountinuousReading(bool en, int time)
{ 
	KeyAbstract::setCountinuousReading(en, time);
}

/**
Function to call in the main cycle loop. Manage the key reading time and propriety.
*/
void Key::manager()
{
	if (m_port == NULL)
	{
		return;
	}
	if (!isEnable())
		return;
	
	if (m_readTimer.Match())
	{	
		if (!pinValue() && (keyState() == KEY_RELEASED || continuousReadingIsEnable()))
		{
			setKeyState(KEY_PRESSED);
			push(KEY_PRESS);
			
		}else if (pinValue() && (keyState() == KEY_PRESSED || continuousReadingIsEnable()))
		{
			setKeyState(KEY_RELEASED);
			push(KEY_RELEASE);
		}
		m_readTimer.Preset(getReadTime());
	}
	
	
}

/**
Reads a key value from the reading buffer.

@param k key valued read
@return true if the value if valid (buffer not empty), false otherwise.
@see keyReadingValues
*/
bool Key::pop(keyReadingValues &k)
{
	if(m_keyFifo.empty())
	{
		return false;
	}
	m_keyFifo.pop_front(k);
	return true;
}

/**
Push a key value in the reading buffer 
@param k key value to push
@return true if the buffer is not full, false otherwise
@see keyReadingValues
*/
bool Key::push(keyReadingValues k)
{
	bool retVal = true;
	
	if(m_keyFifo.full())
	{
		keyReadingValues k1;
		m_keyFifo.pop_front(k1);
	}
	
	m_keyFifo.push_back(k);
	return retVal;
}

/**
macro that returns the pin state.
@return 1 pin high, 0 pin low
*/
byte Key::pinValue()
{
	return (((*m_port & (1 << m_bit)) == (1 << m_bit)) ? 1 : 0);
		
}



//--------------------------------------------------
//------------ CLASS Keyboard ----------------------
//--------------------------------------------------
/**
Class constructor.

@param num_keys number of keys of the keyboard.
*/

Keyboard::Keyboard(int num_keys): KeyboardAbstract(num_keys)
{
	int i;
	
	m_keys = (KeyAbstract**) new Key*[getKeysNumber()];
	for (i = 0; i < getKeysNumber(); i++)
	{
		m_keys[i] = new Key(NULL, 0, NO_KEY);
	}

	clear();
}

/**
Class destructor.
*/
Keyboard::~Keyboard()
{
	int i;
	for (i = 0; i < getKeysNumber(); i++)
	{
		delete m_keys[i];
	}
	delete m_keys;
}

/**
Function to call in the main cycle loop. Manage the keys reading and thier proprieties.
*/
void Keyboard::manager()
{
	int i;
	Key::keyReadingValues keyRead;
	
	for (i = 0; i < getKeysNumber(); i++)
	{
		m_keys[i]->manager();
		if (m_keys[i]->pop(keyRead))
		{
			if (keyRead == Key::KEY_PRESS)
			{
				push(m_keys[i]->keyNumber());
				if(m_keys[i]->beepIsEnable())
				{
					goBuzzer(KEY_LIKE);
				}
			}else
			{
				push(m_keys[i]->keyNumber() + 1);
			}
		}
	}
}

/**
Sets the key port and pin for the i-th key.

@param i key number
@param port pointer to the port that read the key
@param bit pin of the port that read the key
*/
void Keyboard::setKeyPin(int i, volatile unsigned char * port, byte bit)
{
	if (i >= getKeysNumber())
	{
		return;
	}
	((Key*)m_keys[i])->setKeyPin(port, bit);
}
















