#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
const char ssid[] = "XXXX";  // your network SSID (name)
const char pass[] = "XXXX";   // your network password
int r1green = D1;
int r2green = D2;
int r3green = D3;
int r4green = D4;
int r1red = D5;
int r2red = D6;
int r3red = D7;
int r4red = D8;

WiFiClient  client;

//---------Channel Details---------//
unsigned long counterChannelNumber = XXXX;            // Channel ID
const char * myCounterReadAPIKey = "XXXX"; // Read API Key
const int FieldNumber1 = 2;  // The field you wish to read
const int FieldNumber2 = 5;  // The field you wish to read
const int FieldNumber3 = 2;  // The field you wish to read
const int FieldNumber4 = 5;  // The field you wish to read
const int FieldNumber5 = 2;  // The field you wish to read
const int FieldNumber6 = 5;  // The field you wish to read
const int FieldNumber7 = 2;  // The field you wish to read
const int FieldNumber8 = 5;  // The field you wish to read
//-------------------------------//

void setup()
{
  pinMode(r1green, OUTPUT);
  pinMode(r2green, OUTPUT);
  pinMode(r3green, OUTPUT);
  pinMode(r4green, OUTPUT);
  pinMode(r1red, OUTPUT);
  pinMode(r2red, OUTPUT);
  pinMode(r3red, OUTPUT);
  pinMode(r4red, OUTPUT);

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
  long temp1 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  int statusCode;
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Temperature1: ");
    Serial.println(temp1);
    if (temp1 < 30)
    {
      digitalWrite(r1green, HIGH);
      digitalWrite(r1red, LOW);
    }
    else
    {
      digitalWrite(r1green, LOW);
      digitalWrite(r1red, HIGH);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 1 -------------//

  //---------------- Room 2 ----------------//
  long temp2 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Temperature2: ");
    Serial.println(temp2);
    if (temp2 < 30)
    {
      digitalWrite(r2green, HIGH);
      digitalWrite(r2red, LOW);
    }
    else
    {
      digitalWrite(r2green, LOW);
      digitalWrite(r2red, HIGH);
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
  unsigned long counterChannelNumber = XXXX;            // Channel ID
  const char * myCounterReadAPIKey = "XXXX"; // Read API Key
  //-------------------------------//

  //---------------- Room 3 ----------------//
  long temp3 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Temperature3: ");
    Serial.println(temp3);
    if (temp1 < 30)
    {
      digitalWrite(r3green, HIGH);
      digitalWrite(r3red, LOW);
    }
    else
    {
      digitalWrite(r3green, LOW);
      digitalWrite(r3red, HIGH);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 3 -------------//

  //---------------- Room 4 ----------------//
  long temp4 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Temperature4: ");
    Serial.println(temp4);
    if (temp4 < 30)
    {
      digitalWrite(r4green, HIGH);
      digitalWrite(r4red, LOW);
    }
    else
    {
      digitalWrite(r4green, LOW);
      digitalWrite(r4red, HIGH);
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(100);
  //-------------- End of Room 4 -------------//
}
