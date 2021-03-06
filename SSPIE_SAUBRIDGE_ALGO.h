/***************************************************************************************

	Lattice Semiconductor Corp.
	SSPI Embedded Algo File.

	CREATED BY:    ispVM System Version 18.0.1
	CREATION DATE: Wed Apr 18 12:19:51 2012

	CHAIN DESCRIPTION:
	1	LAXP2-5E	Slave SPI Erase,Program,Verify	E:\Lattice\SAUBridge\SAUBridge_05\SAUBridge_SAUBridge_05.jed

	The following constant variables should be stored in external data.

***************************************************************************************/

const int g_iAlgoSize  __attribute__ ((section (".message"))) = 683;
const unsigned char g_pucAlgoArray[ 683 ]  __attribute__ ((section (".message"))) = {
	
	0xA0, 0x43, 0x52, 0x45, 0x41, 0x54, 0x45, 0x44, 0x20, 0x42, 0x59, 0x3A, 0x20, 0x69, 0x73, 
	0x70, 0x56, 0x4D, 0x28, 0x52, 0x29, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x20, 0x56, 
	0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x31, 0x38, 0x2E, 0x30, 0x2E, 0x31, 0x0A, 0x43, 
	0x52, 0x45, 0x41, 0x54, 0x49, 0x4F, 0x4E, 0x20, 0x44, 0x41, 0x54, 0x45, 0x3A, 0x20, 0x30, 
	0x35, 0x2F, 0x31, 0x37, 0x2F, 0x31, 0x32, 0x20, 0x31, 0x32, 0x3A, 0x34, 0x32, 0x3A, 0x35, 
	0x36, 0x0A, 0x4C, 0x41, 0x58, 0x50, 0x32, 0x2D, 0x35, 0x45, 0x0A, 0x45, 0x3A, 0x5C, 0x4C, 
	0x61, 0x74, 0x74, 0x69, 0x63, 0x65, 0x5C, 0x53, 0x41, 0x55, 0x42, 0x72, 0x69, 0x64, 0x67, 
	0x65, 0x5C, 0x53, 0x41, 0x55, 0x42, 0x72, 0x69, 0x64, 0x67, 0x65, 0x5F, 0x30, 0x36, 0x5C, 
	0x53, 0x41, 0x55, 0x42, 0x72, 0x69, 0x64, 0x67, 0x65, 0x5F, 0x53, 0x41, 0x55, 0x42, 0x72, 
	0x69, 0x64, 0x67, 0x65, 0x5F, 0x30, 0x36, 0x2E, 0x6A, 0x65, 0x64, 0x0A, 0x53, 0x6C, 0x61, 
	0x76, 0x65, 0x20, 0x53, 0x50, 0x49, 0x20, 0x45, 0x72, 0x61, 0x73, 0x65, 0x2C, 0x50, 0x72, 
	0x6F, 0x67, 0x72, 0x61, 0x6D, 0x2C, 0x56, 0x65, 0x72, 0x69, 0x66, 0x79, 0x0A, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xA1, 0xA2, 0xFF, 0xFF, 0xFF, 0xFF, 0xA3, 0x04, 0x00, 0x00, 0xA4, 0x04, 0xA5, 
	0x02, 0xA6, 0x04, 0xA7, 0x00, 0xA0, 0xA1, 0xA8, 0x8E, 0xBE, 0x60, 0x10, 0x1F, 0x10, 0x12, 
	0x20, 0x22, 0x98, 0xFF, 0xFF, 0xFF, 0x13, 0x20, 0x21, 0xFF, 0xFF, 0xFF, 0xFF, 0x27, 0x01, 
	0x1F, 0x10, 0x12, 0x20, 0x22, 0xAC, 0xFF, 0xFF, 0xFF, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x4D, 
	0xFF, 0xFF, 0xFF, 0x13, 0x08, 0x21, 0x08, 0x22, 0x00, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x4D, 
	0xFF, 0xFF, 0xFF, 0x13, 0x08, 0x21, 0x02, 0x22, 0x00, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x24, 
	0xFF, 0xFF, 0xFF, 0x1F, 0x14, 0x40, 0x01, 0x10, 0x12, 0x20, 0x22, 0xC0, 0xFF, 0xFF, 0xFF, 
	0x1F, 0x14, 0x40, 0x01, 0x43, 0xB0, 0x09, 0x40, 0x64, 0x10, 0x12, 0x20, 0x22, 0x4A, 0xFF, 
	0xFF, 0xFF, 0x13, 0x01, 0x22, 0x80, 0x1F, 0x44, 0x10, 0x12, 0x20, 0x22, 0x78, 0xFF, 0xFF, 
	0xFF, 0x1F, 0x14, 0x10, 0x12, 0x20, 0x22, 0xC4, 0xFF, 0xFF, 0xFF, 0x1F, 0x14, 0x40, 0xC8, 
	0x01, 0x10, 0x12, 0x20, 0x22, 0xAC, 0xFF, 0xFF, 0xFF, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x4D, 
	0xFF, 0xFF, 0xFF, 0x13, 0x08, 0x21, 0x80, 0x22, 0x00, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x84, 
	0xFF, 0xFF, 0xFF, 0x1F, 0x26, 0x41, 0x92, 0x0F, 0x10, 0x12, 0x20, 0x22, 0xE6, 0xFF, 0xFF, 
	0xFF, 0x12, 0xFE, 0x04, 0x27, 0x04, 0x1F, 0x14, 0x43, 0x0A, 0x40, 0x01, 0x10, 0x12, 0x20, 
	0x22, 0x4A, 0xFF, 0xFF, 0xFF, 0x13, 0x01, 0x22, 0x80, 0x1F, 0x44, 0x42, 0x10, 0x12, 0x20, 
	0x22, 0x58, 0xFF, 0xFF, 0xFF, 0x12, 0x20, 0x27, 0x05, 0x1F, 0x14, 0x43, 0x0A, 0x40, 0x01, 
	0x10, 0x12, 0x20, 0x22, 0x4A, 0xFF, 0xFF, 0xFF, 0x13, 0x01, 0x22, 0x80, 0x1F, 0x44, 0x10, 
	0x12, 0x20, 0x22, 0x4D, 0xFF, 0xFF, 0xFF, 0x13, 0x08, 0x21, 0x80, 0x22, 0x00, 0x1F, 0x10, 
	0x12, 0x20, 0x22, 0x84, 0xFF, 0xFF, 0xFF, 0x1F, 0x26, 0x41, 0x92, 0x0F, 0x10, 0x12, 0x20, 
	0x22, 0x56, 0xFF, 0xFF, 0xFF, 0x13, 0xFE, 0x04, 0x27, 0x04, 0x1F, 0x14, 0x42, 0x10, 0x12, 
	0x20, 0x22, 0xE8, 0xFF, 0xFF, 0xFF, 0x13, 0x20, 0x27, 0x05, 0x1F, 0x10, 0x12, 0x20, 0x22, 
	0xA2, 0xFF, 0xFF, 0xFF, 0x12, 0x20, 0x27, 0x06, 0x1F, 0x14, 0x43, 0x0A, 0x40, 0x01, 0x10, 
	0x12, 0x20, 0x22, 0x4A, 0xFF, 0xFF, 0xFF, 0x13, 0x01, 0x22, 0x80, 0x1F, 0x44, 0x10, 0x12, 
	0x20, 0x22, 0x22, 0xFF, 0xFF, 0xFF, 0x13, 0x20, 0x27, 0x06, 0x1F, 0x10, 0x12, 0x20, 0x22, 
	0x4D, 0xFF, 0xFF, 0xFF, 0x13, 0x08, 0x21, 0x80, 0x22, 0x00, 0x1F, 0x10, 0x12, 0x20, 0x22, 
	0xF4, 0xFF, 0xFF, 0xFF, 0x1F, 0x14, 0x43, 0x0A, 0x40, 0x01, 0x10, 0x12, 0x20, 0x22, 0x4A, 
	0xFF, 0xFF, 0xFF, 0x13, 0x01, 0x22, 0x80, 0x1F, 0x44, 0x10, 0x12, 0x20, 0x22, 0x4D, 0xFF, 
	0xFF, 0xFF, 0x13, 0x08, 0x21, 0x80, 0x22, 0x00, 0x1F, 0x10, 0x12, 0x20, 0x22, 0x78, 0xFF, 
	0xFF, 0xFF, 0x1F, 0x14, 0x40, 0xC8, 0x01, 0x61 };
	
