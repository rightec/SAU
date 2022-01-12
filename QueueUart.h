/**
@file		QueueUart.h
@brief		Classes to manage the microntroller UART.

@author		Nicola
@date		18/01/2011
@version	01.1
*/

#ifndef _QUEUEUART_H_
#define _QUEUEUART_H_

#include "global.h"
#include <string.h>
#include "uartFPGAdrv.h"




const static int BUFFER_SIZE = 2500;//64;

/**
RingBuffer creates a characters ring buffer

*/
class RingBuffer
{
	public:
		RingBuffer(unsigned size);
		~RingBuffer();

		void Clear(bool reset = false);
		void Insert(unsigned char data);
		bool Equals(const unsigned char *pdata, unsigned depth);
	private:
		unsigned m_BufferSize;
		unsigned char *m_pCurrent;
		unsigned char *m_pRingBuffer;
};


/**
	UartDeviceAbstaction abstracts the microcontroller hardware UART.

	- Manages the UART initialization
	- Maganges the base I/O
	- Manages the transmission and receprions ISRs 
*/
class UartDeviceAbstraction
{
	public:
		/**
		UART device
		*/
		enum UARTDevice
		{
			UART0 = 0,
			UART1,
			UART2,
			FPGA_UART_PC,
			FPGA_UART_ADuC_VINF,
			FPGA_UART_ADuC_VV,
			FPGA_UART_ADuC_GEN,
			FPGA_UART_ADuC_P12,
			FPGA_UART_ADuC_P34,
			FPGA_UART_ADuC_P56,
			FPGA_UART_ADuC_P78
		};

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
			BR76800 = 76800,
			BR115200 = 115200,
			BR230400 = 230400,
			BR460800 = 460800,
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
			NoErrorUART = 0x0000,
		};
		
		UartDeviceAbstraction(UARTDevice device);
		virtual ~UartDeviceAbstraction();

		void Manager();
		
		virtual void SetParity(UARTParity parity, bool forcemodify = false);
		/**
		@return the parity
		*/
		virtual UARTParity GetParity() { return m_Parity; }
		virtual void SetFrameBits(UARTFrameBits framebits, bool forcemodify = false);
		/**
			@return the frames bits
		*/
		virtual UARTFrameBits GetFrameBits() { return m_FrameBits; }
		virtual void SetStopBits(UARTStopBits stopbits, bool forcemodify = false);
		/**
			@return the stop bits
		*/
		virtual UARTStopBits GetStopBits() { return m_StopBits; }
		virtual void SetBaudRate(UARTBaudRate baudrate, bool forcemodify = false);
		/**
			@return the baud rate
		*/
		virtual UARTBaudRate GetBaudRate() { return m_BaudRate; }

		virtual void TransmitData(unsigned char data);
		virtual void SetTransmitCallBack(void (* pcallback)(UartDeviceAbstraction *pdevice));
		virtual void SetTransmitCallBackFpgaUart(void (* pcallback)(UartDeviceAbstraction *pdevice));
		
		virtual unsigned ReceiveData();
		virtual void SetReceiveCallBack(void (* pcallback)(UartDeviceAbstraction *pdevice, unsigned data));
		virtual void SetReceiveCallBackFpgaUart(void (* pcallback)(UartDeviceAbstraction *pdevice));
		/**
			@return the last UART hardware error
		*/
		virtual unsigned GetLastError() { return m_LastError; }
		virtual void TurnOn();
		virtual void TurnOff();
		bool IsTransmissionBufferEmpty();
		static void FpgaUartManager();
		FP_UART_DEVICES getFpgaDevice() {return m_fpgaDevice;};
		bool isFpgaDevice() {return m_isFpgaDevice;};
		
	protected:
		/**
		Sets the Uart hardware error
		@param error uart hardware error.
		@see UARTErrors
		*/
		virtual void SetError(int error){m_LastError = error;};
		virtual void ClearError();
		virtual void TransmitLock();
		virtual void TransmitUnlock();
		virtual void ReceiveLock();
		virtual void ReceiveUnlock();
		virtual void StartTransmission();
		virtual void StartTransmission(unsigned char data);
			
	private:
		volatile unsigned char  *m_pUAnCTL0;
		volatile unsigned char  *m_pUAnCTL1;
		volatile unsigned char  *m_pUAnCTL2;
		volatile unsigned char  *m_pUAnOPT0;
		volatile unsigned char  *m_pUAnSTR;
		volatile unsigned char  *m_pUAnRX;		
		volatile unsigned char  *m_pUAnTX;
		volatile unsigned char	*m_pUAnTIC;
		volatile unsigned char	*m_pUAnRIC;

		FP_UART_DEVICES m_fpgaDevice;
		bool m_isFpgaDevice;
		
		bool m_DeviceOwner;
		UARTDevice m_Device;
		UARTParity m_Parity;
		UARTFrameBits m_FrameBits;
		UARTStopBits m_StopBits;
		UARTBaudRate m_BaudRate;
		unsigned m_LastError;

		static UartDeviceAbstraction *m_pUart0DeviceAbstraction;
		static UartDeviceAbstraction *m_pUart1DeviceAbstraction;
		static UartDeviceAbstraction *m_pUart2DeviceAbstraction;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionPC;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_Vinf;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_VV;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_Gen;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_P12;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_P34;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_P56;
		static UartDeviceAbstraction *m_pFpgaUartDeviceAbstractionADuC_P78;
		
		void (* m_pTransmitCallback)(UartDeviceAbstraction *pdevice);
		void (* m_pReceiveCallback)(UartDeviceAbstraction *pdevice, unsigned data);

		void (* m_pTransmitCallbackFpgaUart)(UartDeviceAbstraction *pdevice);
		void (* m_pReceiveCallbackFpgaUart)(UartDeviceAbstraction *pdevice);

		void SelectUart0();
		void SelectUart1();
		void SelectUart2();
		void SelectUartFpgaPc();
		void SelectUartFpgaADuC_Vinf();
		void SelectUartFpgaADuC_VV();
		void SelectUartFpgaADuC_Gen();
		void SelectUartFpgaADuC_P12();
		void SelectUartFpgaADuC_P34();
		void SelectUartFpgaADuC_P56();
		void SelectUartFpgaADuC_P78();
		
		void Init();
		void SetTransmissionBuffer(unsigned char data);
		unsigned GetReceptionError();
		unsigned char GetReceptionBuffer();

		static void IrqHandUA0T(int vector);
		static void IrqHandUA1T(int vector);
		static void IrqHandUA2T(int vector);
		static void IrqHandUA0R(int vector);
		static void IrqHandUA1R(int vector);
		static void IrqHandUA2R(int vector);
		
};


