#include "vl.h"

// Array de N instancias de los sensores VL53L0X
VL53L0X sensor_instances[NUM_SENSORS];

// Array de N punteros que apuntarán a las instancias anteriores
VL53L0X* sensors[NUM_SENSORS];

// Definición e inicialización del expansor I2C (dirección 0x20 por defecto)
PCF8574 pcf(0x20); 

void inicializar_array_de_punteros() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i] = &sensor_instances[i];
    }
}

void init_vl() {
 
    delay(2000); 
    Serial.println("Iniciando Sensores VL53L0X...");
    // Iniciar I2C en los pines definidos
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);
    
    // Asegurar que los punteros apunten a las instancias correctas
    inicializar_array_de_punteros();

    pcf.begin();


    for(int i = 0; i < NUM_SENSORS; i++) {
        pcf.pinMode(i, OUTPUT);
        pcf.digitalWrite(i, LOW); 
    }
    delay(100); 


    for(int i = 0; i < NUM_SENSORS; i++) {

        pcf.digitalWrite(i, HIGH); 
        delay(10); 
        
        sensors[i]->setTimeout(500);
        

        if (!sensors[i]->init()) {
            Serial.print("Fallo en sensor "); 
            Serial.println(i + 1);
        } else {
            // Cambiar dirección única: 0x30, 0x31, 0x32, 0x33
            sensors[i]->setAddress(0x30 + i);
            
            sensors[i]->setMeasurementTimingBudget(20000);
            
            // Iniciar modo de lectura continuo
            sensors[i]->startContinuous();
            
            Serial.print("Sensor "); 
            Serial.print(i + 1); 
            Serial.print(" configurado en 0x");
            Serial.println(0x30 + i, HEX);
        }
    }
}
void leerTodosLosSensores(uint16_t distancias[NUM_SENSORS]) {
    for (int i = 0; i < NUM_SENSORS; i++) {
        distancias[i] = sensors[i]->readRangeContinuousMillimeters();
    }
}