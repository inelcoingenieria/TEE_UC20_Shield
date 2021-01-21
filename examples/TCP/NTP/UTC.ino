#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "tcp.h"
#include <TimeLib.h> // Download TimeLib Library from https://github.com/PaulStoffregen/Time

INTERNET net;
TCP tcp;

//SIM TRUE  internet
#define APN "internet"
#define USER ""
#define PASS ""

tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup()
{
  Serial.begin(9600);
  gsm.begin(&mySerial, 9600);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}

  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());

  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());


}

String Time_NTP;

void loop()
{
  Time_NTP = tcp.NTP(1, "time2.navy.mi.th", "123");
  UTC_adjust();

  unix_timestamp =  makeTime(my_time);
  unsigned long unix_timestamp_UTC = unix_timestamp + 25200; // 25200 sec = +7 hours UTC Bangkok
  setTime(unix_timestamp_UTC);

  Serial.print("Date: ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());

  delay(500);
}


void UTC_adjust() {
  String Date_separate = getValue(Time_NTP, ',', 1 );
  String Date = Date_separate.substring(1, 11);

  String YY = getValue(Date, '/', 0 );
  String MM = getValue(Date, '/', 1 );
  String DD = getValue(Date, '/', 2 );

  String Time_separate = getValue(Time_NTP, ',', 2 );
  String Time = Time_separate.substring(0, 8);

  String Hr = Time.substring(0, 2);
  String Min = Time.substring(3, 5);
  String Sec = Time.substring(6, 8);

  my_time.Second = Sec.toInt();
  my_time.Hour  = Hr.toInt();
  my_time.Minute  = Min.toInt();
  my_time.Day  = DD.toInt();
  my_time.Month  = MM.toInt() - 1;      // months start from 0, so deduct 1
  my_time.Year  = YY.toInt() - 1970;
}
