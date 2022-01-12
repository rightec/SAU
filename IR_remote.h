/**
@file		IR_remote.h
@brief		Classes that manage  IR remote
			
@author		Nicola Molinazzi
@date		22/09/2011
@version	01.00
*/



#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__


#include "global.h"
#include "IR_protocolAbstarct.h"
#include "sound.h"
#include "swtim.h"
#include "KeyAbstract.h"
#include "graph.h"

enum IR_remote_Keys
{
	IRk_1 = 0b00011000,
	IRk_2 = 0b00100011,
	IRk_3 = 0b00011011,
	IRk_4 = 0b00100000,
	IRk_5 = 0b00101000,
	IRk_6 = 0b00100001,
	IRk_7 = 0b00100010,
	IRk_8 = 0b00000011,
	IRk_9 = 0b00100100,
	IRk_10 = 0b00100101,
	IRk_11 = 0b00100110,
	IRk_12 = 0b00010000,
	IRk_13 = 0b00010001,
	IRk_14 = 0b00010010,
	IRk_15 = 0b00001011,
	IRk_16 = 0b00100111,
	IRk_17 = 0b00000100,
	IRk_18 = 0b00101001,
	IRk_19 = 0b00101010,
	IRk_20 = 0b00101011,
	IRk_21 = 0b00101100,
	IRk_22 = 0b00101101,
	IRk_23 = 0b00101110,
	IRk_24 = 0b00101111,
	IRk_25 = 0b00110000,
	IRk_26 = 0b00110001,
	IRk_27 = 0b00110010,
	IRk_28 = 0b00110011,
	IRk_29 = 0b00110100,
	IRk_30 = 0b00110101,
	IRk_31 = 0b00110110,
	IRk_32 = 0b01111111,
	IRk_33 = 0b00111000,
	IRk_34 = 0b00111001,
	IRk_35 = 0b00111010,
	IRk_36 = 0b00111011,
	IRk_37 = 0b00111100,
	IRk_38 = 0b00111101,
	IRk_39 = 0b00111110,
	IRk_40 = 0b00111111,
	IRk_41 = 0b00000000,
	IRk_42 = 0b00000001,
	IRk_43 = 0b00000010,
	IRk_44 = 25,
	IRk_45 = 8,
	IRk_46 = 10,
	IRk_47 = 9,
	IRk_48 = 19,
	
	IRk_none = 250,
	IRk_unknown,
	IRk_error,
};

#define IRk_numOfKeys 48


class IR_Key: public KeyAbstract
{
	public:
		IR_Key();
		~IR_Key();

		virtual void setEnable(bool en, int time = 200);

		virtual void setCountinuousReading(bool en, int time = 100);
		
		virtual void setKeyState(keyStates key);
		
		virtual bool pop(keyReadingValues &k);
		
		virtual void manager();
		
	protected:
		virtual bool push(keyReadingValues k);

	private:
		DecTimer m_readTimer;
		
		keyStates m_keyPreviousState;
		
		int m_continuosReadingTick;
		int m_continuosReadingTickCount;
		
};

class IR_Remote: public KeyboardAbstract
{
	public:
		
		IR_Remote(int num_keys, IR_protocolAbstract* __IR_protocol);
		~IR_Remote();

		void manager();
		
	protected:
		
	private:
		IR_protocolAbstract *m_IR_protocol;

		
};

#endif













