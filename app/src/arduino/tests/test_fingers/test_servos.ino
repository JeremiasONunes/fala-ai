/*
  Teste individual dos servomotores
  Use para calibrar e testar cada servo
*/

#include <Servo.h>

Servo servos[5];
int servoPins[5] = {3, 5, 6, 9, 10};
String servoNames[5] = {"Polegar", "Indicador", "Medio", "Anelar", "Mindinho"};

void setup() {
  Serial.begin(9600);
  
  // Inicializar servos
  for (int i = 0; i < 5; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(90);
  }
  
  Serial.println("=== TESTE DE SERVOS ===");
  Serial.println("Comandos:");
  Serial.println("0-4: Testar servo específico");
  Serial.println("a: Testar todos");
  Serial.println("r: Reset (posição 90°)");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    if (cmd >= '0' && cmd <= '4') {
      int servo = cmd - '0';
      testServo(servo);
    }
    else if (cmd == 'a') {
      testAllServos();
    }
    else if (cmd == 'r') {
      resetServos();
    }
  }
}

void testServo(int servoIndex) {
  Serial.println("Testando " + servoNames[servoIndex]);
  
  servos[servoIndex].write(0);
  delay(1000);
  servos[servoIndex].write(90);
  delay(1000);
  servos[servoIndex].write(180);
  delay(1000);
  servos[servoIndex].write(90);
  
  Serial.println("Teste concluído");
}

void testAllServos() {
  Serial.println("Testando todos os servos...");
  
  for (int i = 0; i < 5; i++) {
    Serial.println("Servo " + String(i) + ": " + servoNames[i]);
    testServo(i);
    delay(500);
  }
  
  Serial.println("Teste completo finalizado");
}

void resetServos() {
  Serial.println("Reset - todos em 90°");
  for (int i = 0; i < 5; i++) {
    servos[i].write(90);
  }
}