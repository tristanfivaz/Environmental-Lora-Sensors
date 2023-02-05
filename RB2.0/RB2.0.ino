#include <TheThingsNetwork.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <OrangeForRN2483.h>

// Set your AppEUI and AppKey
const char *appEui = " FILLMEIN ";
const char *appKey = " FILLMEIN ";

#define loraSerial Serial2
#define debugSerial SerialUSB
#define ONE_WIRE_BUS 6

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_AU915

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  sensors.begin();

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
  OrangeForRN2483.init();

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  if (!OrangeForRN2483.isDeepSleeping())
  {
    setup();
    senddata();
    digitalWrite(LED_BUILTIN, LOW);
    String devEUI = OrangeForRN2483.getSysCmds()->getHardwareDevEUI();

    delay(500);

    debugSerial.print("devEUI = "); debugSerial.println(devEUI);
    debugSerial.end();

    digitalWrite(LED_BUILTIN, HIGH);
  }

  // DeepSleep 00:01:30
  OrangeForRN2483.deepSleep(01, 00, 00);
}

void senddata()
{
  int s1pin = A0, sm1;
  int s2pin = A1, sm2;
  int s1map, s2map;
  float temp;

  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  sm1 = analogRead(s1pin);
  sm2 = analogRead(s2pin);

  s1map = map(sm1, 1023, 430, 0, 100);
  s2map = map(sm2, 1023, 430, 0, 100);

  int tempint = temp * 100;
  int avgmst = (s1map + s2map) / 2;

  byte payloadA[2];
  payloadA[0] = highByte(tempint);
  payloadA[1] = lowByte(tempint);
  byte payloadB[2];
  payloadB[0] = highByte(avgmst);
  payloadB[1] = lowByte(avgmst);

  int sizeofPayloadA = sizeof(payloadA);
  int sizeofPayloadB = sizeof(payloadB);

  byte payload[sizeofPayloadA + sizeofPayloadB];

  memcpy(payload, payloadA, sizeofPayloadA);
  memcpy(payload + sizeofPayloadA, payloadB, sizeofPayloadB);

  debugSerial.println(temp);
  debugSerial.println(avgmst);

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));
}
