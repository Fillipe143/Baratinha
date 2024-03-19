void setupLidars()//Configura os LIDARs
{
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
  for(int i = 0; i < numSenDist; i++)
  {
    vl[i].init();
    vl[i].setTimeout(500);
    vl[i].startContinuous();
    vl[i].setMeasurementTimingBudget(modoSensor);
  }
  Serial.println("Terminada configuração dos LIDARs");

  //dist2.startRangeContinuous(10000); --> Periodo de medição
  
  //dist3.configSensor(VL53L0X_SENSE_HIGH_ACCURACY);
}

void setID()//Troca os endereços dos LIDARs
{
  delay(10);

  // seta tudo pra baixo
  Serial.println("entrei set lidars");

  delay(10);

  pcf.pinMode(xShut1, INPUT); // seta pra cima
  delay(10);
  vl[0].setAddress(addr1);

  pcf.pinMode(xShut2, INPUT); // seta pra cima
  delay(10);
  vl[1].setAddress(addr2);
  
  pcf.pinMode(xShut3, INPUT); // seta pra cima
  delay(10);
  vl[2].setAddress(addr3);

  pcf.pinMode(xShut4, INPUT); // seta pra cima
  delay(10);
  vl[3].setAddress(addr4);

  pcf.pinMode(xShut5, INPUT); // seta pra cima
  delay(10);
  vl[4].setAddress(addr5);

  pcf.pinMode(xShut6, INPUT); // seta pra cima
  delay(10);
  vl[5].setAddress(addr6);

  pcf.pinMode(xShut7, INPUT); // seta pra cima
  delay(10);
  vl[6].setAddress(addr7);

  pcf.pinMode(xShut8, INPUT); // seta pra cima
  delay(10);
  vl[7].setAddress(addr8);

  Serial.println("Setei lidars");
}

void setupMotores()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}