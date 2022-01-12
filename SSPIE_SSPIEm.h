#ifndef _SSPIE_SSPIEM_H_
#define _SSPIE_SSPIEM_H_

int SSPIEm_preset(unsigned char *setAlgoPtr, unsigned int setAlgoSize, 
				  unsigned char *setDataPtr, unsigned int setDataSize);
int SSPIEm(unsigned int algoID);

#endif