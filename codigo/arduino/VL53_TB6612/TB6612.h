#ifndef TB6612_h
#define TB6612_h
#include <Arduino.h>
class TB6612 {
private:
  int AI1, AI2, PWMA;  // Motor izquierdo
  int BI1, BI2, PWMB;  // Motor derecho
public:
  //inicializar los pines
  TB6612(int ina1, int ina2, int inb1, int inb2, int APWM, int BPWM);
  void init_pines();
  //poner todos los metodos
  void adelante(int velocidad);
  void atras(int velocidad);
  void frenar();
  void giro_izquierda_parado(int velocidad);
  void giro_izquierda_suave(int velocidad);
  void giro_derecha_parado(int velocidad);
  void giro_derecha_suave(int velocidad);
};
#endif