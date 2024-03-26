#include <Wire.h>

#define ARDUINO_ADDR    0x08

#define MOTOR_LEFT_BACK   3
#define MOTOR_LEFT_FRONT  5
#define MOTOR_RIGHT_FRONT 6
#define MOTOR_RIGHT_BACK  9

#define BUTTON_PIN 2

#define MAX_SPEED_OF_LEFT_MOTOR   255
#define MAX_SPEED_OF_RIGHT_MOTOR  185

#define GO_FORWARD  0
#define GO_BACK     1

void setup() {
  Wire.begin(ARDUINO_ADDR);
  Wire.onReceive(onReceiveData);
  Serial.begin(57600);
}

void loop() { }

/** Protocolo de comunicação
    
    Utiliza 2 bytes, um para cada motor, sendo o primeiro o da esquerda e o segundo da direita

    O primeiro bit de cada byte simboliza a direção na qual o motor girar, sendo 0 ir para frente e 1 ir para tras
    Os outros 7 bits são utilizados para definir a potência que o motor ira girar, pondendo ela ser de representada de 0 a 100

    Exemplo:
    1 0110011 // Esquerda indo para frente em uma velocidade de 50%
    0 1100100 // Direita indo para tras em uma velocidade de 100%
**/

void onReceiveData(int len) {
  if (len != 2) {
    while (Wire.available()) Wire.read();
    return;
  }

  uint8_t leftMotor   = Wire.read();
  uint8_t rightMotor  = Wire.read();

  bool leftDirection  = leftMotor  >> 7;
  bool rightDirection = rightMotor >> 7;

  uint8_t leftSpeed   = leftMotor - leftDirection   * 0x80;
  uint8_t rightSpeed  = rightMotor - rightDirection * 0x80;

  motor(leftDirection, leftSpeed, rightDirection, rightSpeed);
}

void motor(bool leftDirection, uint8_t leftSpeed, bool rightDirection, uint8_t rightSpeed) {
  uint8_t leftPwm  = map(leftSpeed,  0, 100, 0, MAX_SPEED_OF_LEFT_MOTOR);
  uint8_t rightPwm = map(rightSpeed, 0, 100, 0, MAX_SPEED_OF_RIGHT_MOTOR);

  if (leftDirection == GO_FORWARD) {
    analogWrite(MOTOR_LEFT_FRONT, leftPwm);
    analogWrite(MOTOR_LEFT_BACK,  0); 
  } else {
    analogWrite(MOTOR_LEFT_FRONT, 0);
    analogWrite(MOTOR_LEFT_BACK,  leftPwm);
  }

  if (rightDirection == GO_FORWARD) {
    analogWrite(MOTOR_RIGHT_FRONT, rightPwm);
    analogWrite(MOTOR_RIGHT_BACK,  0); 
  } else {
    analogWrite(MOTOR_RIGHT_FRONT, 0);
    analogWrite(MOTOR_RIGHT_BACK,  rightPwm);
  }
}
