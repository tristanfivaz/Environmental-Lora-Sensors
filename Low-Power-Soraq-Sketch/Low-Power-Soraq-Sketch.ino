#include <Arduino.h>
#include <SPI.h>

#include <Sodaq_RN2483.h>
#include <Sodaq_wdt.h>
#include <RN487x_BLE.h>

// Specific library dependencies:
// RN487x_BLE.h   ->  Microchip_RN487x library  ->  version 1.0.2 +
// Sodaq_wdt.h    ->  Sodaq_wdt library
// Sodaq_RN2483.h ->  Sodaq_RN2483

/*
 *  This sketch as-is sets an ExpLoRer down to 20.5 uA / 76 uW after start-up phase
 *  Lower is possible but requires shutting down the watchdog timer and loses funtionality
 *  
 *  Possible improvement: setting the ATECC508a/608a watchdog timer to 10s vs default 1.3s
 */

#define bleSerial       Serial1
#define loraSerial      Serial2

void setup()
{
    // setup watchdog timer -- we want to wake up
    sodaq_wdt_enable(WDT_PERIOD_8X);
    sodaq_wdt_reset();
    sodaq_wdt_safe_delay(5000);

    // LoRa module connection and sleep
    loraSerial.begin(LoRaBee.getDefaultBaudRate());
    LoRaBee.init(loraSerial, LORA_RESET);
    LoRaBee.sleep();
    sodaq_wdt_safe_delay(5);  // not essential but seems to smooth things up

    // BLE module sleep
    rn487xBle.hwInit();
    bleSerial.begin(rn487xBle.getDefaultBaudRate());
    rn487xBle.initBleStream(&bleSerial);
    rn487xBle.enterCommandMode();
    rn487xBle.dormantMode();
    bleSerial.end();

    // set FLASH to deep sleep & reset SPI pins for min. energy consumption
    DFlashUltraDeepSleep();

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // set sleep mode -> deep sleep

    SerialUSB.flush();
    SerialUSB.end();
    USBDevice.detach();
    USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;   // Disable USB
}

void loop()
{
  sodaq_wdt_reset();
  systemSleep();
}

/**
  Powers down all devices and puts the system to deep sleep.
*/
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
