#include <Wire.h>
#include <Adafruit_PCF8574.h>
//#include <BluetoothSerial.h>

// set the pins to shutdown
#define xShut1 0
#define xShut2 1 //não esta no pino certo
#define xShut3 2
#define xShut4 3 //não esta no pino certo
#define xShut5 4 //não esta no pino certo
#define xShut6 5 //não esta no pino certo
#define xShut7 6 //não esta no pino certo
#define xShut8 7 //não esta no pino certo


Adafruit_PCF8574 pcf;
//BluetoothSerial SerialBT;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  SerialBT.begin("baratinhaESP");
  setupLidars();
}

void loop() {
  if (SerialBT.available()) {
    Serial.println("Enviando");
    Wire.beginTransmission(0x08);
    while (SerialBT.available()) Wire.write(SerialBT.read());
    Wire.endTransmission();
  }
}

void setupLidars() {
  pcf.pinMode(xShut1, OUTPUT);
  pcf.pinMode(xShut2, OUTPUT);
  pcf.pinMode(xShut3, OUTPUT);
  pcf.pinMode(xShut4, OUTPUT);
  pcf.pinMode(xShut5, OUTPUT);
  pcf.pinMode(xShut6, OUTPUT);
  pcf.pinMode(xShut7, OUTPUT);
  pcf.pinMode(xShut8, OUTPUT);

  Serial.println("INICIANDO");
  setID();
  for(int i = 0; i < numSenDist; i++) {
    vl[i].init();
    vl[i].setTimeout(500);
    vl[i].startContinuous();
    vl[i].setMeasurementTimingBudget(modoSensor);
  }
  Serial.println("Terminada configuração dos LIDARs");

}

