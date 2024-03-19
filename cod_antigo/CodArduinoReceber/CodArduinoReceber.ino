#include <QTRSensors.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial Serial2 = SoftwareSerial(10, 11);

QTRSensors qtr;

int s1, s2;
static unsigned int sen1 = 0;
static unsigned int sen2 = 0;
// objects for the vl53l0x

// this holds the measurement


//Portas dos motores
#define in1 3
#define in2 5

#define in3 6
#define in4 9

#define botao2 2
bool b2LastRead = 1, b2Read = 1;

//Pinos LEDs
#define ledVermelho 12
#define ledVerde 13

const uint8_t SensorCount = 7;
uint16_t sensorValues[SensorCount];
#define tempoCalibracao 400



// For bluetooth communication
char cmd[30];
int cmdIndex;
float recebido;

//flag start/stop --> usada para pausar e despausar o robô remotamente
bool startStop = false;

//posicao de referencia para o PID
int posicao = 0; 



//erros (distancia da linha)
int error = 0;
int lastError = 0;
int second_lastError = 0;

//velocidade basal de cada motor
int M2 = 210;//Esquerda
int M1 = 220;//Direita

// //velocidades maximas e minimas de cada motor  
// int Mm2 = 60;
// int Mm1 = 60;//Aumenta a velocidade quando erro tá pequeno(para a reta)
// int MMAX = 200;//max Vel para curvas/correção
// int MMAX2 = -200;//max inversão

//velocidades maximas e minimas de cada motor  
int Mm2 = 110;
int Mm1 = 120;//Aumenta a velocidade quando erro tá pequeno(para a reta)
int MMAX = 255;//max Vel para curvas/correção
int MMAX2 = -255;//max inversão


//posicao ideal para o robo


#define setPoint 3000

//constantes do PID
// float KP = 0.05;       // proportional constant
// float KI = 0.0001;    // integrative constant
// float KD = 0.1;     // derivative constant
// float k = 1;

//0.13, 0.001, 1 --> m2 80 |  m1 100
//0.17, 0.001, 1.77 --> m2 120 |  m1 140
//0.312, 0.001, 1.95 --> m2 180 |  m1 200

float KP = 0.34;       // proportional constant
float KI = 0.001;    // integrative constant
float KD = 2;     // derivative constant
float k = 1;

//contador de erro acumulado
int I = 0;


