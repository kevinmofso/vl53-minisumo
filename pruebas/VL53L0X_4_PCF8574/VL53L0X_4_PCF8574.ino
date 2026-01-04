#include <Wire.h>
#include <VL53L0X.h>
#include <PCF8574.h>

// Pines ESP32-C3 Super Mini
#define SDA_PIN 8
#define SCL_PIN 9

// Instancias de los sensores
VL53L0X s1, s2, s3, s4;
VL53L0X* sensors[] = {&s1, &s2, &s3, &s4};

// Instancia del expansor
PCF8574 pcf(0x20);

void setup() {
  Serial.begin(115200);
  // USB CDC On Boot debe estar Enabled en el menú Herramientas
  delay(2000); 

  Serial.println("--- Iniciando con Libreria Pololu ---");

  // Configurar I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  pcf.begin();

  // 1. Reset físico: Apagar todos los sensores
  for(int i=0; i<4; i++) {
    pcf.pinMode(i, OUTPUT);
    pcf.digitalWrite(i, LOW);
  }
  delay(10);

  // 2. Inicializar y asignar direcciones
  for(int i=0; i<4; i++) {
    pcf.digitalWrite(i, HIGH); // Encender sensor actual
    delay(10);
    
    sensors[i]->setTimeout(500);
    if (!sensors[i]->init()) {
      Serial.print("Fallo en sensor "); Serial.println(i+1);
    } else {
      // Cambiar dirección (0x30, 0x31, 0x32, 0x33)
      sensors[i]->setAddress(0x30 + i);
      
      // Configurar para alta velocidad (Timing budget a 20ms)
      sensors[i]->setMeasurementTimingBudget(20000);
      
      // Iniciar modo continuo
      sensors[i]->startContinuous();
      Serial.print("Sensor "); Serial.print(i+1); Serial.println(" configurado.");
    }
  }
}

void loop() {
  // Lectura ultra rápida
  // readRangeContinuousMillimeters() devuelve el dato del registro sin esperas
  uint16_t d1 = s1.readRangeContinuousMillimeters();
  uint16_t d2 = s2.readRangeContinuousMillimeters();
  uint16_t d3 = s3.readRangeContinuousMillimeters();
  uint16_t d4 = s4.readRangeContinuousMillimeters();

  // Comprobar si hubo timeout en alguno
  if (s1.timeoutOccurred()) Serial.print("T1!");
  
  // Imprimir para Serial Plotter (formato: valor1,valor2,valor3,valor4)
  Serial.print(d1); Serial.print(",");
  Serial.print(d2); Serial.print(",");
  Serial.print(d3); Serial.print(",");
  Serial.println(d4);

  // El ESP32-C3 es tan rápido que si no pones un delay mínimo, 
  // el monitor serial podría saturarse. 
  delay(5); 
}