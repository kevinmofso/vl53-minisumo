#ifndef vl_H
#define vl_H

#include <Arduino.h>
#include <VL53L0X.h>
#include <PCF8574.h>
#define NUM_SENSORS 5

// Definiciones de Pines I2C (si las mantienes en el .ino, puedes dejarlas aquí)
#define SDA_PIN 8
#define SCL_PIN 9
// Array de instancias de los sensores. 
extern VL53L0X sensor_instances[NUM_SENSORS];

// Array de punteros a los sensores
extern VL53L0X* sensors[NUM_SENSORS]; 

// Instancia del expansor I2C PCF8574
extern PCF8574 pcf;



void inicializar_array_de_punteros();


void init_vl();

void leerTodosLosSensores(uint16_t distancias[NUM_SENSORS]);

#endif // vl_H