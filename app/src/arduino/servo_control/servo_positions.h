/*
  Definições de posições dos servos para alfabeto manual da Libras
  Posições: [Polegar, Indicador, Médio, Anelar, Mindinho]
  Ângulos: 0-180 graus (0=fechado, 180=aberto)
*/

#ifndef SERVO_POSITIONS_H
#define SERVO_POSITIONS_H

#define NUM_SERVOS 5
#define MOVEMENT_DELAY 1000  // Tempo que cada letra fica visível (1 segundo)

// Posições do alfabeto manual da Libras
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


// Função para obter posições de uma letra
int* getLetterPositions(char letter) {
  switch(letter) {
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

#endif