void setup()
{
  Wire.begin(0x08);
  Wire.onReceive(receberwire);

  
  Serial.begin(57600);
  Serial2.begin(57600);
  delay(500);
  Serial.println("INICIANDO");

  // //Sensor de linha  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A3, A2, A1, A4, A0, A7, A6}, SensorCount);
  // Serial.println("XXXXXXXX");
  // //expansor I/O + distancia

  pinMode(botao2, INPUT_PULLUP);
  Serial.println("Aguardando inicio");
  while(!(b2Read == 0 && b2LastRead == 1))
  {
    b2LastRead = b2Read;
    b2Read = digitalRead(botao2);
  }
  Serial.println("Iniciou");

  delay(500);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT); 



  Serial.print("Calibrando");
   digitalWrite(ledVerde, HIGH);
  delay(300);
  digitalWrite(ledVerde, LOW);//Pisca led para sinalizar inicio da calibração

  // // motor('e', 'f',80);
  // // motor('d', 't',80);
  
  for (int i = 0; i < tempoCalibracao; i++)  // calibracao
  {
    qtr.calibrate();
    if(i%5 == 0) Serial.print(".");      
  }
  motor('a', 'f',0);
   digitalWrite(ledVerde, HIGH);
  delay(300);
  digitalWrite(ledVerde, LOW);//Pisca led para sinalizar fim da calibração
  Serial.println("  OK!");
  // // mostra os valores calibrados minimos
  // for (uint8_t i = 0; i < SensorCount; i++)
  // {
  //   Serial.print(qtr.calibrationOn.minimum[i]);
  //   Serial.print(' ');
  // }
  // Serial.println();

  // // mostra os valores calibrados maximos
  // for (uint8_t i = 0; i < SensorCount; i++)
  // {
  //   Serial.print(qtr.calibrationOn.maximum[i]);
  //   Serial.print(' ');
  // }
  // Serial.println();
  // Serial.println();
  // delay(3000);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop()
{
  // for(int i = 0; i < 255; i++)
  // {
  //   Serial.println(i);
  //   motor('a', 'f', i);
  //   delay(15);
  // }
  // motor('a', 'f', 0);
  // delay(1000);
  // for(int i = 255; i > 0; i--)
  // {
  //   Serial.println(i);
  //   motor('a', 't', i);
  //   delay(15);
  // }
  // motor('a', 'f', 0);
  // delay(1000);
  // Serial.println("To no loop");
  // delay(1000);
  // motor('d', 'f', 50);
  // motor('e', 't', 50);
  // Serial.println("Giro");
  // delay(1000);
  // motor('a', 'f', 0);
  // Serial.println("Para");
  // delay(1000);
  //transmission(0b00000000);
  //read_dual_sensors();
  // lerSens();

  b2LastRead = b2Read;
  b2Read = digitalRead(botao2);

  if(Serial.available()) 
  {
    char c = (char)Serial.read();
    if(c=='\r' || c=='\n') 
    {
        cmd[cmdIndex] = 0;
        cmdIndex = 0;
        execute(cmd);
    }
    else 
    {      
      cmd[cmdIndex++] = c;
    }
  }
  if(b2Read == 0 && b2LastRead == 1) startStop = !startStop;
  // Serial.print("Sensor 1: ");
  // Serial.print(sen1);
  // Serial.print(" Sensor 2: ");
  // Serial.println(sen2);
    //lerDist(1);
    //Serial2.println("1");
    //Serial.println("Bytes disponiveis: " + String(Serial2.available()));
    //delay(2000);
   //Full message received...
    
    // String leitura = Serial2.readStringUntil('\n');
    // s1 = leitura.substring(0, leitura.indexOf('|')).toInt();
    // s2 = leitura.substring(leitura.indexOf('|') + 1).toInt();
    //Serial.println();
    //Serial.println("  s1: " + String(s1) + " s2: " + String(s2) + " String: " + leitura);
    // if(s1 < 50 && s1 > 10)
    // {
    //   // motor('a', 'f', 0);
    //   // digitalWrite(ledVermelho, HIGH);
    //   // delay(2000);
    //   // motor('d', 't', 100);//direita
    //   // motor('e', 'f', 100);
    //   // delay(800);

    //   // motor('a', 'f', 0);
    //   while(true)
    //   {
    //     char[10] l;
    //     l = Serial2.readBytesUntil('\n',*l);
    //   }
      // while(sensorValues[0] < 500)
      // {
      //   Serial.println("sensor 0" + sensorValues[0]);
      //   if(Serial2.available()) 
      //   {
      //     char[10] l;
      //     l = Serial2.readBytes();
      //     s1 = leitura.substring(0, leitura.indexOf('|')).toInt();
      //     s2 = leitura.substring(leitura.indexOf('|') + 1).toInt();
      //   }
      //   qtr.readLineBlack(sensorValues);

      //   if(s2 > 65)
      //   {
      //     motor('d', 'f', 50);
      //     motor('e', 'e', 50);
      //   }
      //   else motor('a', 'f', 80);
      // }

      // motor('a', 'f', 0);
      // digitalWrite(ledVermelho, LOW);
  // }
  if(startStop)
  {
    //lerSens();
    pid_seguelinha();
    // motor('e', 'f', 180);
    // motor('d', 'f', 200);
  }
  else
  {
    motor('e', 'f', 0);
    motor('d', 'f', 0);
  }


}

