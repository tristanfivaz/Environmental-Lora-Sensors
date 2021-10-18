#define ADC_AREF 3.3f
// #define BATVOLT_R1 2.0f // One v1
// #define BATVOLT_R2 2.0f // One v1
#define BATVOLT_R1 4.7f // One v2
#define BATVOLT_R2 10.0f // One v2
#define BATVOLT_PIN BAT_VOLT

#define debugSerial SerialUSB
int BatteryAVG = 0;
int BatteryPin = A5;
int BatteryInput = 0;
int Voltage;

void setup() {
  // put your setup code here, to run once:
  debugSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  SerialUSB.println(getBatteryVoltage());
  delay(2000);
}

uint16_t getBatteryVoltage()
{
  BatteryInput = analogRead(BatteryPin);
  Voltage = BatteryInput * (4.7 / (14.7));
  debugSerial.print(F("Battery Voltage: "));
  SerialUSB.println(Voltage);
}
