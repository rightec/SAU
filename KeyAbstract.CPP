
/**
@file		KeyAbstract.cpp
@brief		Abstract key - keyboard
					
@author		
@date		27/09/2011
@version	01.00

@see KeyAbstract
@see KeyboardAbstract
*/



#include "KeyAbstract.h"


//--------------------------------------------------
//------------ CLASS Keyboard Abstract -------------
//--------------------------------------------------
/**
Class constructor.

@param num_keys number of keys of the keyboard.
*/

KeyboardAbstract::KeyboardAbstract(int num_keys)
{
	m_numKeys = num_keys;
}

/**
Class destructor.
*/
KeyboardAbstract::~KeyboardAbstract()
{

}


/**
Returns the key that was pressed

@param key kind of key pressed
@return true if valid key was pressed (buffer is nor empty), false otherwise.
*/	
bool KeyboardAbstract::pop(int &key)
{
	if (m_keysFifo.empty())
		return false;
	m_keysFifo.pop_front(key);
	return true;
}

/**
Sets the name of the i-th key
@param i key number
@param name kind of key 
@see kindOfKey
*/
void KeyboardAbstract::setKeyName(int i, int name)
{
	if (i >= m_numKeys)
	{
		return;
	}
	m_keys[i]->setKeyNumber((int)name);
}

/**
Returns the i-th key's type

@param i key numenr
@return kind of key
@see kindOfKey
*/
int  KeyboardAbstract::keyName(int i)
{
	if (i >= m_numKeys)
	{
		return -1;
	}
	return m_keys[i]->keyNumber();
}

/**
Returns if the i-th key is active

@param i key number
@return true if the i-th key is active, false otherwise
*/
bool KeyboardAbstract::isEnable(int i)
{
	if (i >= m_numKeys)
	{
		return false;
	}
	return m_keys[i]->isEnable();
}

/**
Sets the i-th key as active according to the value of a
@param key number
@param a true set the key as active with reading time time, false deactivete the key and the reading timer.
@time key reading time (msec)
*/
void KeyboardAbstract::setEnable(int i, bool a, int time)
{
	if (i >= m_numKeys)
	{
		return;
	}

	m_keys[i]->setEnable(a, time);
}

/**
@return true is the i-th key's continuous mode reading is activate, false otherwise.
*/
bool KeyboardAbstract::continuousReadingIsEnable(int i)
{
	if (i >= m_numKeys)
	{
		return false;
	}
	return m_keys[i]->continuousReadingIsEnable();
}

/**
Sets the propriety continuous reading mode active according to c value

In countinuous reading mode if the key is kept pressed a KEY_PRESS value is pushed
in the reading buffer each time the time "time" elapsed. If not in contiunuous mode only the first time.
@param i key numbber 
@param c true  the i-th key continuous mode reading is activate, false deactivate.
@param time countinuos mode read time (msec)
@see push
@see pop
*/
void KeyboardAbstract::setCountinuousReading(int i, bool c, int time)
{
	if (i >= m_numKeys)
	{
		return;
	}
	m_keys[i]->setCountinuousReading(c, time);
}

/**
@param i key number
@return true if when a key is pressed a bip is emitted, false othewise.
*/
bool KeyboardAbstract::beepIsEnable(int i)
{
	if (i >= m_numKeys)
	{
		return false;
	}
	return m_keys[i]->beepIsEnable();
}

/**
Enables the bip when the i-th key is pressed according to s
@param i key numner
@param s true enable the bip, false disable it.
*/
void KeyboardAbstract::setBeepEnable(int i, bool s)
{
	if (i >= m_numKeys)
	{
		return;
	}
	m_keys[i]->setBeepEnable(s);
}

/**
clears the keys reading buffer
*/
void KeyboardAbstract::clear()
{
	int i;
	for(i = 0; i < m_numKeys; i++)
	{
		m_keys[i]->clear();
	}
	m_keysFifo.clear();
}

/**
Push the key pressed in the buffer
@param key kind of key pressed
@return true is the buffer is not full, false otherwise
*/
bool KeyboardAbstract::push(int key)
{
	bool retVal = true;
	
	if (m_keysFifo.full())
	{
		int key2;
		m_keysFifo.pop_front(key2);
		retVal = false;
	}
	m_keysFifo.push_back(key);
	return retVal;
}

/**
Returns the state of the i-th key
@param i key number
@return key state
@see keyStates
*/
KeyAbstract::keyStates KeyboardAbstract::keyState(int i)
{
	if (i >= m_numKeys)
	{
		return KeyAbstract::KEY_RELEASED;
	}
	return m_keys[i]->keyState();
}

/**
Pop the reading values of the i-th key
@param i key number
@param k valued popped
@return true if the buffer is not empty (valid value), false otherwise
*/
bool KeyboardAbstract::popKey(int i, KeyAbstract::keyReadingValues &k)
{
	if (i >= m_numKeys)
	{
		return false;
	}
	return m_keys[i]->pop(k);
}

/**
Clears the i-th key buffer
@param i key number
*/
void KeyboardAbstract::clearKey(int i)
{
	if (i >= m_numKeys)
	{
		return;
	}
	m_keys[i]->clear();
}















