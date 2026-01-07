#include <Wire.h>
#include <VL53L0X.h>
#include <PCF8574.h>
#include "TB6612.h"
#define SDA_PIN 8
#define SCL_PIN 9
VL53L0X s1, s2, s3, s4;
VL53L0X* sensors[] = {&s1, &s2, &s3, &s4};

PCF8574 pcf(0x20);
#include "vl.h"
TB6612 motores(7,6,10,20,5,21);//(AI1, AI2, BI1, BI2, PWMA, PWMB)

#include <Arduino.h>
#include <IRrecv.h>
#include <IRutils.h> 

const uint16_t IR_PIN = 4;

IRrecv irrecv(IR_PIN);
decode_results results; 

#define led 3
#include "IR_Remote.h"
void setup() {
init_vl();
IR_Init(); // Inicia el receptor IR
motores.init_pines();
 }

void loop() {
  IR_ProcessSignals();
  if (IR_getBattleStatus()) {
        // Modo de Combate
    int angulo = IR_getInitialAngle();
    int estrategia = IR_getStrategy();
    ejecutarEstrategia(estrategia, angulo);
    
  } else {
    // Modo de Configuración
    
    Serial.print("Angulo: ");
    Serial.print(IR_getInitialAngle());
    Serial.print(" | Estrategia: ");
    Serial.println(IR_getStrategy());
     
         
  }
  uint16_t d1 = s1.readRangeContinuousMillimeters();
  uint16_t d2 = s2.readRangeContinuousMillimeters();
  uint16_t d3 = s3.readRangeContinuousMillimeters();
  uint16_t d4 = s4.readRangeContinuousMillimeters();

  Serial.print(d1); Serial.print(",");
  Serial.print(d2); Serial.print(",");
  Serial.print(d3); Serial.print(",");
  Serial.println(d4);

  delay(5); 
}
void ejecutarEstrategia(int estrategia, int angulo) {
//falta hacer las diferentes estrategias y angulos de inicio
}