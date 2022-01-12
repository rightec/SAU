/**
@file		uartFPGAdrv.cpp
@brief		drivers for the uarts devices implements on the FPGA
			
@author		Nicola Molinazzi
@date		13/07/2011
@version	01.00
*/

#include "uartFPGAdrv.h"

//------------- static function used only within the module ------
static void fpga_pc_uart_init();
static void fpga_ADuC_uart_init();


//------------ Global varibles used whitin the module --
FP_UART_BAUDRATE m_FpgaUartBaud[FP_UART_NUM_DEVICES]; /*< devices baudrate's array*/

volatile unsigned char *m_FpgaUartBaudReg0[FP_UART_NUM_DEVICES]; 
volatile unsigned char *m_FpgaUartBaudReg1[FP_UART_NUM_DEVICES];

volatile unsigned char *m_FpgaUartSettings[FP_UART_NUM_DEVICES];

volatile unsigned char *m_FpgaUartRxFifoStatus[FP_UART_NUM_DEVICES];
volatile unsigned char *m_FpgaUartTxFifoStatus[FP_UART_NUM_DEVICES];

volatile unsigned char *m_FpgaUartDataRegister[FP_UART_NUM_DEVICES];

ADuCs_State m_ADuCState;



/**
Init of all uart devices
*/
void fpga_uarts_init()
{
	int i;
	UART_RST = 0; // reset all uarts
	for(i = 0; i< 100; i++)
		asm("nop");

	
	for (i = 0; i < FP_UART_NUM_DEVICES; i++)
	{
		m_FpgaUartBaud[i] = _FP_BAUD_NOT_SET;
	}

	m_FpgaUartBaudReg0[FP_UART_NO_DEVICE] = NULL;
	m_FpgaUartBaudReg1[FP_UART_NO_DEVICE] = NULL;

	m_FpgaUartBaudReg0[FP_UART_PC_DEVICE] = &PC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_PC_DEVICE] = &PC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_PC_DEVICE] = &PC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_PC_DEVICE] = &PC_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_PC_DEVICE] = &PC_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_PC_DEVICE] = &PC_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_VINF] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_VINF] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_VINF] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_VINF] = &ADuC_VINF_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_VINF] = &ADuC_VINF_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_VINF] = &ADuC_VINF_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_VV] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_VV] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_VV] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_VV] = &ADuC_VV_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_VV] = &ADuC_VV_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_VV] = &ADuC_VV_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_GEN] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_GEN] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_GEN] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_GEN] = &ADuC_GEN_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_GEN] = &ADuC_GEN_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_GEN] = &ADuC_GEN_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_P12] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_P12] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_P12] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_P12] = &ADuC_P12_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_P12] = &ADuC_P12_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_P12] = &ADuC_P12_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_P34] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_P34] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_P34] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_P34] = &ADuC_P34_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_P34] = &ADuC_P34_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_P34] = &ADuC_P34_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_P56] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_P56] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_P56] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_P56] = &ADuC_P56_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_P56] = &ADuC_P56_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_P56] = &ADuC_P56_UART_DATA_REG;

	m_FpgaUartBaudReg0[FP_UART_ADuC_DEVICE_P78] = &ADuC_UART_BAUD_R0;
	m_FpgaUartBaudReg1[FP_UART_ADuC_DEVICE_P78] = &ADuC_UART_BAUD_R1;
	m_FpgaUartSettings[FP_UART_ADuC_DEVICE_P78] = &ADuC_UART_SETTING_REG;
	m_FpgaUartRxFifoStatus[FP_UART_ADuC_DEVICE_P78] = &ADuC_P78_UART_RX_FIFO_STATUS_REG;
	m_FpgaUartTxFifoStatus[FP_UART_ADuC_DEVICE_P78] = &ADuC_P78_UART_TX_FIFO_STATUS_REG;
	m_FpgaUartDataRegister[FP_UART_ADuC_DEVICE_P78] = &ADuC_P78_UART_DATA_REG;
	
	UART_RST = 1;
	for(i = 0; i< 100; i++)
		asm("nop");
	
	fpga_pc_uart_init();
	fpga_ADuC_uart_init();

	m_ADuCState = ADuC_State_Normal_Mode;
}

