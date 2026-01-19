#include <Wire.h>
#include <VL53L0X.h>
#include <PCF8574.h>
#include "TB6612.h"
#define SDA_PIN 8
#define SCL_PIN 9

#include "vl.h"
TB6612 motores(7,6,10,20,5,21);//(AI1, AI2, BI1, BI2, PWMA, PWMB)
uint16_t lecturas_mm[NUM_SENSORS];
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
    leerTodosLosSensores(lecturas_mm);
    Serial.print("Distancia S1: ");
    Serial.println(lecturas_mm[0]);
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

}
void ejecutarEstrategia(int estrategia, int angulo) {
//falta hacer las diferentes estrategias y angulos de inicio
}