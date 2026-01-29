#include <Wire.h>

#include <VL53L0X.h>

#include <PCF8574.h>

#include "TB6612.h"

#include "IR_Remote.h"

#include "QTR1A.h"

#define SDA_PIN 8

#define SCL_PIN 9

#include "vl.h"

TB6612 motores(7, 6, 10, 20, 5, 21);  //(AI1, AI2, BI1, BI2, PWMA, PWMB)

uint16_t lecturas_mm[NUM_SENSORS];

#include <Arduino.h>

#include <IRrecv.h>

#include <IRutils.h>



const uint16_t IR_PIN = 2;



IRrecv irrecv(IR_PIN);

decode_results results;



void setup() {

  Serial.begin(115200);



  IR_Init();

  motores.init_pines();

  init_sensores_piso();

  init_vl();
}



void loop() {



  IR_ProcessSignals();

  int angulo = IR_getInitialAngle();

  int estrategia = IR_getStrategy();

  processing(estrategia, angulo);

  if (IR_getBattleStatus()) {



    // Modo de Combate





  } else {

    // Modo de Configuración



    Serial.print("Angulo: ");

    Serial.print(IR_getInitialAngle());

    Serial.print(" | Estrategia: ");

    Serial.println(IR_getStrategy());
  }
}

void processing(int estrategia, int angulo) {

  // 1. Enviamos el Estado y Comando con sus letras (E y C)

  // Así Processing sabe que NO son datos del radar

  Serial.print("E,ESTRATEGIA: ");

  Serial.println(estrategia);



  Serial.print("C,ANGULO: ");

  Serial.println(angulo);



  // 2. Obtenemos lecturas frescas

  leerTodosLosSensores(lecturas_mm);

  int q_izq, q_der;  // Variables locales (Perfecto)

  leer_piso(q_izq, q_der);



  // 3. ENVIAR TRAMA PARA EL RADAR (Cabecera "D")

  Serial.print("D");

  for (int i = 0; i < NUM_SENSORS; i++) {

    Serial.print(",");

    Serial.print(lecturas_mm[i]);
  }

  Serial.print(",");

  Serial.print(q_izq);

  Serial.print(",");

  Serial.println(q_der);
}