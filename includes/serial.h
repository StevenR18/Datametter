#ifndef SERIAL_H
#define SERIAL_H
#include <Windows.h>

HANDLE openPortCOM(char * port,
		   int baudRate,
		   int dataBit,
		   int stopBit,
		   int parity,
		   int flowControl);
void readPortCOM(HANDLE * pHSerial, int dataType);

#endif
