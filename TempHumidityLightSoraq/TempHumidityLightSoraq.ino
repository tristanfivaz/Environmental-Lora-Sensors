   
#include <TheThingsNetwork.h>
#include "DHT.h"
#include <Arduino.h>
#include <SPI.h>
#include <Sodaq_RN2483.h>
#include <Sodaq_wdt.h>
#include <RN487x_BLE.h>

// Set your AppEUI and AppKey
const char *appEui = "Insert AppEUI";
const char *appKey = "Insert Appkey";


#define bleSerial Serial1
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
  sodaq_wdt_enable(WDT_PERIOD_8X);
  sodaq_wdt_reset();
  sodaq_wdt_safe_delay(5000);
    
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  LoRaBee.init(loraSerial, LORA_RESET);
  LoRaBee.sleep();
  sodaq_wdt_safe_delay(5); 

  rn487xBle.hwInit();
  bleSerial.begin(rn487xBle.getDefaultBaudRate());
  rn487xBle.initBleStream(&bleSerial);
  rn487xBle.enterCommandMode();
  rn487xBle.dormantMode();
  bleSerial.end();
  DFlashUltraDeepSleep();
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // set sleep mode -> deep sleep
  SerialUSB.flush();
  SerialUSB.end();
  USBDevice.detach();
  USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;   // Disable USB
  
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

  //SLEEP
  sodaq_wdt_reset();
  systemSleep();
}

void systemSleep()
{
  // Disable systick interrupt
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

  __WFI(); // SAMD sleep

  // Enable systick interrupt
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

}

// FLASH chip sleep functions
void DFlashUltraDeepSleep()
{
  static const uint8_t SS_DFLASH  = 44 ;
  SPI.begin();
  pinMode(SS_DFLASH, OUTPUT);
  digitalWrite(SS_DFLASH, HIGH);
 // transmit(0xAB);
 // sodaq_wdt_safe_delay(10);
  transmit(0xB9);
  SPI.end();
  resetSPIPins();
}

void transmit(uint8_t val)
{
  SPISettings settings;
  digitalWrite(SS_DFLASH, LOW);
  SPI.beginTransaction(settings);
  SPI.transfer(val);
  SPI.endTransaction();
  digitalWrite(SS_DFLASH, HIGH);
  delayMicroseconds(1000);
}

void resetSPIPins()
{
  resetPin(MISO);
  resetPin(MOSI);
  resetPin(SCK);
  resetPin(SS_DFLASH);
}

void resetPin(uint8_t pin)
{
  PORT->Group[g_APinDescription[pin].ulPort].
  PINCFG[g_APinDescription[pin].ulPin].reg=(uint8_t)(0);
  PORT->Group[g_APinDescription[pin].ulPort].
  DIRCLR.reg = (uint32_t)(1<<g_APinDescription[pin].ulPin);
  PORT->Group[g_APinDescription[pin].ulPort].
  OUTCLR.reg = (uint32_t) (1<<g_APinDescription[pin].ulPin);
}
