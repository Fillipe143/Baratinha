#include <Wire.h>

#define in1 3 // esquerda tras
#define in2 5 // esquerda frente

#define in3 6 // direita tras
#define in4 9 // direita frente

#define botao2 2

char moveInstruction = 'p';

void setup() {
  Wire.begin(0x08);
  Wire.onReceive(receberwire);
  Serial.begin(57600);
}

void loop() {
  switch (moveInstruction) {
    case 'f': motor('a', 't', 255);
    case 'd': motor('d', 't', 255);
    case 'e': motor('e', 't', 255);
  }
}

void receberwire(int len){
  String data;
  while (Wire.available()) {
    data += (char) Wire.read();
  }
  Serial.println(data);
  moveInstruction = data[0];
}

void motor(char lado, char dir, int pwm) {
  if (lado == 'e' || lado == 'a') {
    if (dir == 'f') {
      analogWrite(in2, pwm);
      analogWrite(in1, 0); 
    } else {
      analogWrite(in2, 0);
      analogWrite(in1, pwm);
    }
  }

  if (lado == 'd' || lado == 'a') {
    if (dir == 'f') {
      analogWrite(in3, pwm);
      analogWrite(in4, 0);
    } else {
      analogWrite(in3, 0);
      analogWrite(in4, pwm);
    }
  }
}
