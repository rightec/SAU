/**
@file		QueueUart.h
@brief		Classes to manage the microntroller UART.

@author		Nicola
@date		18/01/2011
@version	01.1
*/

#ifndef _UARTABSTRACT_H_
#define _UARTABSTRACT_H_

#include "global.h"
#include <string.h>

/**
RingBuffer creates a characters ring buffer

*/
class UartRingBuffer
{	public:
		UartRingBuffer(int __size);
		UartRingBuffer();
		~UartRingBuffer();

		int resize(int __newsize);			// ridimensiona il buffer di lavoro
		
		int read(char *__pBuf, int __count);
		int read_isr(char *__pBuf, int __count);
		
		int write(const char *__pBuf, int __count);
		int write_isr(const char *__pBuf, int __count);
	
		inline char getc() { 	register byte c = m_pBuf[m_idxGet++]; 
								if(m_idxGet >= m_size) 
								{
									m_idxGetRound++;	//incrementare prima m_idxGetRound per evitare errori di overrun a interrupt attivati
									m_idxGet = 0;
								}
								return c;
							}
		
		inline int putc(char __c) { m_pBuf[m_idxMem++] = __c; 
									if(m_idxMem >= m_size) 
									{
										m_idxMemRound++;
										m_idxMem = 0; 
									}
									return __c;
								  }

					// routines dedicate per il transfer da routines irq
		inline char getc_isr() { register byte c = m_pBuf[m_idxGet++];
									if(m_idxGet >= m_size) 
									{
										m_idxGetRound++;
										m_idxGet = 0;
									}
									return c; 
								}
					
		inline int putc_isr(char __c) { m_pBuf[m_idxMem++] = __c; 
										if(m_idxMem >= m_size)
										{
											m_idxMemRound++;
											m_idxMem = 0; 
										}
										return __c; }

		int puts( char *s );
		int puts_isr( char *s );
		
		int size() { return m_size; }
		
		int used();
		int used_isr();
		
		bool valid() { return m_pBuf != NULL; }
		int isvalid() { return m_pBuf != NULL; }
		
		int isempty() {return ((m_idxMem == m_idxGet) && (m_idxGetRound == m_idxMemRound));}
		int isempty_isr() { return ((m_idxMem == m_idxGet) && (m_idxGetRound == m_idxMemRound));}
		
		int isfull() { return (used() >= m_size); }
		int isfull_isr() { return (used_isr() >= m_size); }
		
		
		
		void clear() { asm("di"); m_count = m_idxMem = m_idxGet = m_idxMemRound = m_idxGetRound =0; asm("ei"); }
		void clear_isr(){m_count = m_idxMem = m_idxGet = m_idxMemRound = m_idxGetRound =0;};
	protected:
		void alloc(int __size);
		void destroy();
		void zeroIndex();

	private:
		char 	*m_pBuf;		// buffer dati allocato in dinamica
		int 	 m_size;		// buffer size paddato 
		int	 	 m_count;		// count locale per test
		int		 m_idxMem;		// indice dentro
		int		 m_idxGet;		// indice fuori
		int m_idxMemRound;
		int m_idxGetRound;
};


/**
	UartDeviceAbstaction abstracts the microcontroller hardware UART.

	- Manages the UART initialization
	- Maganges the base I/O
	- Manages the transmission and receprions ISRs 
*/
class UartDeviceAbstract
{
	public:

		/**
		Kind of parity
		*/
		enum UARTParity
		{
			ParityNone = 0,
			ParityZero,
			ParityOdd,
			ParityEven
		};

		/**
		Number of bits per frame
		*/
		enum UARTFrameBits
		{
			Frame7Bits = 0,
			Frame8Bits
		};

		/**
		Number of stops bits
		*/
		enum UARTStopBits
		{
			Stop1Bits = 0,
			Stop2Bits
		};

		/**
		UART baud rate
		*/
		enum UARTBaudRate
		{
			BR1200 = 1200,
			BR2400 = 2400,
			BR4800 = 4800,
			BR9600 = 9600,
			BR19200 = 19200,
			BR38400 = 38400,
			BR57600 = 57600,
			BR76800 = 76800,
			BR115200 = 115200,
			BR230400 = 230400,
			BR460800 = 460800,
		};

		/**
		UART flow control
		*/
		enum UARThwFlwCnt
		{
			hwFlwCtl_None = 0,
			hwFlwCtl_CtsRts,	
		};
		
