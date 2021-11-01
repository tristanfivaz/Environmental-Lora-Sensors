#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
const char ssid[] = "XXXX";  // your network SSID (name)
const char pass[] = "XXXX";   // your network password
int r1 = D1;
int r2 = D2;
int r3 = D3;
int r4 = D4;
int r5 = D5;
int r6 = D6;
int r7 = D7;
int r8 = D8;

WiFiClient  client;

//---------Channel Details---------//
unsigned long counterChannelNumber = X;            // Channel ID
unsigned long counterChannelNumber2 = X;
unsigned long counterChannelNumber3 = X;
unsigned long counterChannelNumber4 = X;
const char * myCounterReadAPIKey = "XX"; // Read API Key
const char * myCounterReadAPIKey2 = "XX"; // Read API Key
const char * myCounterReadAPIKey3 = "XX"; // Read API Key
const char * myCounterReadAPIKey4 = "XX"; // Read API Key
const int FieldNumber1 = 3;  // The field you wish to read
const int FieldNumber2 = 6;  // The field you wish to read
const int FieldNumber3 = 3;  // The field you wish to read
const int FieldNumber4 = 6;  // The field you wish to read
const int FieldNumber5 = 3;  // The field you wish to read
const int FieldNumber6 = 6;  // The field you wish to read
const int FieldNumber7 = 3;  // The field you wish to read
const int FieldNumber8 = 6;  // The field you wish to read
//-------------------------------//

void setup()
{
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(r6, OUTPUT);
  pinMode(r7, OUTPUT);
  pinMode(r8, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  //----------------- Network -----------------//
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ....");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    Serial.println("Connected to Wi-Fi Succesfully.");
  }
  //--------- End of Network connection--------//

  //---------------- Room 1 ----------------//
  long light1 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  int statusCode;
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light1: ");
    Serial.println(light1);
    if (light1 > 5)
    {
      digitalWrite(r1, HIGH);
    }
    else
    {
      digitalWrite(r1, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 1 -------------//

  //---------------- Room 2 ----------------//
  long light2 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light2: ");
    Serial.println(light2);
    if (light2 > 5)
    {
      digitalWrite(r2, HIGH);
    }
    else
    {
      digitalWrite(r2, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 2 -------------//



  //--------------------NEW CHANNEL--------------------//



  //---------Channel Details---------//
  //-------------------------------//

  //---------------- Room 3 ----------------//
  long light3 = ThingSpeak.readLongField(counterChannelNumber2, FieldNumber3, myCounterReadAPIKey2);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light3: ");
    Serial.println(light3);
    if (light3 > 5)
    {
      digitalWrite(r3, HIGH);
    }
    else
    {
      digitalWrite(r3, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 3 -------------//

  //---------------- Room 4 ----------------//
  long light4 = ThingSpeak.readLongField(counterChannelNumber2, FieldNumber4, myCounterReadAPIKey2);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light4: ");
    Serial.println(light4);
    if (light4 > 5)
    {
      digitalWrite(r4, HIGH);
    }
    else
    {
      digitalWrite(r4, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 4 -------------//
}


  //--------------------NEW CHANNEL--------------------//



  //---------Channel Details---------//
  //-------------------------------//


//---------------- Room 5 ----------------//
  long light5 = ThingSpeak.readLongField(counterChannelNumber3, FieldNumber5, myCounterReadAPIKey3);
  int statusCode;
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light5: ");
    Serial.println(light5);
    if (light5 > 5)
    {
      digitalWrite(r5, HIGH);
    }
    else
    {
      digitalWrite(r5, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 5 -------------//

  //---------------- Room 6 ----------------//
  long light6 = ThingSpeak.readLongField(counterChannelNumber3, FieldNumber6, myCounterReadAPIKey3);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light6: ");
    Serial.println(light6);
    if (light6 > 5)
    {
      digitalWrite(r6, HIGH);
    }
    else
    {
      digitalWrite(r6, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 6 -------------//



  //--------------------NEW CHANNEL--------------------//



  //---------Channel Details---------//
  //-------------------------------//

  //---------------- Room 7 ----------------//
  long light7 = ThingSpeak.readLongField(counterChannelNumber4, FieldNumber7, myCounterReadAPIKey4);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light7: ");
    Serial.println(light7);
    if (light7 > 5)
    {
      digitalWrite(r7, HIGH);
    }
    else
    {
      digitalWrite(r7, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 7 -------------//

  //---------------- Room 8 ----------------//
  long light8 = ThingSpeak.readLongField(counterChannelNumber4, FieldNumber8, myCounterReadAPIKey4);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Light8: ");
    Serial.println(light8);
    if (light8 > 5)
    {
      digitalWrite(r8, HIGH);
    }
    else
    {
      digitalWrite(r8, LOW);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 8 -------------//
}
