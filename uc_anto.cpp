#include "uc_anto.h"
#include "uc_mqtt.h"

UCxMQTT mqtt;


UCxANTO::anto()
{
	//antocallback = func_null;
	//AntoIO::AntoIO(const char *user, const char *token, const char *thing, const char *client_id): 
    
	//mqtt(client_id, ANTO_BROKER, ANTO_BROKER_PORT),_user(user), _token(token), _thing(thing)
}

void  callback_(String topic ,char *playload,unsigned char length)
{
	Serial.println();
  Serial.println(F("%%%%%%%%%%%%%%%%%%%%%%%%%%%%"));
  Serial.print(F("Topic --> "));
  Serial.println(topic);
  playload[length]=0;
  String str_data(playload);
  Serial.print(F("Playload --> "));
  Serial.println(str_data);
}

void UCxANTO :: begin(String username,String password,String name)
{
	anto_USER = username;
	anto_PASS = password;
	anto_Thing = name;
	mqtt.callback = callback_;
}

bool UCxANTO :: connectServer()
{
	
	do
	{
		gsm.debug(F("Connect Server"));
		gsm.debug(F("wait connect"));
		if(mqtt.DisconnectMQTTServer())
		{
			mqtt.ConnectMQTTServer(ANTO_SERVER ,ANTO_PORT);
		}
		delay(500);
	}
	while(!mqtt.ConnectState());
	gsm.debug(F("Server Connected"));
	unsigned char ret = mqtt.Connect(anto_Thing,anto_USER,anto_PASS);
	Serial.println(mqtt.ConnectReturnCode(ret));
		if(ret==0)
		{
			
			return(true);
		}
		return(false);
}


void UCxANTO :: pub(String channel,String msg)
{
	String dat = "channel/"+anto_USER+"/"+anto_Thing+"/"+channel;
	mqtt.Publish(dat,msg);
}
void UCxANTO :: pub(String channel,int msg)
{
	String dat = "channel/"+anto_USER+"/"+anto_Thing+"/"+channel;
	mqtt.Publish(dat,String(msg));
}
void UCxANTO :: sub(String channel)
{
	String dat = "channel/"+anto_USER+"/"+anto_Thing+"/"+channel;
	mqtt.Subscribe(dat);
}
bool UCxANTO :: loop()
{
	mqtt.MqttLoop();
	if(!mqtt.ConnectState())
     return(true);
	return(false);
}














