/**
Init PC communication's uart 
*/
static void fpga_pc_uart_init()
{
	fpga_uart_set_baud(FP_UART_PC_DEVICE, _FP_BAUD_460800);
	
	fpga_uart_disable_device(FP_UART_PC_DEVICE);
	fpga_uart_disable_tx(FP_UART_PC_DEVICE);
}

/**
Init ADuC communication's uart 
*/
static void fpga_ADuC_uart_init()
{
	int i;
	for (i = FP_UART_ADuC_DEVICE_VINF; i< FP_UART_ADuC_DEVICE_P78 + 1; i++)
	{
		fpga_uart_set_baud((FP_UART_DEVICES)i, _FP_BAUD_115200);
	
		fpga_uart_disable_device((FP_UART_DEVICES)i);
		fpga_uart_disable_tx((FP_UART_DEVICES)i);
	}
}

/**
Sets the device's  baudrate
@param device uart device
@param baud desired baudrate 

#see FP_UART_DEVICES
@see FP_UART_BAUDRATE
*/
void fpga_uart_set_baud(FP_UART_DEVICES device, FP_UART_BAUDRATE baud)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return;
	
	m_FpgaUartBaud[device] = baud;
	switch (baud)
	{
		case _FP_BAUD_NOT_SET:
			*m_FpgaUartBaudReg0[device] = 255;
			*m_FpgaUartBaudReg1[device] = 255; 
			break;
			
		case _FP_BAUD_9600: // real baud: 9635, error(%): +0.37%
			*m_FpgaUartBaudReg0[device] = 5;
			*m_FpgaUartBaudReg1[device] = 87;//86; 
			break;
			
		case _FP_BAUD_19200: // real baud: 19 380, error(%): +0.94%
			*m_FpgaUartBaudReg0[device] = 4;
			*m_FpgaUartBaudReg1[device] = 51; 
			break;
			
		case _FP_BAUD_38400: // real baud: 39 070, error(%): +1.75%
			*m_FpgaUartBaudReg0[device] = 4;
			*m_FpgaUartBaudReg1[device] = 25; 
			break;
			
		case _FP_BAUD_115200: // real baud: 116 300, error(%): +0.96%  
			*m_FpgaUartBaudReg0[device] = 7;
			*m_FpgaUartBaudReg1[device] = 5; 
			break;
			
		case _FP_BAUD_230400: // real baud: 238 095, error(%): +3.4%  
			*m_FpgaUartBaudReg0[device] = 2;
			*m_FpgaUartBaudReg1[device] = 6; 
			break;
			
		case _FP_BAUD_460800: // real baud: 454 545, error(%): -1.36%  
			*m_FpgaUartBaudReg0[device] = 3;
			*m_FpgaUartBaudReg1[device] = 2; 
			break;
	}
}

/**
Returs the devices's baudrate

#param device device name
#see FP_UART_DEVICES
*/
FP_UART_BAUDRATE fpga_uart_get_baud( FP_UART_DEVICES device)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return _FP_BAUD_NOT_SET;
	
	return m_FpgaUartBaud[device];
}

/**
Enables the uart device

#param device device name
#see FP_UART_DEVICES
*/
void fpga_uart_enable_device(FP_UART_DEVICES device)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return;

	*m_FpgaUartSettings[device] |= 0x02;
}

/**
Disables the uart device

#param device device name
#see FP_UART_DEVICES
*/
void fpga_uart_disable_device(FP_UART_DEVICES device)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return;

	*m_FpgaUartSettings[device] &= ~0x02;
}

/**
Enables uart device's tx signal

#param device device name
#see FP_UART_DEVICES
*/
void fpga_uart_enable_tx(FP_UART_DEVICES device)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return;

	*m_FpgaUartSettings[device] &= ~0x01;
}

/**
Disables uart device's tx signal

#param device device name
#see FP_UART_DEVICES
*/
void fpga_uart_disable_tx(FP_UART_DEVICES device)
{
	if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return;

	*m_FpgaUartSettings[device] |= 0x01;
}

