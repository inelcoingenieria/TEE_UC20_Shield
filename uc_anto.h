#ifndef uc_anto_h
#define uc_anto_h

#include "TEE_UC20.h"

#define ANTO_SERVER      "service.anto.io"
#define ANTO_PORT        "1883"
#define ANTO_ID          "3G"




class UCxANTO
{
	public:
	UCxANTO();
	void begin(String username,String password,String name_);
	bool connectServer();
	void pub(String channel,String msg);
	void pub(String channel,int msg);
	void sub(String channel);
	bool loop();
	void func_map(void (*callback)(String topic ,char *playload,unsigned char length));
	void mqtt_sub(String topic ,char *playload,unsigned char length);
	private:
	String anto_USER;
	String anto_PASS;
	String anto_Thing;
	//void callback_(String topic ,char *playload,unsigned char length);

};


































#endif
