/**
@file		UartFPGA.h
@brief		Classes to manage the fpga UART.

@author		Enrico
@date		03/09/2011
@version	01.1
*/

#ifndef _UARTFPGA_H_
#define _UARTFPGA_H_


	// mappiamo i registri con maschere relative xchè la macro reg è singola
	// registri aggiuntivi rispetto alla macro NEC

				// bits aggiuntivi per registro CTL1 (01h)
#define	UART_CTL1_TXF_CLR	0x80	// clear tx fifo
#define	UART_CTL1_RXF_CLR	0x40	// clear rx fifo
#define	UART_CTL1_FLW_ENA	0x10	// abilitazione controllo flusso

				// bits per registro STR1 (08h) status register 1
#define	UART_STR1_LRX	0x80		// 
#define	UART_STR1_URX	0x40		// 
#define	UART_STR1_RTS	0x20		// stato linea rts
#define	UART_STR1_PRX	0x10		// stato pin rx
#define	UART_STR1_RFERW	0x08		// receiver fifo error read/write (write operation performed while fifo was full)/(read operation performed while fifo was empty)
#define	UART_STR1_RFWFL	0x04		// receiver fifo warning full
#define	UART_STR1_RFFL	0x02		// receiver fifo full
#define	UART_STR1_RFFE	0x01		// receiver fifo empty  

				// bits per registro STR2 (09h) status register 2
#define	UART_STR2_LTX	0x80		// 
#define	UART_STR2_UTX	0x40		// 
#define	UART_STR2_CTS	0x20		// stato linea cts
#define	UART_STR2_PTX	0x10		// stato pin tx
#define	UART_STR2_TFERW	0x08		// transmitter fifo error read/write (write operation performed while fifo was full)/(read operation performed while fifo was empty)
#define	UART_STR2_TFWFL	0x04		// transmitter fifo warning full
#define	UART_STR2_TFFL	0x02		// transmitter fifo full
#define	UART_STR2_TFFE	0x01		// transmitter fifo empty  


// definizione del moduletto uart_module nella pld
// la conformazione dei registri è molto vicina a quella che troviamo nell mcu NEC
struct uart_fpga_t{
	byte	CTL0;
	byte	CTL1;
	byte 	CTL2;
	byte	OPT0;
	byte	OPT1;
	byte	STR;
	byte	RXD;
	byte	TXD;
	byte	STR1;
	byte	STR2;
	word	marker_f4cc;			// marker per periferica uart: 0xF4CC (non presente sul NEC)
};




/**
	UartDeviceAbstaction abstracts the microcontroller hardware UART.

	- Manages the UART initialization
	- Maganges the base I/O
	- Manages the transmission and receprions ISRs 
*/
class UartFPGA: public UartDeviceAbstract
{
	public:

		UartFPGA(volatile uart_fpga_t *pRegUart, UARTParity __parity, 
													UARTFrameBits __frame, 
													UARTStopBits __stopbit, 
													UARTBaudRate __baudrate,
													UARThwFlwCnt __hwFlwCtl,
													int __rxbuf_size, 
													int __txbuf_size);
		virtual ~UartFPGA();
		
		virtual bool SetBaudRate(UARTBaudRate baudrate, bool forcemodify = false);
		virtual bool SetHwFlwCtl(UARThwFlwCnt flwCtl, bool forcemodify = false);
		virtual void TurnOn();
		virtual void TurnOff();		

		
		void HwTransfer();						// esegue il trasferimento dati tra hw e buffers
												// da chiamare a polling o irq polling
	protected:

		void HwInit();

	private:
		volatile uart_fpga_t	*m_pRegUart;			// puntatore alla periferica hw (registri macchina)
		volatile uart_fpga_t	*m_pDebug;				// puntatore di debug nel caso venga passato un device
												// sbagliato
		
};


#endif




