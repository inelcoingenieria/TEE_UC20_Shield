#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "uc_anto.h"
#include "DHT.h"
AltSoftSerial mySerial;
INTERNET net;
UCxANTO anto;

//SIM TRUE  internet
#define APN "internet"
#define USER ""
#define PASS ""

// anto
#define USERNAME    "your username"  
#define NAME        "name Thing"    
#define KEY         " Authen key "  



unsigned long previousMillis = 0;
const long interval = 2000;

#define LEDPIN 10
#define DHTPIN 2   
#define DHTTYPE DHT22    
DHT dht(DHTPIN, DHTTYPE);


void debug(String data)
{
  Serial.println(data);
}

void setup() 
{
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,HIGH);
  Serial.begin(9600);
  gsm.begin(&mySerial,9600);
  dht.begin();
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
  anto.begin(USERNAME,KEY,NAME);
  anto.func_map(sub_rx);
  connect_();
}

void loop()
{
  unsigned char connected_server = anto.loop();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {  
    previousMillis = currentMillis;
    Serial.println("Publish");
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    anto.pub("Humi",h);
    anto.pub("Temp",t);
  }
   if(connected_server==0)
      connect_();
}
void connect_()
{
  anto.connectServer();
  anto.sub("Ctrl_LED");
}
void sub_rx(String topic ,char *payloat,unsigned char length)
{
  payloat[length]=0;
  String str_payloat(payloat);
  if(topic.indexOf("Ctrl_LED")!=-1)
  {
       if(str_payloat.indexOf("1")!=-1)   
          digitalWrite(LEDPIN,LOW);
       if(str_payloat.indexOf("0")!=-1)   
          digitalWrite(LEDPIN,HIGH);
  }
}
