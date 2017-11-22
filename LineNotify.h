#ifndef LINE_NOTIFY_h
#define LINE_NOTIFY_h

#include "TEE_UC20.h"
#include "ssl.h"

#define CONTEX 1
#define CLI_ID 4

class LINE_NOTIFY
{
public:
	LINE_NOTIFY();
	bool begin(String auth);
	int  connect();
	bool close();
	bool sendMessage(String message);
	void send_http(String http_type,String data);
	
	
private:
	String Authen;
	
	
	
};




#endif