		/**
		Uart Hardware error
		*/
		enum UARTErrors
		{
			ReceptionError = 0xff00,
			ParityError = 0x8000,
			FrameError = 0x4000,
			OverrunError = 0x2000,
			NoDevice = 0x1000,//0x0001,
			OverflowRxBuff = 0x0800,
			OverrunRxFifo = 0x0400,
			OverrunTxFifo = 0x0200,
			NoErrorUART = 0x0000,
		};
		
		UartDeviceAbstract(int __rxbuf_size, int __txbuf_size);

		virtual ~UartDeviceAbstract();

		void manager();

		/** gruppo funzioni virtuali per remapping in funzione all'hw
		*/
		
		virtual bool SetParity(UARTParity parity, bool forcemodify = false);
		/**
		@parity set/get methods
		*/
		UARTParity GetParity() { return m_Parity; }

		
		virtual bool SetFrameBits(UARTFrameBits framebits, bool forcemodify = false);
		/**
			@frames bits set/get methods
		*/
		UARTFrameBits GetFrameBits() { return m_FrameBits; }
		
		virtual bool SetStopBits(UARTStopBits stopbits, bool forcemodify = false);
		/**
			@the stop bits set/get methods
		*/
		UARTStopBits GetStopBits() { return m_StopBits; }
		
		
		virtual bool SetBaudRate(UARTBaudRate baudrate, bool forcemodify = false);
		/**
			@return the baud rate
		*/
		UARTBaudRate GetBaudRate() { return m_BaudRate; }

		
		virtual bool SetHwFlwCtl(UARThwFlwCnt flwCtl, bool forcemodify = false);
		/**
			@return the flow control
		*/
		UARThwFlwCnt GetHwFlwCtl() { return m_hwFlwCnt; }
		
		/**
			@transmit channel interface
		*/
		bool TransmissionBufferIsFull(){return m_txbuf.isfull();};
		void TransmitData(byte data);
		void TransmitData(const byte *pdata, int count);
		void TransmitString(const char *pdata);
		int GetTransmissionBufferSize()			{ return m_txbuf.size(); }
		int GetNumBytesInTransmissionBuffer()	{ return m_txbuf.used(); }
		int GetFreeBytesInTransmissionBuffer()	{ return m_txbuf.size() - m_txbuf.used(); }

		void ClearTransmissionBuffer();
		void SetReceptionError(UARTErrors new_error) { m_LastError |= (int)new_error; }
		int GetReceptionError() 				 { return m_LastError; }
		void ClearReceptionError() 					 { m_LastError = NoErrorUART; }


		/**
			@receive channel interface
		*/
		bool ReceptionBufferIsEmpty(){return m_rxbuf.isempty();};
		bool ReceiveData(char &data);
		bool ReceiveData(byte *pdata, int &count);
		int GetReceptionBufferSize();
		int GetNumBytesInReceptionBuffer();
		int GetFreeBytesInReceptionBuffer();
		void ClearReceptionBuffer();

		/**
		@return true if transmit buffer overflow error occurred.
		*/
		bool TransmitBufferOverflow(){return m_TransmitBufferOverflow; }
		/**
		@return true if reception buffer overflow error occurred.
		*/
		bool ReceptionBufferOverflow(){return m_ReceptionBufferOverflow; }

		/**
		Resets transmission buffer overflow error.
		*/
		void rstTransmitBufferOverflow(){ m_TransmitBufferOverflow = 0; }
		/**
		Resets reception buffer overflow error.
		*/
		void rstReceptionBufferOverflow(){m_ReceptionBufferOverflow = 0; }

		/**
			@return the last UART hardware error
		*/
		virtual void TurnOn() { m_Enabled = true; }
		virtual void TurnOff() { m_Enabled = false; }
		bool IsTransmissionBufferEmpty();

	protected:

		UartRingBuffer	m_txbuf;	// bufferini per rx & tx
		UartRingBuffer  m_rxbuf;

		UARTParity 		m_Parity;		// settaggi 
		UARTFrameBits 	m_FrameBits;
		UARTStopBits 	m_StopBits;
		UARTBaudRate 	m_BaudRate;		
		UARThwFlwCnt    m_hwFlwCnt;
		
		bool			m_Enabled;		// abilitazione sw
		bool			m_Init;			// inizializzazione hw
		
		/**
		Sets the Uart hardware error
		@param error uart hardware error.
		@see UARTErrors
		*/
		virtual void HwInit() { m_Init = True; }
		void usec_delay(int __req_delay);

	private:

		int		m_LastError;

		volatile bool 	m_RunningTransmission;
		volatile bool 	m_TransmitBufferOverflow;
		volatile bool 	m_ReceptionBufferOverflow;
		
};


#endif