void receberwire(int x)
{
  Serial.println("recebi algo");
  String r;
  int rec;
  static char c[50];
  if (Wire.available()) 
  {
    r = Wire.readStringUntil('.');
    //rec = Wire.read();
  }
  Serial.print("quantidade: " + String(x) + "   lido:"); Serial.println(String(r)) ;
}

int lerDist(int sen)
{
  Serial2.println("1");
  //Serial2.write(sen);
  Serial.println("Tô preso");
  while(Serial2.available() <= 0)
  {
    Serial2.println("1");
  }

  Serial.println("sai");

  String a = Serial2.readStringUntil('\0');
  Serial.print(a);

  while(Serial2.available())
  {
    char x = Serial2.read();
  }
  Serial.println();
  // Serial.println("Lido: " + Serial2.read());
  return 0;
}


void execute(char* cmd) 
{
  if(cmd[1] != ' ') return; // unknown command

  //atribui a recebido o float dps das duas primeiras casas de cmd
  if(strcmp(cmd, "S S") == 0)startStop = !startStop;
  else
  {
    recebido = atof(cmd+2);
    if(cmd[0] == 'p')KP = recebido;
    else if(cmd[0] == 'i')KI = recebido;
    else if(cmd[0] == 'd')KD = recebido;
    else return; // comando desconhecido
  }

  Serial.print("Kp: ");
  Serial.println(startStop);
  Serial.print("Kp: ");
  Serial.println(KP, 3);
  Serial.print("Ki: ");
  Serial.println(KI, 4);
  Serial.print("Kd: ");
  Serial.println(KD, 2);
}



//determina a posicao do robo em relacao a linha
void lerSens()
{
  posicao = qtr.readLineBlack(sensorValues);
  // for(int i = 0; i < SensorCount; i++)
  // {
  //   Serial.print("Sensor " + String(i+1) + ": " = String(sensorValues[i]) + String("  -> "));
  // }
  // Serial.println(posicao);
}

//controlador PID
void pid_seguelinha()
{
    lerSens();
    int m1Speed;
    int m2Speed;
    
    second_lastError = lastError;
    lastError=error;
    error = posicao - setPoint;
    

    // Serial.println("Setpoint: " + String(setPoint) + " Erro: " + String(error) + " m1Speed: " + String(m1Speed) + " m2Speed: " + String(m2mSpeed) );
    
    I = I + error;
    
    int motorSpeed = KP * error + KD * (error - lastError) + KI*I;
    
    


    if(abs(error) == 3000)
    {
        Serial.print("  talvez Le: "+ String(abs(lastError)) + " LLE: "+String(abs(second_lastError)) + "   ");
        //delay(1300);
        if(abs(lastError) < 1000 && abs(second_lastError) < 1000)
        {
            error = 0;   
            Serial.println("REEEEEEEEE");
            for(int i = 0; i < 50; i ++)
            {
              Serial.println("REEEEEEEEE");
              motor('e','f',180);
              motor('d','f',200);
            }
            
            // delay(500);
            // motor('a','f',0);
        }  
    }  
 
    if(abs(error) <= 200)
    {
         m1Speed = Mm1 - motorSpeed;
    
         m2Speed = Mm2 + motorSpeed;  
    }
    else
    {
         m1Speed = M1 - motorSpeed;
    
         m2Speed = M2 + motorSpeed;  
    }

    
 
    if (m1Speed < MMAX2) 
    {
        m1Speed =  MMAX2;           // Determina o limite inferior
    }

    //Serial.println("Setpoint: " + String(setPoint) + " Erro: " + String(error) + " m1Speed: " + String(m1Speed) + " m2Speed: " + String(m2Speed)  + " motorSpeed: " + String(motorSpeed));
    
    
    if (m2Speed < MMAX2) 
    {
      m2Speed = MMAX2;           // Determina o limite inferior
    }
    
    if (m1Speed > MMAX)
    {
      m1Speed = MMAX;     // Determina o limite superior
    }
    
    if (m2Speed > MMAX) 
    {
        m2Speed = MMAX;     // Determina o limite superior
    }
    
    if(m1Speed < 0)
    {
        motor('d', 't', abs(m1Speed));
        Serial.print("MOTOR D:  ");   
    }
    else
    {
        motor('d', 'f', abs(m1Speed));
        Serial.println(abs(m1Speed));   
    }
    
   
    if(m2Speed < 0)
    {
        motor('e', 't', abs(m2Speed));
        Serial.print("MOTOR E:  ");  
    }
    else
    {
        motor('e', 'f', abs(m2Speed));
        Serial.println(abs(m2Speed));  
    }

    //Serial.println("Setpoint: " + String(setPoint) + " Erro: " + String(error) + " m1Speed: " + String(m1Speed) + " m2Speed: " + String(m2Speed)  + " motorSpeed: " + String(motorSpeed));
    //Serial.println("Setpoint: " + String(setPoint) + " Erro: " + String(error) + " m1Speed: " + String(m1Speed) + " m2Speed: " + String(m2Speed)  + " motorSpeed: " + String(motorSpeed));
    //Serial.println();
}

