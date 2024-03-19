uint8_t readLidar(int lidar)//Função que retorna a leitura de um LIDAR
{
  uint8_t valorMax = 5000;
  uint8_t t1;
  switch(lidar)
  {
    case 1:
      t1 = vl[0].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 2:
      t1 = vl[1].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 3:
      t1 = vl[2].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 4:
      t1 = vl[3].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 5:
      t1 = vl[4].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 6:
      t1 = vl[5].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 7:
      t1 = vl[6].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
    case 8:
      t1 = vl[7].readRangeContinuousMillimeters();
      return t1 > valorMax ? valorMax : t1;
  }
  return 0;
}
void motor(int pwmE, int pwmD)
{

  if (pwmE < 0)//tras
  {
    analogWrite(in2, abs(pwmE));
    analogWrite(in1, 0);
  }
  else//frente
  {
    analogWrite(in2, 0);
    analogWrite(in1, pwmE);
  }


  if (pwmD > 0)//frente
  {
    analogWrite(in3, pwmD);
    analogWrite(in4, 0);
  }
  else//tras
  {
    analogWrite(in3, 0);
    analogWrite(in4, abs(pwmD));
  }
  
}

void parar()
{
  motor(0, 0);
}