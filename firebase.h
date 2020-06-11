#ifndef FIREBASE_h
#define FIREBASE_h


#include "TEE_UC20.h"
#include "ssl.h"

#define CONTEX 1
#define CLI_ID 4

class FIREBASE 
{
public:
	FIREBASE();
	bool begin(String host,String auth);
	int connect();
	bool set(String thing,String data);
	bool setStr(String thing,String data);
	bool setInt(String thing,int data);
	bool setFloat(String thing,float data);
	bool setBool(String thing, bool data);
	String get(String thing);
	int getInt(String thing);
	float getFloat(String thing);
	
	String push(String thing,String data);
	String pushStr(String thing, String data);
	
	bool remove(String thing);
	bool close();
	bool  status();
	
private:
	String host_;
	String auth_;
	void  send_http(String http_type,String thing,String data);
};

#endif
