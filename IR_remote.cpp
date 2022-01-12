/**
@file		IR_remote.cpp
@brief		Classes that manage  IR remote
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/



#include "IR_remote.h"



IR_Key::IR_Key()
{
	setKeyState(KEY_RELEASED);
	m_keyPreviousState = KEY_RELEASED;
	setEnable(false);
	setCountinuousReading(false);
	setBeepEnable(false);
	m_continuosReadingTick = 0;
	m_continuosReadingTickCount = 2;
}

IR_Key::~IR_Key()
{

}

void IR_Key::setEnable(bool en, int time)
{
	if (time < 200)
		time = 200;

	KeyAbstract::setEnable(en, time);
	if (en)
	{
		m_readTimer.Preset(KeyAbstract::getReadTime());
	}else
	{
		m_readTimer.Stop();
	}
}

void IR_Key::setCountinuousReading(bool en, int time)
{
	if (time < 200)
		time = 200;
	if (en)
		m_continuosReadingTickCount = time / 100;
	
	KeyAbstract::setCountinuousReading(en, KeyAbstract::getReadTime());
}

void IR_Key::setKeyState(keyStates key)
{
	if (keyState() != KEY_IDLE)
		m_keyPreviousState = keyState();
	
	KeyAbstract::setKeyState(key);
}

bool IR_Key::pop(keyReadingValues &k)
{
	if(m_keyFifo.empty())
	{
		return false;
	}
	m_keyFifo.pop_front(k);
	return true;
}

bool IR_Key::push(keyReadingValues k)
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

void IR_Key::manager()
{
	if (!isEnable())
		return;

	if(1)//m_readTimer.Match())
	{
		if (keyState() == KEY_PRESSED)
		{
			
			if(m_keyPreviousState == KEY_RELEASED || (continuousReadingIsEnable() && m_continuosReadingTick >= m_continuosReadingTickCount))
			{
				m_continuosReadingTick = 0;
				push(KEY_PRESS);
			}
			
			if(continuousReadingIsEnable())
			{
				m_continuosReadingTick++;
			}
			
			m_readTimer.Preset(getReadTime());
		}else if (keyState() == KEY_RELEASED)
		{
			push(KEY_RELEASE);
			m_readTimer.Preset(getReadTime());
		}
		
	}

	if (m_readTimer.Match())
	{
		setKeyState(KEY_RELEASED);
		m_keyPreviousState = KEY_RELEASED;
		m_readTimer.Preset(getReadTime());
	}
}



IR_Remote::IR_Remote(int num_keys, IR_protocolAbstract* __IR_protocol): KeyboardAbstract(num_keys)
{
	int i;

	m_IR_protocol = __IR_protocol;
	
	m_keys = (KeyAbstract**) new IR_Key*[getKeysNumber()];
	for (i = 0; i < getKeysNumber(); i++)
	{
		m_keys[i] = new IR_Key();
	}

	clear();
}

/**
Class destructor.
*/
IR_Remote::~IR_Remote()
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
void IR_Remote::manager()
{
	int i;
	KeyAbstract::keyReadingValues keyRead;
	KeyAbstract::keyStates state;
	bool unknownKey;
	
	if (m_IR_protocol == NULL)
		return;
	
	m_IR_protocol->manager();

	IR_protocolAbstract::IR_data_t data;
	if(m_IR_protocol->pop_IR_Data(data))
	{


		/*
		char str[50];
		setcolor(BLACK);
		charset_select("pc6x8");
		gputsxy(30, 00, "          ");
		sprintf(str, "cc:%d", data.customCode);
		gputsxy(30, 00, str);


		gputsxy(30, 10, "          ");
		sprintf(str, "dc:%d", data.dataCode);
		gputsxy(30, 10, str);
		
		gputsxy(30, 20, "          ");
		sprintf(str, "err:%d", data.error);
		gputsxy(30, 20, str);
		*/
				
		if (data.error == IR_protocolAbstract::IRe_none)
		{
			if(data.customCode != 0)
			{
				unknownKey = true;
				
				for(i = 0; i < getKeysNumber(); i++)
				{
					if( m_keys[i]->keyNumber() == (int)data.dataCode)
					{
						/*gputsxy(30, 30, "          ");
						sprintf(str, "%d:%d", data.dataCode, m_keys[i]->keyNumber());
						gputsxy(30, 30, str);*/
						
						state = KeyAbstract::KEY_PRESSED;
						unknownKey = false;
					}
					else 
					{
						state = KeyAbstract::KEY_RELEASED;
					}
					m_keys[i]->setKeyState(state);
				}
				if(unknownKey)
					push(IRk_unknown);
			}
		}else
		{
			push(IRk_error);
		}

	}else
	{
		for(i = 0; i < getKeysNumber(); i++)
		{
			
			m_keys[i]->setKeyState(KeyAbstract::KEY_IDLE);
		}
	}
	
	for (i = 0; i < getKeysNumber(); i++)
	{
		m_keys[i]->manager();
		if (m_keys[i]->pop(keyRead))
		{
			if (keyRead == KeyAbstract::KEY_PRESS)
			{
				push(m_keys[i]->keyNumber());
				if(m_keys[i]->beepIsEnable())
				{
					goBuzzer(KEY_LIKE);
				}
			}
		}
	}
}



























