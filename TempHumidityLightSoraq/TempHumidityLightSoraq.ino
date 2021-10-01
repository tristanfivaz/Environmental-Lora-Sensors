   
#include <TheThingsNetwork.h>
#include "DHT.h"

// Set your AppEUI and AppKey
const char *appEui = "Insert AppEUI";
const char *appKey = "Insert Appkey";

#define loraSerial Serial2
#define debugSerial SerialUSB

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_AU915

int lightpin = A0;    // select the input pin for the potentiometer
int sensorvalue = 0;  // variable to store the value coming from the sensor
#define DHTPIN 2 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
 
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
 
  pinMode(BUTTON, INPUT);
 
  debugSerial.println("-- STATUS");
  ttn.showStatus();
 
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  dht.begin();
}
 
void loop()
{
  debugSerial.println("-- LOOP");

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  sensorvalue = analogRead(lightpin);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    debugSerial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Multiply by 100 to convert to remove decimal places
  int humidityInt = humidity * 100;
  int temperatureInt = temperature * 100;

  //Break the humidity, temperature and heat index into Bytes in individual buffer arrays
  byte payloadA[2];
  payloadA[0] = highByte(humidityInt);
  payloadA[1] = lowByte(humidityInt);
  byte payloadB[2];
  payloadB[0] = highByte(temperatureInt);
  payloadB[1] = lowByte(temperatureInt);
  byte payloadC[2];
  payloadC[0] = highByte(sensorvalue);
  payloadC[1] = lowByte(sensorvalue);

  //Get the size of each buffer array (in this case it will always be 2, but this could be used if they were variable)
  int sizeofPayloadA = sizeof(payloadA);
  int sizeofPayloadB = sizeof(payloadB);
  int sizeofPayloadC = sizeof(payloadC);

  //Make a buffer array big enough for all the values
  byte payload[sizeofPayloadA + sizeofPayloadB + sizeofPayloadC];

  //Add each of the individual buffer arrays the single large buffer array
  memcpy(payload, payloadA, sizeofPayloadA);
  memcpy(payload + sizeofPayloadA, payloadB, sizeofPayloadB);
  memcpy(payload + sizeofPayloadA + sizeofPayloadB, payloadC, sizeofPayloadC);

  //Print some outputs for debugging
  debugSerial.print(F("Humidity: "));
  debugSerial.print(humidityInt);
  debugSerial.print(F("%  Temperature: "));
  debugSerial.print(F("°C "));
  debugSerial.print(temperatureInt);
  debugSerial.print(F("Light: "));
  debugSerial.println(sensorvalue);

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));

  //Wait 60 second between readings
  delay(60000);
}
