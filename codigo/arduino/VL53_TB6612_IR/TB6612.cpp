#include "TB6612.h"
TB6612::TB6612(int ina1, int ina2, int inb1, int inb2, int APWM, int BPWM) {
  AI1 = ina1;
  AI2 = ina2;
  PWMA = APWM;
  BI1 = inb1;
  BI2 = inb2;
  PWMB = BPWM;
}
void TB6612::init_pines() {
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}
void TB6612::adelante(int velocidad) {
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, LOW);
  analogWrite(PWMB, velocidad);
}
void TB6612::atras(int velocidad) {
  digitalWrite(AI1, LOW);
  digitalWrite(AI2, HIGH);
  analogWrite(PWMA, velocidad);

  digitalWrite(BI1, LOW);
  digitalWrite(BI2, HIGH);
  analogWrite(PWMB, velocidad);
}
void TB6612::frenar() {
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, HIGH);
  analogWrite(PWMA, 0);

  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, HIGH);
  analogWrite(PWMB, 0);
}
void TB6612::giro_izquierda_parado(int velocidad) {
  digitalWrite(AI1, LOW);
  digitalWrite(AI2, HIGH);
  analogWrite(PWMA, velocidad);

  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, LOW);
  analogWrite(PWMB, velocidad);
}
void TB6612::giro_izquierda_suave(int velocidad) {

  int velocidad_izquierdo = velocidad / 2;
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, LOW);
  analogWrite(PWMA, velocidad_izquierdo);

  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, LOW);
  analogWrite(PWMB, velocidad);
}
void TB6612::giro_derecha_parado(int velocidad) {
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(BI1, LOW);
  digitalWrite(BI2, HIGH);
  analogWrite(PWMB, velocidad);
}
void TB6612::giro_derecha_suave(int velocidad) {

  int velocidad_derecha = velocidad / 2;
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, LOW);
  analogWrite(PWMB, velocidad_derecha);
}