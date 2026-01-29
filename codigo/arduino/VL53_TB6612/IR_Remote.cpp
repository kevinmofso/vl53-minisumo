

#include "IR_Remote.h"
#include "TB6612.h" 


extern TB6612 motores; 


static bool isBattleActive = false; // true = Batalla activa, false = Configuración/Pausa
static int currentAngle = 0;       // Ángulo inicial de giro (ajustado con +/-)
static int currentStrategy = 0;    // Estrategia seleccionada (0 = Configuración, 1-9 = Estrategia)


void toggleBattle() {
    isBattleActive = !isBattleActive;
    if (isBattleActive) {
        Serial.println(">>> INICIO: Combate programado.");
    
    } else {
        Serial.println(">>> PAUSA/CONFIGURACIÓN.");
    }
}

void changeAngle(int step) {
    currentAngle += step;
    // Limitamos el ángulo de -90 a 90 grados
    if (currentAngle > 90) currentAngle = 90;
    if (currentAngle < -90) currentAngle = -90;
    Serial.print(">>> ANGULO INICIAL: ");
    Serial.print(currentAngle);
    Serial.println(" grados.");
}

void selectStrategy(int strategy) {
    currentStrategy = strategy;
    Serial.print(">>> ESTRATEGIA: ");
    Serial.print(strategy);
    Serial.println(" seleccionada.");
}

void IR_RunMotorTest() {

    motores.adelante(255); 
    
   
}

void IR_StopMotors() {
    Serial.println(">>> [ACCIÓN FÍSICA]: Detener todos los motores.");
    

    motores.frenar(); 
    
 
}


void procesarCodigo(uint64_t codigoRecibido) {
    switch (codigoRecibido) {
        
        //BATALLA Y PAUSA
        case B_ENCENDIDO:
            toggleBattle();
            break;
        case B_PAUSA:
            isBattleActive = false;
            IR_StopMotors(); 
            break;

        //  CALIBRACIÓN DE ÁNGULO (+/- 15 grados) 
        case B_MAS:
            changeAngle(15);
            break;
        case B_MENOS:
            changeAngle(-15);
            break;

        //  PRUEBA DE MOTOR
        case B_SILENCIAR:
            IR_RunMotorTest();
            break;

        //  SELECCIÓN DE ESTRATEGIAS (0-9) 
        case B_0: selectStrategy(0); break;
        case B_1: selectStrategy(1); break;
        case B_2: selectStrategy(2); break;
        case B_3: selectStrategy(3); break;
        case B_4: selectStrategy(4); break;
        case B_5: selectStrategy(5); break;
        case B_6: selectStrategy(6); break;
        case B_7: selectStrategy(7); break;
        case B_8: selectStrategy(8); break;
        case B_9: selectStrategy(9); break;
        
        default:
            Serial.println(">>> Botón no mapeado.");
            break;
    }
}


void IR_Init() {
    irrecv.enableIRIn();
    Serial.println("IR Remoto Inicializado. Esperando comandos.");
}

bool IR_getBattleStatus() {
    return isBattleActive;
}

int IR_getInitialAngle() {
    return currentAngle;
}

int IR_getStrategy() {
    return currentStrategy;
}

void IR_ProcessSignals() {
    if (irrecv.decode(&results)) {
        uint64_t codigoRecibido = results.value;
        
        Serial.print("IR Recibido: 0x");
        serialPrintUint64(codigoRecibido, HEX); 
        Serial.println();
        
        if (codigoRecibido != 0) {
            procesarCodigo(codigoRecibido);
        }
        Serial.println("---");
        irrecv.resume();
    }
}
