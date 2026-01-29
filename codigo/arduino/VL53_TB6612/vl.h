 void init_vl(){
  Serial.begin(115200);
  delay(2000); 
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  pcf.begin();
  for(int i=0; i<4; i++) {
    pcf.pinMode(i, OUTPUT);
    pcf.digitalWrite(i, LOW);
  }
  delay(10);
  for(int i=0; i<4; i++) {
    pcf.digitalWrite(i, HIGH);
    delay(10);
    
    sensors[i]->setTimeout(500);
    if (!sensors[i]->init()) {
      Serial.print("Fallo en sensor "); Serial.println(i+1);
    } else {
      // Cambiar dirección (0x30, 0x31, 0x32, 0x33)
      sensors[i]->setAddress(0x30 + i);
      
      // timout a 20ms
      sensors[i]->setMeasurementTimingBudget(20000);
      
      // Iniciar modo continuo
      sensors[i]->startContinuous();
      Serial.print("Sensor "); Serial.print(i+1); Serial.println(" configurado");
    }
  }}