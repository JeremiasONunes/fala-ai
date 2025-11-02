/*
  Teste Individual de Servos - Tradutor Libras v1.0.0
  Testa cada servo separadamente e retorna à posição 0
  
  Comandos:
  1-5: Testa servo específico
  a: Testa todos sequencialmente
  r: Reset todos para posição 0
*/

#include <Servo.h>

Servo servos[5];
int servoPins[5] = {3, 5, 6, 9, 10};
String dedos[5] = {"Polegar", "Indicador", "Medio", "Anelar", "Mindinho"};

void setup() {
  Serial.begin(9600);
  
  // Inicializar servos
  for (int i = 0; i < 5; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);  // Posição inicial 0
  }
  
  delay(1000);
  
  Serial.println("=== TESTE INDIVIDUAL DE SERVOS ===");
  Serial.println("Comandos:");
  Serial.println("1-5: Testar servo (1=Polegar, 2=Indicador, etc)");
  Serial.println("a: Testar todos");
  Serial.println("r: Reset (posição 0)");
  Serial.println("Servos inicializados na posição 0");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    if (cmd >= '1' && cmd <= '5') {
      int servo = cmd - '1';  // Converter para índice 0-4
      testarServo(servo);
    }
    else if (cmd == 'a') {
      testarTodos();
    }
    else if (cmd == 'r') {
      resetServos();
    }
  }
}

void testarServo(int index) {
  Serial.println("Testando " + dedos[index] + " (Pino " + String(servoPins[index]) + ")");
  
  // Movimento de teste: 0 → 90 → 180 → 0
  servos[index].write(0);
  delay(500);
  Serial.println("  Posição 0°");
  
  servos[index].write(90);
  delay(500);
  Serial.println("  Posição 90°");
  
  servos[index].write(180);
  delay(500);
  Serial.println("  Posição 180°");
  
  servos[index].write(0);
  delay(500);
  Serial.println("  Retorno à posição 0°");
  Serial.println("Teste concluído\n");
}

void testarTodos() {
  Serial.println("Testando todos os servos sequencialmente...\n");
  
  for (int i = 0; i < 5; i++) {
    testarServo(i);
    delay(1000);  // Pausa entre servos
  }
  
  Serial.println("Teste completo finalizado - Todos em posição 0°\n");
}

void resetServos() {
  Serial.println("Reset: Movendo todos para posição 0°");
  
  for (int i = 0; i < 5; i++) {
    servos[i].write(0);
    Serial.println(dedos[i] + " → 0°");
  }
  
  Serial.println("Reset concluído\n");
}