/**
QueueUart builds a UART channels for the upper layers.

	- manages the transmission's and receprion's fifo.
	- manage the sequential access to the data.

*/
class QueueUart : protected UartDeviceAbstraction
{
	public:
		
		QueueUart(UARTDevice device);
		virtual ~QueueUart();
		virtual void TransmitData(unsigned char data);
		virtual void TransmitData(const unsigned char *pdata, int count);
		virtual void TransmitData(const unsigned char *pdata);
		virtual int GetTransmissionBufferSize();
		virtual int GetNumBytesInTransmissionBuffer();
		virtual int GetFreeBytesInTransmissionBuffer();
		
		virtual void SetTransmitCallBack(void (* pcallback)(UartDeviceAbstraction *pdevice, bool last));


		virtual void ClearReceptionBuffer();
		virtual void ClearTransmissionBuffer();
		virtual bool ReceiveData(unsigned short &data);
		virtual bool ReceiveData(unsigned char &data);
		virtual bool ReceiveData(unsigned char *pdata, int &count);
		virtual bool ReceiveData(unsigned short *pdata, int &count);
		virtual int GetReceptionBufferSize();
		virtual int GetNumBytesInReceptionBuffer();
		virtual int GetFreeBytesInReceptionBuffer();
		
		virtual void SetReceiveCallBack(void (* pcallback)(UartDeviceAbstraction *pdevice, unsigned data));

		/**
		Turns On the UART
		*/
		virtual void TurnOn(){UartDeviceAbstraction::TurnOn();};

		/**
		Turns Off the UART
		*/
		virtual void TurnOff(){UartDeviceAbstraction::TurnOff();};

		/**
		Sets the uart baudrate
		@param baudrate desired baudrate
		@see UARTBaudRate
		*/
		virtual void SetBaudRate(UARTBaudRate baudrate){UartDeviceAbstraction::SetBaudRate(baudrate, true);};
		
		virtual void resetFifo();
		
	protected:
		//const static unsigned BUFFER_SIZE = 64;
		virtual void StartTransmission();

		/**
		@return true if a transmission is running, false otherwise.
		*/
		bool RunningTransmission(){return m_RunningTransmission;};
		
		virtual void TransmitLock();
		virtual void TransmitUnlock();
		virtual void ReceiveLock();
		virtual void ReceiveUnlock();

		/**
		@return true if transmit buffer overflow error occurred.
		*/
		bool TransmitBufferOverflow(){return m_TransmitBufferOverflow;};
		/**
		@return true if reception buffer overflow error occurred.
		*/
		bool ReceptionBufferOverflow(){return m_ReceptionBufferOverflow;};

		/**
		Resets transmission buffer overflow error.
		*/
		void rstTransmitBufferOverflow(){ m_TransmitBufferOverflow = 0;};
		/**
		Resets reception buffer overflow error.
		*/
		void rstReceptionBufferOverflow(){m_ReceptionBufferOverflow = 0;};
		
	private:
		volatile unsigned m_TransmitLockDepth;
		volatile unsigned m_ReceiveLockDepth;
		volatile bool m_RunningTransmission;
		volatile bool m_TransmitBufferOverflow;
		volatile bool m_ReceptionBufferOverflow;

		/**
		Transmission Fifo
		*/
		CSmallRingBuf<unsigned char, BUFFER_SIZE> m_TransmissionBuffer;
		/**
		Reception Fifo
		*/
		CSmallRingBuf<unsigned short, BUFFER_SIZE> m_ReceptionBuffer;

		void (* m_pTransmitCallback)(UartDeviceAbstraction *pdevice, bool last);
		void (* m_pReceiveCallback)(UartDeviceAbstraction *pdevice, unsigned data);


		static void TransmitCallback(UartDeviceAbstraction *pdevice);
		static void ReceiveCallback(UartDeviceAbstraction *pdevice, unsigned data);
		static void TransmitCallbackFpgaUart(UartDeviceAbstraction *pdevice);
		static void ReceiveCallbackFpgaUart(UartDeviceAbstraction *pdevice);
};

#endif














































