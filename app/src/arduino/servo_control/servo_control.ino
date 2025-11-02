/*
  TRADUTOR DE VOZ PARA LIBRAS v1.0.0
  Controle dos servomotores para alfabeto manual
  Desenvolvido por: Jeremias Maya
*/

#include <Servo.h>
#include "servo_positions.h"

// Servos para os 5 dedos
Servo servos[NUM_SERVOS];
int servoPins[NUM_SERVOS] = {3, 5, 6, 9, 10};

String receivedData = "";
bool dataComplete = false;

void setup() {
  Serial.begin(9600);
  
  // Inicializar servos
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0); // Posição 0
  }
  
  delay(1000);
  Serial.println("Arduino pronto");
}

void loop() {
  // Ler dados seriais
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      dataComplete = true;
    } else {
      receivedData += inChar;
    }
  }
  
  // Processar comando
  if (dataComplete) {
    processCommand(receivedData);
    receivedData = "";
    dataComplete = false;
  }
}

void processCommand(String command) {
  command.toUpperCase();
  command.trim();
  
  if (command == "RESET") {
    resetAllServos();
    Serial.println("OK: RESET");
  } else if (command.length() == 1) {
    char letter = command.charAt(0);
    executeLetterPosition(letter);
    Serial.println("OK: " + command);
  } else {
    Serial.println("ERROR: Comando inválido");
  }
}

void resetAllServos() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].write(0);
    delay(10); // Movimento suave para posição 0
  }
  delay(500); // Pausa na posição 0
}

void executeLetterPosition(char letter) {
  int* positions = getLetterPositions(letter);
  
  if (positions != nullptr) {
    // Mover servos suavemente
    for (int i = 0; i < NUM_SERVOS; i++) {
      servos[i].write(positions[i]);
      delay(10); // Pequeno delay entre servos para movimento suave
    }
    delay(MOVEMENT_DELAY); // Manter posição por meio segundo
  } else {
    Serial.println("ERROR: Letra não mapeada - " + String(letter));
  }
}