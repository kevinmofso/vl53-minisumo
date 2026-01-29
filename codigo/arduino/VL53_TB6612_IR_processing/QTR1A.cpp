#include "QTR1A.h"

void init_sensores_piso() {
    // En el ESP32-C3 no es estrictamente necesario pinMode para analogRead,
    // pero ayuda a definir el estado inicial.
    pinMode(QTR_IZQ, INPUT);
    pinMode(QTR_DER, INPUT);
}

void leer_piso(int &izq, int &der) {
    izq = analogRead(QTR_IZQ);
    der = analogRead(QTR_DER);
}