/**
Returns if the rx fifo of the device is empty

#param device device name
@return true is the rx fifo is empty
#see FP_UART_DEVICES
*/
bool fpga_uart_rx_fifo_is_empty(FP_UART_DEVICES device)
{
	/*if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return true;
	*/
	return (*m_FpgaUartRxFifoStatus[device] == 0x81);
}

/**
Returns if the tx fifo of the device is full

#param device device name
@return true is the tx fifo is full
#see FP_UART_DEVICES
*/
bool fpga_uart_tx_fifo_is_full(FP_UART_DEVICES device)
{
	/*if ((device == FP_UART_NO_DEVICE) || (device >= FP_UART_NUM_DEVICES))
		return true;
	*/
	return (*m_FpgaUartTxFifoStatus[device] == 0x81);
}

/**
Returs the data in the rx fifo. Before to call this funcion verify that the fifo is not empty.

#param device device name
@return the first rx fifo's first data
#see FP_UART_DEVICES
#see fpga_uart_rx_fifo_is_empty
*/
byte fpga_uart_read(FP_UART_DEVICES device)
{
	return *m_FpgaUartDataRegister[device];
}

/**
Write a data in the tx fifo. As soos as the data is written the transmissio starts.
Before to call this funcion verify that the fifo is not full.

#param device device name
@return true is the rx fifo is empty
#see FP_UART_DEVICES
#see fpga_uart_tx_fifo_is_full
*/
void fpga_uart_send_byte(FP_UART_DEVICES device, byte data)
{
	*m_FpgaUartDataRegister[device] = data;
}


/**
Puts the ADuCs in boot mode
*/
void ADuCs_bootMode()
{
	int i;
	
	setBit(FPGA_reg_02, _FP_ADuCs_NTRST);	//NTRST high
	clrBit(ioFPGA_reg_02, _FP_ADuCs_NTRST);	//output

	setBit(FPGA_reg_02, _FP_ADuCs_RST);		//RST high
	clrBit(ioFPGA_reg_02, _FP_ADuCs_RST);	//output

	for (i = 0; i < 10000; i++) // wait...
	{
		asm("nop");
	}
	clrBit(FPGA_reg_02, _FP_ADuCs_NTRST);	//NTRST low

	for (i = 0; i < 10000; i++) // wait...
	{
		asm("nop");
	}
	clrBit(FPGA_reg_02, _FP_ADuCs_RST);	//RST low

	for (i = 0; i < 10000; i++) // wait...
	{
		asm("nop");
	}
	setBit(FPGA_reg_02, _FP_ADuCs_RST);	//RST high

	for (i = 0; i < 100000; i++) // wait...
	{
		asm("nop");
	}
	setBit(FPGA_reg_02, _FP_ADuCs_NTRST);	//NTRST high
	
	for (i = 0; i < 1000; i++)	//wait...
	{
		asm("nop");
	}
	setBit(ioFPGA_reg_02, _FP_ADuCs_RST);	//rst as input
	m_ADuCState = ADuC_State_Boot_Mode;
}

/**
Puts the ADuCs in normal mode
*/
void ADuCs_normalMode()
{
	int i,k;
	
	setBit(FPGA_reg_02, _FP_ADuCs_NTRST);	//NTRST high
	clrBit(ioFPGA_reg_02, _FP_ADuCs_NTRST);	//output

	setBit(FPGA_reg_02, _FP_ADuCs_RST);		//RST high
	clrBit(ioFPGA_reg_02, _FP_ADuCs_RST);	//output

	for (i = 0; i < 100000; i++) // wait...
	{
		asm("nop");
	}
	clrBit(FPGA_reg_02, _FP_ADuCs_RST);		//RST low

	for (i = 0; i < 100000; i++) // wait...
	{
		for (k = 0; k < 30; k++)
		{
			asm("nop");
			asm("nop");
		}
	}
	
	
	setBit(FPGA_reg_02, _FP_ADuCs_RST);		//RST high

	for (i = 0; i < 10000; i++)	//wait...
	{
		asm("nop");
	}
	setBit(ioFPGA_reg_02, _FP_ADuCs_RST);	//rst as input
	m_ADuCState = ADuC_State_Normal_Mode;
}

/**
@return the ADuCs current state
*/
ADuCs_State ADuCsState()
{
	return m_ADuCState;
}























