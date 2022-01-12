/**
@file		UartNEC.h
@brief		Classes to manage the microntroller UART.

@author		Enrico
@date		03/09/2011
@version	01.1
*/

#ifndef _UARTNEC_H_
#define _UARTNEC_H_

				// bits per registro CTL0 (00h)
#define	UART_CTL0_PWR	0x80		// uart module enable
#define	UART_CTL0_TXE	0x40		// tx enable
#define	UART_CTL0_RXE	0x20		// rx enable
#define	UART_CTL0_DIR	0x10		// msb-lsb first
#define	UART_CTL0_PS1	0x08		// parity 
#define	UART_CTL0_PS0	0x04
#define	UART_CTL0_CL	0x02		// char len: 0=7bit, 1=8bit
#define	UART_CTL0_SL	0x01		// stop-bit len: 0=1stop, 1=2stop

				// bits per registro CTL1 (01h)
#define	UART_CTL1_CKS3	0x08		// prescaler, from fxx to fxx/1024
#define	UART_CTL1_CKS2	0x04		// viene usato come div0 verso il modulo sasc
#define	UART_CTL1_CKS1	0x02		
#define	UART_CTL1_CKS0	0x01		


				// bits per registro CTL2 (02h)
#define	UART_CTL2_BRG3	0x08		// baud rate generator
#define	UART_CTL2_BRG2	0x04		// viene usato come div1 verso il modulo sasc
#define	UART_CTL2_BRG1	0x02		
#define	UART_CTL2_BRG0	0x01		

				// bits per registro OPT0 (03h)
#define	UART_OPT0_SRF	0x80		// SBF reception flag
#define	UART_OPT0_SRT	0x40		// SBF reception trigger
#define	UART_OPT0_STT	0x20		// SBF transmission trigger
#define	UART_OPT0_SL2	0x10		// SBF transmit length selection
#define	UART_OPT0_SL1	0x08		// 
#define	UART_OPT0_SL0	0x04		
#define	UART_OPT0_TDL	0x02		// Transmit data level bit: 0=Normal output of transfer data, 1=Inverted output of transfer data
#define	UART_OPT0_RDL	0x01		// Receive data level bit: 0=Normal input of transfer data , 1=Inverted input of transfer data 	

				// bits per registro STR (04h) status register
#define	UART_STR_TSF	0x80		// Transfer status flag
#define	UART_STR_ST3	0x40		// spare
#define	UART_STR_ST2	0x20		// spare
#define	UART_STR_ST1	0x10		// spare
#define	UART_STR_ST0	0x08		// spare
#define	UART_STR_PE		0x04		// Parity error flag
#define	UART_STR_FE		0x02		// Framing error flag
#define	UART_STR_OVE	0x01		// Overrun error flag  



// definizione del moduletto uart_module nel microbo nec
struct uart_nec_t{
	byte	CTL0;
	byte	CTL1;
	byte 	CTL2;
	byte	OPT0;
	byte	OPT1;
	byte	STR;
	byte	RXD;
	byte	TXD;
};


/**
	UartDeviceAbstaction abstracts the microcontroller hardware UART.

	- Manages the UART initialization
	- Maganges the base I/O
	- Manages the transmission and receprions ISRs 
*/
class UartNEC: public UartDeviceAbstract
{
	public:

		UartNEC(volatile uart_nec_t *pRegUart, UARTParity __parity, 
													UARTFrameBits __frame, 
													UARTStopBits __stopbit, 
													UARTBaudRate __baudrate,
													int __rxbuf_size, 
													int __txbuf_size);
		virtual ~UartNEC();
		
		virtual bool SetParity(UARTParity parity, bool forcemodify = false);
		virtual bool SetFrameBits(UARTFrameBits framebits, bool forcemodify = false);
		virtual bool SetStopBits(UARTStopBits stopbits, bool forcemodify = false);
		virtual bool SetBaudRate(UARTBaudRate baudrate, bool forcemodify = false);

// cosa aggiungono ? 
//	    virtual void TurnOn(void);
//	    virtual void TurnOff(void);
		

		bool isr_txAvail() { return !m_txbuf.isempty(); }
		char isr_txGet() { return m_txbuf.getc_isr(); }

		int isr_rxPut(char c) { return m_rxbuf.putc_isr(c); }

	protected:

		void HwInit();

	private:
		volatile uart_nec_t	*m_pRegUart;			// puntatore alla periferica hw (registri macchina)
		volatile uart_nec_t	*m_pDebug;				// puntatore di debug nel caso venga passato un device
												// sbagliato
		
};


#endif




