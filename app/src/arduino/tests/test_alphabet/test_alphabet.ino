/*
  TESTE DO ALFABETO LIBRAS - Arduino
  Executa todas as letras A-Z automaticamente
  Desenvolvido por: Jeremias Maya
*/

#include <Servo.h>

#define NUM_SERVOS 5
#define MOVEMENT_DELAY 2000  // 2 segundos por letra

// Servos para os 5 dedos
Servo servos[NUM_SERVOS];
int servoPins[NUM_SERVOS] = {3, 5, 6, 9, 10};
String servoNames[NUM_SERVOS] = {"Polegar", "Indicador", "Medio", "Anelar", "Mindinho"};

// ===========================================================
// POSIÇÕES DO ALFABETO MANUAL LIBRAS
// ===========================================================

// [Polegar, Indicador, Médio, Anelar, Mindinho]
int positionsA[NUM_SERVOS] = {0, 180, 180, 180, 180};      // A: punho fechado, polegar para fora
int positionsB[NUM_SERVOS] = {180, 0, 0, 0, 0};            // B: dedos estendidos, polegar dobrado
int positionsC[NUM_SERVOS] = {90, 130, 120, 100, 100};         // C: mão curvada em C
int positionsD[NUM_SERVOS] = {180, 0, 180, 180, 180};            // D: só indicador estendido
int positionsE[NUM_SERVOS] = {180, 180, 180, 180, 180};    // E: punho totalmente fechado
int positionsF[NUM_SERVOS] = {180, 180, 0, 0, 0};          // F: polegar toca indicador
int positionsG[NUM_SERVOS] = {80, 80, 180, 180, 180};          // G: indicador e polegar horizontal
int positionsH[NUM_SERVOS] = {180, 0, 0, 180, 180};          // H: indicador e médio juntos
int positionsI[NUM_SERVOS] = {180, 180, 180, 180, 0};      // I: só mindinho estendido
int positionsJ[NUM_SERVOS] = {180, 180, 180, 180, 0};      // J: mindinho com movimento
int positionsK[NUM_SERVOS] = {70, 80, 0, 180, 180};        // K: polegar entre indicador e médio
int positionsL[NUM_SERVOS] = {0, 0, 180, 180, 180};        // L: polegar e indicador em L
int positionsM[NUM_SERVOS] = {180, 180, 180, 180, 180};    // M: polegar sob punho fechado
int positionsN[NUM_SERVOS] = {180, 180, 180, 180, 180};    // N: similar ao M
int positionsO[NUM_SERVOS] = {90, 130, 120, 100, 100};         // O: todos dedos curvados
int positionsP[NUM_SERVOS] = {100, 130, 0, 180, 180};          // P: similar ao K invertido
int positionsQ[NUM_SERVOS] = {50, 70, 180, 180, 180};          // Q: polegar e indicador para baixo
int positionsR[NUM_SERVOS] = {0, 180, 180, 0, 0};          // R: indicador cruza médio
int positionsS[NUM_SERVOS] = {180, 180, 180, 180, 180};    // S: punho fechado com polegar
int positionsT[NUM_SERVOS] = {180, 180, 0, 0, 0};          // T: polegar entre indicador
int positionsU[NUM_SERVOS] = {180, 0, 0, 180, 180};          // U: indicador e médio juntos
int positionsV[NUM_SERVOS] = {180, 0, 0, 180, 180};          // V: indicador e médio em V
int positionsW[NUM_SERVOS] = {180, 0, 0, 0, 180};        // W: três dedos estendidos
int positionsX[NUM_SERVOS] = {180, 90, 180, 180, 180};           // X: indicador dobrado
int positionsY[NUM_SERVOS] = {0, 180, 180, 180, 0};            // Y: polegar e mindinho
int positionsZ[NUM_SERVOS] = {180, 0, 180, 180, 180};            // Z: indicador faz movimento Z

// ===========================================================
// Função para retornar o vetor de posições de cada letra
// ===========================================================
int* getLetterPositions(char letter) {
  switch (letter) {
    case 'A': return positionsA;
    case 'B': return positionsB;
    case 'C': return positionsC;
    case 'D': return positionsD;
    case 'E': return positionsE;
    case 'F': return positionsF;
    case 'G': return positionsG;
    case 'H': return positionsH;
    case 'I': return positionsI;
    case 'J': return positionsJ;
    case 'K': return positionsK;
    case 'L': return positionsL;
    case 'M': return positionsM;
    case 'N': return positionsN;
    case 'O': return positionsO;
    case 'P': return positionsP;
    case 'Q': return positionsQ;
    case 'R': return positionsR;
    case 'S': return positionsS;
    case 'T': return positionsT;
    case 'U': return positionsU;
    case 'V': return positionsV;
    case 'W': return positionsW;
    case 'X': return positionsX;
    case 'Y': return positionsY;
    case 'Z': return positionsZ;
    default: return nullptr;
  }
}

// ===========================================================
// SETUP
// ===========================================================
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
  }

  delay(2000);

  Serial.println("=== TESTE DO ALFABETO LIBRAS ===");
  Serial.println("Comandos:");
  Serial.println("1 - Executar alfabeto completo A-Z");
  Serial.println("2 - Executar letra específica (A-Z)");
  Serial.println("R - Reset (posição 0)");
  Serial.println("Pronto para comandos!");
}

// ===========================================================
// LOOP PRINCIPAL
// ===========================================================
void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();

    if (command == "1") {
      executeFullAlphabet();
    } 
    else if (command == "2") {
      Serial.println("Digite uma letra (A-Z):");
      while (!Serial.available()) delay(10);
      String letter = Serial.readStringUntil('\n');
      letter.trim();
      letter.toUpperCase();
      if (letter.length() == 1) {
        executeLetter(letter.charAt(0));
      }
    } 
    else if (command == "R") {
      resetServos();
    } 
    else if (command.length() == 1 && command.charAt(0) >= 'A' && command.charAt(0) <= 'Z') {
      executeLetter(command.charAt(0));
    } 
    else {
      Serial.println("Comando inválido!");
    }
  }
}

// ===========================================================
// EXECUÇÃO DE TODO O ALFABETO
// ===========================================================
void executeFullAlphabet() {
  Serial.println("Iniciando teste completo do alfabeto...");
  for (char letter = 'A'; letter <= 'Z'; letter++) {
    executeLetter(letter);
    delay(MOVEMENT_DELAY);
    resetServos();
    delay(500);
  }
  Serial.println("Teste completo finalizado!");
}

// ===========================================================
// EXECUÇÃO DE UMA LETRA ESPECÍFICA
// ===========================================================
void executeLetter(char letter) {
  int* positions = getLetterPositions(letter);
  if (positions == nullptr) {
    Serial.println("Letra inválida!");
    return;
  }

  Serial.print("Executando letra: ");
  Serial.println(letter);

  // Mover servos
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].write(positions[i]);
  }

  // Mostrar posições no Serial
  Serial.print("Posições: ");
  for (int i = 0; i < NUM_SERVOS; i++) {
    Serial.print(servoNames[i]);
    Serial.print("=");
    Serial.print(positions[i]);
    Serial.print("° ");
  }
  Serial.println();
}

// ===========================================================
// RESET DOS SERVOS
// ===========================================================
void resetServos() {
  Serial.println("Reset - todos servos em 0°");
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].write(0);
  }
  delay(300);
}
