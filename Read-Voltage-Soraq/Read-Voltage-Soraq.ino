#define BATVOLT_PIN BAT_VOLT

#define debugSerial SerialUSB
int BatteryAVG = 0;
int BatteryPin = A5;
int BatteryInput = 0;
float Voltage;

void setup() {
  // put your setup code here, to run once:
  debugSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  getBatteryVoltage();
  //SerialUSB.println(getBatteryVoltage());
  debugSerial.println(BatteryInput);
  delay(2000);
}

uint16_t getBatteryVoltage()
{
  BatteryInput = analogRead(BatteryPin);
  //Voltage = BatteryInput * (4.7 / (14.7)); voltage divider formula
  Voltage = 4.2 * (BatteryInput / 1023.0) + 0.44;
  debugSerial.print(F("Battery Voltage: "));
  debugSerial.println(Voltage);
}