//funcao para facilitar o controle dos motores - fica da forma motor(lado, direcao, velocidade)
void motor(char lado, char dir , int pwm)
{
  if (lado == 'e' || lado == 'a')
  {
    if (dir == 't')
    {
      analogWrite(in2, pwm);
      analogWrite(in1, 0);
    }
    else
    {
      analogWrite(in2, 0);
      analogWrite(in1, pwm);
    }
  }
  if (lado == 'd' || lado == 'a')
  {
    if (dir == 'f')
    {
      analogWrite(in3, pwm);
      analogWrite(in4, 0);
    }
    else
    {
      analogWrite(in3, 0);
      analogWrite(in4, pwm);
    }
  }
}

// void setID()
// {
//   // all reset
//   // PCF_01.write(SHT_DIST1, LOW);    
//   // PCF_01.write(SHT_DIST3, LOW);
//   transmission(0b00000000);
//   delay(10);
//   // all unreset
//   // PCF_01.write(SHT_DIST1, HIGH);
//   // PCF_01.write(SHT_DIST3, HIGH);
//   transmission(0b00000101);
//   delay(10);

//   // activating dist1 and resetting dist3
//   // PCF_01.write(SHT_DIST1, HIGH);
//   // PCF_01.write(SHT_DIST3, LOW);
//   transmission(0b00000001);

//   // initing dist1
//   if(!dist1.begin(DSIT1_ADDRESS)) {
//     Serial.println(F("Failed to boot first VL53L0X"));
//   }
//   delay(10);

//   // activating dist3
//   //PCF_01.write(SHT_DIST3, HIGH);
//   transmission(0b00000101);
//   delay(10);

//   //initing dist3
//   if(!dist3.begin(DIST3_ADDRESS)) {
//     Serial.println(F("Failed to boot second VL53L0X"));
//   }
//   transmission(0b00000101);
// }

// void read_dual_sensors() {
  
//   dist1.rangingTest(&dFrente, false); // pass in 'true' to get debug data printout!
//   // dist3.rangingTest(&dEsquerda, false); // pass in 'true' to get debug data printout!

//   // print sensor one reading
//   Serial.print(F("1: "));
//   if(dFrente.RangeStatus != 4) {     // if not out of range
//     Serial.print(dFrente.RangeMilliMeter);
//     Serial.print(" ");
//   } else {
//     Serial.print(F("Out of range "));
//   }
  
//   Serial.print(F(" "));

//   // // print sensor two reading
//   // Serial.print(F("2: "));
//   // if(dEsquerda.RangeStatus != 4) {
//   //   Serial.print(dEsquerda.RangeMilliMeter);
//   //   Serial.print(" ");
//   // } else {
//   //   Serial.print(F("Out of range "));
//   // }
  
//   Serial.println();
// }

// void transmission ( byte val ) 
// {
//   Wire. beginTransmission ( 0x20 ) ;
//   Wire. write ( val ) ;
//   Wire. endTransmission () ;
// }