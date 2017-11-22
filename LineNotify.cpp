#include "LineNotify.h"

SSL line_ssl;
unsigned char flag_line_error=1; 

LINE_NOTIFY::LINE_NOTIFY()
{
	
}
bool LINE_NOTIFY :: begin(String auth)
{
	int ret=0;
	Authen = auth;
	ret = line_ssl.sslversion(CONTEX,1);
	if(ret==1)
	{
		ret = line_ssl.ciphersuite(CONTEX,"0xFFFF");
		if(ret==1)
		{
			ret = line_ssl.seclevel(CONTEX,0);
			if(ret==1)
			{
				return(true);
			}
			else
			{
				gsm.debug (F("seclevel=  = Error"));
				return(false);
			}
		}
		else
		{
			gsm.debug (F("ciphersuite=  = Error"));
			return(false);
		}
			
	}
	else
	{
		gsm.debug (F("sslver = Error"));
		return(false);
	}	
	
}
int LINE_NOTIFY :: connect()
{
	return line_ssl.open(1,CONTEX,CLI_ID,F("notify-api.line.me"),F("443"),0);
}

bool LINE_NOTIFY :: close()
{
	return line_ssl.close(CLI_ID);
}

bool LINE_NOTIFY::sendMessage(String message)
{
	if(line_ssl.startSend(CLI_ID,0))
	{
		flag_line_error=0;
		send_http("POST",message);
		line_ssl.stopSend();
		line_ssl.waitRead(3000);
		int len = line_ssl.read(CLI_ID);
		while(len)
		{
			if(gsm.available())
			{
				String req = gsm.readStringUntil('\n');
				//Serial.println(req);
				if(req.indexOf(F("HTTP"))!= -1)
				{
					gsm.debug (req);
				}
				if(req.indexOf(F("OK"))!= -1)
				{
					len =0 ;
				}
			}
		}
		
	}
	else
	{
		flag_line_error=1;
		gsm.debug (F("OpenSend = Error"));
		return(false);
	}
}
void  LINE_NOTIFY :: send_http(String http_type,String data)
{
	String msg = "message="+data;
	String out = http_type+F(" /api/notify HTTP/1.1");
	gsm.println(out);
	gsm.println(F("Host: notify-api.line.me"));
	gsm.print(F("Authorization: Bearer "));
	gsm.println(Authen);
	gsm.println(F("Content-Type: application/x-www-form-urlencoded"));
	gsm.println(F("Cache-Control: no-cache"));
	gsm.print(F("content-length: "));
	gsm.println(String(msg.length()));
	gsm.println(F("Connection: keep-alive"));
	gsm.println("");
	gsm.println(msg);
	/*
	//out	+=  F("Authorization: Bearer ")+String(Authen)+"\r\n");
	//out += 	F("Content-Type: application/x-www-form-urlencoded\r\n");
	//out	+=	F("Cache-Control: no-cache\r\n");
	out	+=	F("content-length: ")+String(msg.length());
	out	+=	F("Connection: keep-alive\r\n");
	out	+= "\r\n";
	out	+= msg;
	*/
}




















