#include "ssl.h"


SSL::SSL()
{
	
}

bool SSL :: sslversion(unsigned char contexid,unsigned char sslver)
{
	//AT+QSSLCFG="sslversion",1,1
	gsm.print(F("AT+QSSLCFG=\"sslversion\","));
	gsm.println(String(contexid)+","+String(sslver));
	return(gsm.wait_ok_ndb(3000));
}

bool SSL :: ciphersuite(unsigned char contexid,String tls_rsa)
{
	gsm.print(F("AT+QSSLCFG=\"ciphersuite\","));
	gsm.println(String(contexid)+","+tls_rsa);
	return(gsm.wait_ok_ndb(3000));
}

bool SSL :: seclevel(unsigned char contexid,unsigned char level)
{
	gsm.print(F("AT+QSSLCFG=\"seclevel\","));
	gsm.println(String(contexid)+","+String(level));
	return(gsm.wait_ok_ndb(3000));
}

int SSL :: open(unsigned char pdpid,unsigned char contexid,unsigned char clientid,String serverid,String port,unsigned char acc_mode)
{
	//AT+QSSLOPEN=1,1,4,"test-491ef.firebaseio.com",443,2
	gsm.print(F("AT+QSSLOPEN="));
	gsm.println(String(pdpid)+","+String(contexid)+","+String(clientid)+",\""+serverid+"\","+port+","+String(acc_mode));

	if(gsm.wait_ok_ndb(10000))
	{
		const long interval = 5000; 
		unsigned long previousMillis = millis(); 
		unsigned char flag=1;
		
		while(flag)
		{
			if(gsm.available())
			{
				String req = gsm.readStringUntil('\n');
				if(req.indexOf(F("+QSSLOPEN"))!= -1)
				{
					//Serial.println(req);
					int index = req.indexOf(F(","));
					int res = req.substring(index+1).toInt();
					if(res == 0)
						return(1);
					else
						return(res);
				}
			}
			unsigned long currentMillis = millis();
			if(currentMillis - previousMillis >= interval) 
			{
				Serial.println("out");
				previousMillis = currentMillis;
				return(-1);
			}			
		}
	}
	Serial.print("open timeout");
	return(-2);
}

bool SSL :: close(unsigned char contexid)
{
	gsm.print(F("AT+QSSLCLOSE="));
	gsm.println(String(contexid));
	
	const long interval = 10000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			//Serial.println(req);
			if(req.indexOf(F("OK"))!= -1)
			{
				String req = gsm.readStringUntil('\n');
				return(true);
			}
			
			
		}
		
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			stopSend();
			Serial.println("close time out");
			previousMillis = currentMillis;
			return(false);
		}			
	}
	//return(gsm.wait_ok_ndb(15000));
}

bool SSL :: startSend(unsigned char contexid,int len)
{
	gsm.print(F("AT+QSSLSEND="));
	gsm.print(String(contexid));
	if(len>0)
	{
		gsm.print(",");
		gsm.print(String(len));
	}
	gsm.println("");
	
	const long interval = 5000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			char c = gsm.read(); 
			//Serial.write(c);
			if(c =='>')
			{
				//Serial.println("\r\n startsend ok");
				return(true);
			}
				
			
		}
		
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			gsm.write(0x1A);
			delay(1000);
			Serial.println("Start send out");
			previousMillis = currentMillis;
			return(false);
		}			
	}
}

 bool SSL :: stopSend()
{
	gsm.write(0x1A);
	const long interval = 10000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			//Serial.println(req);
			if(req.indexOf(F("SEND OK"))!= -1)
			{
					return(true);
			}
			if(req.indexOf(F("SEND FAIL"))!= -1)
			{
					return(false);
			}
		}
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			gsm.write(0x1A);
			delay(1000);
			Serial.println("out");
			previousMillis = currentMillis;
			return(false);
		}			
	}
}

bool SSL :: waitRead(long time)
{
	const long interval = time; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			//Serial.println(req);
			if(req.indexOf(F("+QSSLURC: \"recv\""))!= -1)
			{
				return(true);
				
			}
		
		}
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			Serial.println("Read Timeout");
			previousMillis = currentMillis;
			return(false);
		}			
	}
	
}

int SSL :: read(unsigned char contexid)
{
	gsm.print(F("AT+QSSLRECV="));
	gsm.print(String(contexid));
	gsm.println(",1500");
	
	const long interval = 3000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			//Serial.println(req);
			
			if(req.indexOf(F("+QSSLRECV:"))!= -1)
			{	
				int index = req.indexOf(":");
				int len = req.substring(index+2).toInt();
				//Serial.println("ssl read len");
				//Serial.println(len);
				return (len);
			}
		
		}
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			Serial.println("Read Timeout");
			previousMillis = currentMillis;
			return(false);
		}			
	}
	
}


bool SSL :: state()
{
	gsm.println(F("AT+QSSLSTATE"));
	const long interval = 5000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			Serial.println(req);
			if(req.indexOf(F("OK"))!= -1)
			{
					return(true);
			}
		
		}
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			Serial.println("out");
			previousMillis = currentMillis;
			return(0);
		}			
	}
}



int SSL :: clear_buf(unsigned char contexid)
{
	gsm.print(F("AT+QSSLRECV="));
	gsm.println(String(contexid)+",1500");
	
	const long interval = 5000; 
	unsigned long previousMillis = millis(); 
	unsigned char flag=1;
	while(flag)
	{
		if(gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			if(req.indexOf(F("OK"))!= -1)
			{
					return(1);
			}
			else
				Serial.print(req);
			
		}
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= interval) 
		{
			Serial.println("out");
			previousMillis = currentMillis;
			return(0);
		}			
	}
	
}












