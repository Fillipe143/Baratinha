#include <Adafruit_PCF8574.h>
#include <BluetoothSerial.h>

#include <VL53L0X.h>
//#include "PCF8574.h"


BluetoothSerial SerialBT;

#define buzzer 26

#define numSenDist 8

// address we will assign if dual sensor is present
#define addr1 0x30
#define addr2 0x31 //não esta no address certo
#define addr3 0x32
#define addr4 0x33 //não esta no address certo
#define addr5 0x34 //não esta no address certo
#define addr6 0x35 //não esta no address certo
#define addr7 0x36 //não esta no address certo
#define addr8 0x37 //não esta no address certo

#define modoSensor 50000

// set the pins to shutdown
#define xShut1 0
#define xShut2 1 //não esta no pino certo
#define xShut3 2
#define xShut4 3 //não esta no pino certo
#define xShut5 4 //não esta no pino certo
#define xShut6 5 //não esta no pino certo
#define xShut7 6 //não esta no pino certo
#define xShut8 7 //não esta no pino certo

VL53L0X vl[numSenDist];

Adafruit_PCF8574 pcf;




//Portas dos motores
  #define in1 3
  #define in2 5

  #define in3 6
  #define in4 9


void setup() 
{
  Serial.begin(57600);
  SerialBT.begin("baratinhaESP");
  Wire.begin();
  // Wire1.begin(0x100);
  // Wire1.onReceive(wirerec);

  pinMode(buzzer, OUTPUT);
  Serial.println("INICIANDO");
  SerialBT.println("Iniciando");
  delay(500);
  
 
  // if (!pcf.begin(0x20, &Wire)) {
  //   Serial.println("Couldn't find PCF8574");
  //   while (1);
  // }
  // //setupMotores();
  // delay(1000);
  // Serial.println(F("Shutdown pins inited..."));
  // delay(1000);
  

  // Serial.println(F("All in reset mode...(pins are low)"));
  // SerialBT.println(F("All in reset mode...(pins are low)"));
  // delay(1000);
  
  // Serial.println(F("Starting..."));
  // SerialBT.println("Starting...");
  // delay(1000);
  // setupLidars();


 
}

void loop() 
{
  //delay(1000);
  if (SerialBT.available()) {
    Serial.println("Recebeu");
    Wire.beginTransmission(0x08);
    while (SerialBT.available()) Wire.write(SerialBT.read());
    int error = Wire.endTransmission();
    Serial.println(error);
  }
  // long t = millis();

  // for(int i = 0; i < numSenDist; i++)
  // {
  //   Serial.print("\tSen ");Serial.print(i+1);Serial.print(": ");Serial.print(readLidar(i+1));
  // }
  

  // static int x = 0;
  // int r =  millis() - t;
  // if(r > x)
  // {
  //   x = r;
    
  // }
  // Serial.print('\t');
  // Serial.println(x);

}

void transmission ( byte val ) 
{
  Wire. beginTransmission ( 0x20 ) ;
  Wire. write ( val ) ;
  Wire. endTransmission () ;
}

// void receber()
// {
//   while (Serial2.available() > 0)
//   {
//     //Create a place to hold the incoming message
//     static char message[20];
//     static unsigned int message_pos = 0;

//     //Read the next available byte in the serial receive buffer
//     char inByte = Serial2.read();

//     //Message coming in (check not terminating character) and guard for over message size
//     if ( inByte != '\n' && (message_pos < 100 - 1) )
//     {
//       //Add the incoming byte to our message
//       message[message_pos] = inByte;
//       message_pos++;
//     }
//     else
//     {
//       //Add null character to string
//       message[message_pos] = '\0';

//       if(message[0] == '1')
//       {
//         Serial.println("Sensor 1 ");
//         Serial2.println(String(s1));

//       }
//       else if(message[0] == '2')
//       {
//         Serial.println("Sensor 2 ");
//         Serial2.println(String(s2));
//       }
//       else if(message[0] == '3')
//       {
//         Serial.println("Sensor 3 ");
//         Serial2.println(String(s3));
//       }
//       else if(message[0] == '4')
//       {
//         Serial.println("Sensor 4 ");
//         Serial2.println(String(s4));
//       }
//       else if(message[0] == '5')
//       {
//         Serial.println("Sensor 5 ");
//         Serial2.println(String(s5));
//       }
//       else if(message[0] == '6')
//       {
//         Serial.println("Sensor 6 ");
//         Serial2.println(String(s6));
//       }
//       else if(message[0] == '7')
//       {
//         Serial.println("Sensor 7 ");
//         Serial2.println(String(s7));
//       }
//       else if(message[0] == '8')
//       {
//         Serial.println("Sensor 8 ");
//         Serial2.println(String(s8));
//       }
//       //Print the message (or do other things)
//       Serial.print("msg: ");
//       Serial.println(message);

//       //Reset for the next message
//       message_pos = 0;
//     }
//   }
// }
