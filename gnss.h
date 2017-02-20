#ifndef GNSS_h
#define GNSS_h

#include "TEE_UC20.h"


class GNSS
{
public:
	GNSS();
	bool Start();
	bool Stop();
	String GetPosition(unsigned char mode=2);
	bool EnableNMEA();
	bool DisableNMEA();
	String GetNMEA(String nmea);
};

#endif