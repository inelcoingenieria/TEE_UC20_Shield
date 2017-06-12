#ifndef SSL_h
#define SSL_h

#include "TEE_UC20.h"


class SSL
{
public:
	SSL();
	bool sslversion(unsigned char contexid,unsigned char sslver);
	bool ciphersuite(unsigned char contexid,String tls_rsa);
	bool seclevel(unsigned char contexid,unsigned char level);
	int open(unsigned char pdpid,unsigned char contexid,unsigned char clientid,String serverid,String port,unsigned char acc_mode);
	bool close(unsigned char contexid);
	bool startSend(unsigned char contexid,int len);
	bool stopSend();
	bool waitRead(long time);
	bool state();
	int read(unsigned char contexid);
	
	
	int clear_buf(unsigned char contexid);

};

#endif