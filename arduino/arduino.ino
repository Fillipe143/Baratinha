#include <Wire.h>

#define in1 3 // esquerda tras
#define in2 5 // esquerda frente

#define in3 6 // direita tras
#define in4 9 // direita frente

#define botao2 2

int dspd = 0, espd  = 0;

void setup() {
  Wire.begin(0x08);
  Wire.onReceive(receberwire);
  Serial.begin(57600);
}

void loop() {
  // 1 = frente, -1 = tras, 0 = para
  if (dspd == 1) motor('e', 'f', 255);
  if (dspd == -1) motor('e', 't', 255);
  if (dspd == 0) motor('e', 'f', 0);

  if (espd == 1) motor('d', 'f', 255);
  if (espd == -1) motor('d', 't', 255);
  if (espd == 0) motor('d', 'f', 0);
}


void receberwire(int len){
  String data;
  while (Wire.available()) {
    data += (char) Wire.read();
  }
  Serial.println(data);

  // f = frente, t = tras, p = para
  if (data[0] == 'f') dspd = 1;
  if (data[0] == 't') dspd = -1;
  if (data[0] == 'p') dspd = 0;

  if (data[1] == 'f') espd = 1;
  if (data[1] == 't') espd = -1;
  if (data[1] == 'p') espd = 0;
}

// e = esquerda, d = direita, a = ambos
// f = frente, t = tras
void motor(char lado, char dir , int pwm) {
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
