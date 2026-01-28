#ifndef QTR1A_H
#define QTR1A_H

#include <Arduino.h>
#define QTR_IZQ 3
#define QTR_DER 4


const int UMBRAL_BLANCO = 1000; 

void init_sensores_piso();
void leer_piso(int &izq, int &der);

#endif