/**
@file		uartFPGAdrv.h
@brief		drivers for the uarts devices implements on the FPGA
			
@author		Nicola Molinazzi
@date		13/07/2011
@version	01.00
*/


#ifndef __UART_FPGA_DRV__
#define __UART_FPGA_DRV__

#include "global.h"
#include "iodef.h"

/**
Uart devices implement on the fpga
*/
typedef enum
{
	FP_UART_NO_DEVICE = 0,
	FP_UART_PC_DEVICE,
	FP_UART_ADuC_DEVICE_VINF,
	FP_UART_ADuC_DEVICE_VV,
	FP_UART_ADuC_DEVICE_GEN,
	FP_UART_ADuC_DEVICE_P12,
	FP_UART_ADuC_DEVICE_P34,
	FP_UART_ADuC_DEVICE_P56,
	FP_UART_ADuC_DEVICE_P78,
	FP_UART_NUM_DEVICES,
}FP_UART_DEVICES;

/**
FPGA's uarts buadrates available
*/
typedef enum
{
	_FP_BAUD_NOT_SET = 0,
	_FP_BAUD_9600,
	_FP_BAUD_19200,
	_FP_BAUD_38400,
	_FP_BAUD_115200,
	_FP_BAUD_230400,
	_FP_BAUD_460800
}FP_UART_BAUDRATE;

/**
ADuC's state mode. The ADuC can be in boot mode or normal mode

@see ADuCs_bootMode
@see ADuCs_normalMode
@see ADuCsState
*/
typedef enum
{
	ADuC_State_Boot_Mode,
	ADuC_State_Normal_Mode,	
}ADuCs_State;

#define UART_RST					PinFPGA_DONE	/*< reset all uart devices, asserted low*/

#define PC_UART_BAUD_R0				_FP_PC_UART_BAUD_R0	/*< baudrate register 0*/
#define PC_UART_BAUD_R1				_FP_PC_UART_BAUD_R1	/*< baudrate register 1*/

#define PC_UART_RX_STATUS_REG		_FP_PC_UART_RX_STATUS_REG	/*< rx status register*/
#define PC_UART_TX_STATUS_REG		_FP_PC_UART_TX_STATUS_REG	/*< tx status register*/

#define PC_UART_SETTING_REG			_FP_PC_UART_SETTING			/*< uart setting register*/

#define PC_UART_RX_FIFO_STATUS_REG	_FP_PC_UART_RX_EMPTY			/*< Rx fifo's status*/

#define PC_UART_TX_FIFO_STATUS_REG	_FP_PC_UART_TX_FULL			  /*< Tx fifo's status*/	

#define PC_UART_DATA_REG			_FP_PC_UART_FIFO	


#define ADuC_UART_BAUD_R0				_FP_ADuC_UART_BAUD_R0		/*< baudrate register 0*/
#define ADuC_UART_BAUD_R1				_FP_ADuC_UART_BAUD_R1		/*< baudrate register 1*/

#define ADuC_UART_RX_STATUS_REG			_FP_ADuC_UART_RX_STATUS_REG	/*< rx status register*/
#define ADuC_UART_TX_STATUS_REG			_FP_ADuC_UART_TX_STATUS_REG	/*< tx status register*/

#define ADuC_UART_SETTING_REG			_FP_ADuC_UART_SETTING		/*< uart setting register*/

#define ADuC_VINF_UART_RX_FIFO_STATUS_REG	_FP_ADuC_VINF_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_VV_UART_RX_FIFO_STATUS_REG		_FP_ADuC_VV_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_GEN_UART_RX_FIFO_STATUS_REG	_FP_ADuC_GEN_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_P12_UART_RX_FIFO_STATUS_REG	_FP_ADuC_P12_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_P34_UART_RX_FIFO_STATUS_REG	_FP_ADuC_P34_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_P56_UART_RX_FIFO_STATUS_REG	_FP_ADuC_P56_UART_RX_EMPTY			/*< Rx fifo's status*/
#define ADuC_P78_UART_RX_FIFO_STATUS_REG	_FP_ADuC_P78_UART_RX_EMPTY			/*< Rx fifo's status*/

#define ADuC_VINF_UART_TX_FIFO_STATUS_REG	_FP_ADuC_VINF_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_VV_UART_TX_FIFO_STATUS_REG		_FP_ADuC_VV_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_GEN_UART_TX_FIFO_STATUS_REG	_FP_ADuC_GEN_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_P12_UART_TX_FIFO_STATUS_REG	_FP_ADuC_P12_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_P34_UART_TX_FIFO_STATUS_REG	_FP_ADuC_P34_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_P56_UART_TX_FIFO_STATUS_REG	_FP_ADuC_P56_UART_TX_FULL			/*< Tx fifo's status*/
#define ADuC_P78_UART_TX_FIFO_STATUS_REG	_FP_ADuC_P78_UART_TX_FULL			/*< Tx fifo's status*/

#define ADuC_VINF_UART_DATA_REG				_FP_ADuC_VINF_UART_FIFO
#define ADuC_VV_UART_DATA_REG				_FP_ADuC_VV_UART_FIFO
#define ADuC_GEN_UART_DATA_REG				_FP_ADuC_GEN_UART_FIFO
#define ADuC_P12_UART_DATA_REG				_FP_ADuC_P12_UART_FIFO
#define ADuC_P34_UART_DATA_REG				_FP_ADuC_P34_UART_FIFO
#define ADuC_P56_UART_DATA_REG				_FP_ADuC_P56_UART_FIFO
#define ADuC_P78_UART_DATA_REG				_FP_ADuC_P78_UART_FIFO




void fpga_uarts_init();

void fpga_uart_set_baud(FP_UART_DEVICES device, FP_UART_BAUDRATE baud);
FP_UART_BAUDRATE fpga_uart_get_baud( FP_UART_DEVICES device);

void fpga_uart_enable_device(FP_UART_DEVICES device);
void fpga_uart_disable_device(FP_UART_DEVICES device);

void fpga_uart_enable_tx(FP_UART_DEVICES device);
void fpga_uart_disable_tx(FP_UART_DEVICES device);

bool fpga_uart_rx_fifo_is_empty(FP_UART_DEVICES device);
bool fpga_uart_tx_fifo_is_full(FP_UART_DEVICES device);

byte fpga_uart_read(FP_UART_DEVICES device);
void fpga_uart_send_byte(FP_UART_DEVICES device, byte data);


void ADuCs_bootMode();
void ADuCs_normalMode();
ADuCs_State ADuCsState();


#endif


































