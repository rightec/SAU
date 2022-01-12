/**
@file		SAU7000_DOC.cpp
@brief		SAU-7000 Board Firmware Documentation 
@author		Nicola Molinazzi
@date		18/01/2011

<BR>
This file does not contain any souirce code, but it as to be always part of the project source files, because it is the main page 
of the html documentation.
*/

/**
@mainpage SAU7000

@section project_data General information
- Typology: SAU-7000 board Application firmware 
- Microcontroller: NEC - V850ES/JG2 - uPD70F3719GC
- Toolchain: GNU Binutils 2.18 - Gcc 4.3.1 cross compiled for NEC/V850
- Integrate Development Environment (IDE): Source Insight 3.50.0058
- Project Manager: Nicola Molinazzi


@section gen_description General Description

SAU-7000 is the board used during Urodymanics Stationary examination.
The board implemets several serial communication interfaces: one with the PC using a FTDI UART to USB chip converter or bluethoot device, with the 
microcontrolles ADuC7061 used to sampled the analog channels and one with and external High Resolotion EMG board. 
In addition on the board can control two motors (puller and peristaltic pump), a clamp (bubble detector), display interface and remote control
interface.
Following a brief description of the firmware organization for a better scrolling of this documentation.
- main.h: this file contians all uC initialization function and the main loop cycle.
- SAU7000_Manager: manages the uC states machine. 
- SauPcProtocol: manages the communication protocol with the PC
- uC_Protocol: manages the communication protocol with the ADuC7061 and the HR-EMG board.
- adc.h: manages  internal adc.
- Channels.h: classes to manage analog channel.
- keyboard.h: manages the device's keyboard.
- sound.h: handler of the board's buzzer.
- swtim.h: implements a series of classes of software timer.

@section history Revisions

Project's revision history can be found at the following link \ref rev_history.

*/

/**
@page rev_history Revision History

@section V1_00_000 V 1.00.000 - 18/01/2011

First revision
*/













































