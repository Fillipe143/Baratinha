#include <Adafruit_PCF8574.h>
#include <Adafruit_VL53L0X.h>

#define PCF_ADDR 0x20

#define N_SENSOR 8
#define LOX1_ADDR 1
#define LOX2_ADDR 2
#define LOX3_ADDR 3
#define LOX4_ADDR 4
#define LOX5_ADDR 5
#define LOX6_ADDR 6
#define LOX7_ADDR 7

Adafruit_PCF8574 pcf;
Adafruit_VL53L0X lox_1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_4 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_5 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_6 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_7 = Adafruit_VL53L0X();

void setup() {
    Serial.begin(115200);
    setupSensors();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox_1.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance 1 (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  
  lox_2.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance 2 (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }

  Serial.println();
    
  delay(400);
}


void setupSensors() {
    if (!pcf.begin(PCF_ADDR, &Wire)) {
        Serial.println("Couldn't find PCF");
        while (1);
    }
    
    Serial.println("Desligando sensores...");
    for (uint8_t sensor = 1; sensor <= N_SENSOR; sensor++) {
        pcf.digitalWrite(sensor, LOW);
    }


    Serial.println("Configurando...");
    
    Serial.println("Sensor " + String(LOX1_ADDR));
    pcf.digitalWrite(LOX1_ADDR, HIGH); 
    if (!lox_1.begin()) {
      Serial.println(("Failed to boot VL53L0X"));
      while(1);
    }
    lox_1.setAddress(0x29 + LOX1_ADDR);


    Serial.println("Sensor " + String(LOX2_ADDR));
    pcf.digitalWrite(LOX2_ADDR, HIGH); 
    if (!lox_2.begin()) {
      Serial.println(("Failed to boot VL53L0X"));
      while(1);
    }
    lox_2.setAddress(0x29 + LOX2_ADDR);

    Serial.println("Terminou a configuração");
}