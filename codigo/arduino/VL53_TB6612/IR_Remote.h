// IR_Remote.h

#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#include <Arduino.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "TB6612.h" // Necesario para declarar el objeto motor

// --- 1. DEFINICIONES DE PINES Y OBJETOS EXTERNOS ---
extern const uint16_t IR_RECEIVE_PIN;
extern IRrecv irrecv;
extern decode_results results;
extern TB6612 motores; 

// Botones de Función (Acciones Críticas)
const uint64_t B_ENCENDIDO   = 0xFFA25D; // INICIAR COMBATE
const uint64_t B_PAUSA       = 0xFF22DD; // ALTO DE EMERGENCIA
const uint64_t B_MAS         = 0xFF906F; // AUMENTAR ANGULO
const uint64_t B_MENOS       = 0xFFA857; // DISMINUIR ANGULO
const uint64_t B_SILENCIAR   = 0xFFE21D; // PRUEBA DE MOTORES

const uint64_t B_MODE        = 0xFF629D;
const uint64_t B_RETROCESO   = 0xFF02FD;
const uint64_t B_ADELANTE    = 0xFFC23D;
const uint64_t B_EQ          = 0xFFE01F;

// Botones Numéricos
const uint64_t B_0 = 0xFF6897;
const uint64_t B_1 = 0xFF30CF;
const uint64_t B_2 = 0xFF18E7;
const uint64_t B_3 = 0xFF7A85;
const uint64_t B_4 = 0xFF10EF;
const uint64_t B_5 = 0xFF38C7;
const uint64_t B_6 = 0xFF5AA5;
const uint64_t B_7 = 0xFF42BD;
const uint64_t B_8 = 0xFF4AB5;
const uint64_t B_9 = 0xFF52AD;


void IR_Init(); 
bool IR_getBattleStatus(); 
int IR_getInitialAngle(); 
int IR_getStrategy();
void IR_ProcessSignals();


void IR_RunMotorTest();
void IR_StopMotors();

#endif 