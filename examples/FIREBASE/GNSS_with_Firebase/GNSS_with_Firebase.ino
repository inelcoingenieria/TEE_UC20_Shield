#include "TEE_UC20.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"
#include "gnss.h"

INTERNET net;
FIREBASE firebase;

//SIM TRUE  internet
#define USER "true"
#define APN "internet"
#define PASS "true"


#define LED 10
#define FIREBASE_URL      "Firebase URL"   // Examples "Thaieasyelecxxx.firebaseio.com" Delete https:// and / of back URL 
#define FIREBASE_SECRET   "Database Secrets"

GNSS gps;
unsigned long previousMillis = 0;
const long interval = 2000;

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}

void data_out(char data)
{
  Serial.write(data);
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
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
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
  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  int ret = firebase.connect();
  if (ret == 1) // test firebase connection.
  {
    Serial.println("Remove Object LED");
    firebase.remove("LED");
    Serial.println("Create Object LED");
    firebase.setInt("LED", 0);
  }
  firebase.close();

  previousMillis =  millis();
  //Start GPS
  gps.Start();
}
void loop() {

  unsigned long currentMillis = millis();
  String GPS_DATA = gps.GetPosition();
  if (GPS_DATA.indexOf(F("+QGPSLOC")) != -1) { // if detected gnss data
    //Serial.println(GPS_DATA);
    String latitude = getValue(GPS_DATA, ',', 1 );
    Serial.println("latitude : " + latitude);
    String longitude = getValue(GPS_DATA, ',', 2 );
    Serial.println("longitude : " + longitude);

    if (currentMillis - previousMillis >= interval)
    {
      int ret = firebase.connect();
      if (ret == 1)
      {

        firebase.setStr("latitude", latitude);
        firebase.setStr("longitude", longitude);
        String url_map = "https://maps.google.com/maps/place/" + latitude + "," + longitude;
        firebase.setStr("url_map", url_map);

        String jsObj = "{\"dataLat\":" + String(latitude) + ",\"dataLong\":" + String(longitude) + "}";  // Data must be JSON format
        String addpth = firebase.pushStr("/ex_push", jsObj);
        Serial.println("push : " + addpth);

      }
      firebase.close();
      previousMillis = currentMillis;
    }
  }
}
