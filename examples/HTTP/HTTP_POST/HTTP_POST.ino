#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "call.h"
#include "sms.h"
#include "internet.h"
#include "File.h"
#include "http.h"

INTERNET net;
UC_FILE file;
HTTP http;

//SIM TRUE  internet
#define APN "internet"
#define USER "True"
#define PASS "true"

//SoftwareSerial mySerial(2, 3); // RX, TX
AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}
void data_out(char data)
{
  Serial.write(data);
}
void setup() 
{
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
  Serial.println(F("Start HTTP"));
  http.begin(1); 
  Serial.println(F("Send HTTP POST"));
  http.url("https://postman-echo.com/post");
  Serial.println(http.post("foo1=bar1&foo2=bar2")); // Body Raw Data
  
  /*
  // Example if you would like to include header
  gsm.println("AT+QHTTPCFG=\"requestheader\","); // enable requestheader 
  gsm.print(1,DEC); 
  gsm.println("");
  
  http.url("https://webhook.site/a7644aa5-5b26-4a63-a0df-ee654edbf253"); 

  String header[] = {"POST /a7644aa5-5b26-4a63-a0df-ee654edbf253 HTTP/1.1\r\n",
                     "Host: webhook.site\r\n",
                     "Content-Type: application/x-www-form-urlencoded\r\n",
                     "Content-Length: 23\r\n",
                     "\r\n"
                    };

  String pay_load = "key5=value5&key6=value6";     //content-length: 23
  String all_data = header[0] + header[1] + header[2] + header[3] + header[4] + pay_load ;
  http.post(all_data);
 */
  
  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM,"*");
  Serial.println(F("Save HTTP Response To RAM"));
  http.SaveResponseToMemory(RAM,"web.hml");
  Serial.println(F("Read data in RAM"));
  read_file(RAM,"web.hml");
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  
}
void read_file(String pattern,String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern,file_name);
}
void loop() 
{
  if (gsm.available())
  {
    Serial.write(gsm.read());
  } 
  if (Serial.available())
  {
    char c = Serial.read();
    gsm.write(c);
    
  } 
}
