#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "LineNotify.h"

#define APN "internet"
#define USER ""
#define PASS ""
#define Button 2
INTERNET net;
LINE_NOTIFY line;

#define Authen  "Your Token" 

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}

void setup() 
{  
  pinMode(Button,INPUT);
  Serial.begin(9600);
  gsm.begin(&mySerial,9600);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
  gsm.PowerOn(); 
  while(gsm.WaitReady()){}
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN,USER,PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  line.begin(Authen);
  sendLine("Hello My name is UC20");
}

void loop() 
{
   if(!digitalRead(Button))
   {
     sendLine("Push Button");
   }

}
void sendLine(String msg)
{
  int ret = line.connect();
  if(ret==1)
  { 
      Serial.println("Connected");
      line.sendMessage(msg);
  }
  else
     Serial.println("Not Connect");
  line.close();
}
