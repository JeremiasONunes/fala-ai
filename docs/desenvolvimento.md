# ⚙️ Guia de Desenvolvimento

## 🎯 Etapas de Desenvolvimento

### Fase 1: Configuração Básica ✅

| Etapa | Descrição | Status | Resultado Esperado |
|-------|-----------|--------|-------------------|
| 1.1 | Testar servos individualmente | 🔄 | Movimento dos dedos calibrado |
| 1.2 | Configurar comunicação serial | 🔄 | Arduino recebe comandos Python |
| 1.3 | Implementar reconhecimento básico | 🔄 | Conversão voz → texto |

### Fase 2: Integração Principal 🔄

| Etapa | Descrição | Status | Resultado Esperado |
|-------|-----------|--------|-------------------|
| 2.1 | Mapear letras para posições | 📋 | Alfabeto A-Z completo |
| 2.2 | Integrar Python ↔ Arduino | 📋 | Pipeline voz → movimento |
| 2.3 | Calibrar tempos e delays | 📋 | Movimentos sincronizados |

### Fase 3: Otimização e Testes 📋

| Etapa | Descrição | Status | Resultado Esperado |
|-------|-----------|--------|-------------------|
| 3.1 | Testes de precisão | 📋 | >90% de acerto |
| 3.2 | Otimização de performance | 📋 | <2s por palavra |
| 3.3 | Interface de usuário | 📋 | Sistema user-friendly |

### Fase 4: Finalização 📋

| Etapa | Descrição | Status | Resultado Esperado |
|-------|-----------|--------|-------------------|
| 4.1 | Documentação completa | 📋 | Manuais e guias |
| 4.2 | Testes finais | 📋 | Sistema robusto |
| 4.3 | Preparação para demo | 📋 | Apresentação funcional |

**Legenda**: ✅ Concluído | 🔄 Em andamento | 📋 Planejado

## 🛠️ Estrutura do Código

### Organização de Arquivos

```
src/
├── python/
│   ├── main.py                 # Script principal
│   ├── modules/
│   │   ├── __init__.py
│   │   ├── speech_recognition.py    # Reconhecimento de voz
│   │   ├── serial_communication.py # Comunicação serial
│   │   ├── audio_processor.py      # Processamento de áudio
│   │   └── config_manager.py       # Gerenciamento de configurações
│   ├── utils/
│   │   ├── __init__.py
│   │   ├── logger.py              # Sistema de logs
│   │   └── helpers.py             # Funções auxiliares
│   └── tests/
│       ├── test_speech.py
│       ├── test_serial.py
│       └── test_integration.py
└── arduino/
    ├── servo_control/
    │   ├── servo_control.ino      # Código principal
    │   ├── servo_positions.h      # Definições de posições
    │   └── communication.h        # Protocolo de comunicação
    └── tests/
        ├── test_servos.ino
        └── test_serial.ino
```

## 🐍 Desenvolvimento Python

### Módulo Principal (main.py)

```python
#!/usr/bin/env python3
"""
Tradutor de Voz para Libras
Sistema principal de integração
"""

import logging
from modules.speech_recognition import SpeechRecognizer
from modules.serial_communication import ArduinoController
from modules.config_manager import Config
from utils.logger import setup_logger

def main():
    # Configurar logging
    logger = setup_logger()
    logger.info("Iniciando Tradutor de Libras")
    
    try:
        # Carregar configurações
        config = Config()
        
        # Inicializar componentes
        speech = SpeechRecognizer(config)
        arduino = ArduinoController(config)
        
        # Loop principal
        while True:
            texto = speech.listen_and_convert()
            if texto:
                arduino.send_text(texto)
                
    except KeyboardInterrupt:
        logger.info("Sistema encerrado pelo usuário")
    except Exception as e:
        logger.error(f"Erro crítico: {e}")
    finally:
        arduino.close()

if __name__ == "__main__":
    main()
```

### Reconhecimento de Voz (speech_recognition.py)

```python
import speech_recognition as sr
import logging

class SpeechRecognizer:
    def __init__(self, config):
        self.config = config
        self.logger = logging.getLogger(__name__)
        self.recognizer = sr.Recognizer()
        
    def listen_and_convert(self):
        """Captura áudio e converte para texto"""
        try:
            with sr.Microphone() as source:
                self.logger.info("Ajustando para ruído ambiente...")
                self.recognizer.adjust_for_ambient_noise(source)
                
                self.logger.info("Escutando...")
                audio = self.recognizer.listen(
                    source, 
                    timeout=self.config.timeout,
                    phrase_time_limit=self.config.phrase_time_limit
                )
            
            # Converter para texto
            texto = self.recognizer.recognize_google(
                audio, 
                language=self.config.speech_language
            ).upper().strip()
            
            self.logger.info(f"Texto reconhecido: {texto}")
            return texto
            
        except sr.WaitTimeoutError:
            self.logger.warning("Timeout - nenhum áudio detectado")
            return None
        except sr.UnknownValueError:
            self.logger.warning("Não foi possível entender o áudio")
            return None
        except sr.RequestError as e:
            self.logger.error(f"Erro na API: {e}")
            return None
```

### Comunicação Serial (serial_communication.py)

```python
import serial
import time
import logging
from serial.tools import list_ports

class ArduinoController:
    def __init__(self, config):
        self.config = config
        self.logger = logging.getLogger(__name__)
        self.connection = self._connect()
        
    def _find_arduino_port(self):
        """Detecta automaticamente a porta do Arduino"""
        ports = list_ports.comports()
        for port in ports:
            if 'Arduino' in port.description or 'CH340' in port.description:
                return port.device
        return None
    
    def _connect(self):
        """Estabelece conexão serial com Arduino"""
        try:
            port = self.config.ARDUINO_PORT or self._find_arduino_port()
            if not port:
                raise Exception("Arduino não encontrado")
                
            connection = serial.Serial(
                port=port,
                baudrate=self.config.BAUD_RATE,
                timeout=1
            )
            time.sleep(2)  # Aguardar inicialização
            
            self.logger.info(f"Conectado ao Arduino em {port}")
            return connection
            
        except Exception as e:
            self.logger.error(f"Erro na conexão: {e}")
            raise
    
    def send_text(self, texto):
        """Envia texto letra por letra para o Arduino"""
        for letra in texto:
            if letra.isalpha():
                self.send_letter(letra)
                time.sleep(self.config.SERVO_DELAY)
    
    def send_letter(self, letra):
        """Envia uma letra específica"""
        try:
            command = f"{letra}\n"
            self.connection.write(command.encode())
            self.logger.debug(f"Enviado: {letra}")
        except Exception as e:
            self.logger.error(f"Erro ao enviar {letra}: {e}")
    
    def close(self):
        """Fecha a conexão serial"""
        if self.connection and self.connection.is_open:
            self.connection.close()
            self.logger.info("Conexão serial fechada")
```

## 🤖 Desenvolvimento Arduino

### Código Principal (servo_control.ino)

```cpp
#include <Servo.h>
#include "servo_positions.h"
#include "communication.h"

// Declaração dos servos
Servo servos[NUM_SERVOS];
int servoPins[NUM_SERVOS] = {3, 5, 6, 9, 10};

String receivedData = "";
bool dataComplete = false;

void setup() {
    Serial.begin(BAUD_RATE);
    
    // Inicializar servos
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].attach(servoPins[i]);
        servos[i].write(90); // Posição neutra
    }
    
    delay(1000);
    Serial.println("Arduino pronto para receber comandos");
}

void loop() {
    // Verificar dados seriais
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        
        if (inChar == '\n') {
            dataComplete = true;
        } else {
            receivedData += inChar;
        }
    }
    
    // Processar comando completo
    if (dataComplete) {
        processCommand(receivedData);
        receivedData = "";
        dataComplete = false;
    }
}

void processCommand(String command) {
    command.toUpperCase();
    command.trim();
    
    if (command.length() == 1) {
        char letter = command.charAt(0);
        executeLetterPosition(letter);
        Serial.println("OK: " + command);
    } else {
        Serial.println("ERROR: Comando inválido");
    }
}

void executeLetterPosition(char letter) {
    int* positions = getLetterPositions(letter);
    
    if (positions != nullptr) {
        // Mover servos para as posições
        for (int i = 0; i < NUM_SERVOS; i++) {
            servos[i].write(positions[i]);
        }
        
        delay(MOVEMENT_DELAY);
    } else {
        Serial.println("ERROR: Letra não mapeada - " + String(letter));
    }
}
```

### Posições dos Servos (servo_positions.h)

```cpp
#ifndef SERVO_POSITIONS_H
#define SERVO_POSITIONS_H

#define NUM_SERVOS 5
#define MOVEMENT_DELAY 500

// Posições para cada letra (polegar, indicador, médio, anelar, mindinho)
int positionsA[NUM_SERVOS] = {90, 0, 0, 0, 0};    // A: punho fechado, polegar para fora
int positionsB[NUM_SERVOS] = {0, 90, 90, 90, 90}; // B: dedos estendidos, polegar dobrado
int positionsC[NUM_SERVOS] = {60, 60, 60, 60, 60}; // C: dedos curvados
int positionsD[NUM_SERVOS] = {0, 90, 0, 0, 0};     // D: só indicador estendido
int positionsE[NUM_SERVOS] = {0, 0, 0, 0, 0};      // E: punho fechado

// Função para obter posições de uma letra
int* getLetterPositions(char letter) {
    switch(letter) {
        case 'A': return positionsA;
        case 'B': return positionsB;
        case 'C': return positionsC;
        case 'D': return positionsD;
        case 'E': return positionsE;
        // ... adicionar todas as letras
        default: return nullptr;
    }
}

#endif
```

## 🧪 Testes e Validação

### Testes Unitários Python

```python
# tests/test_speech.py
import unittest
from modules.speech_recognition import SpeechRecognizer
from modules.config_manager import Config

class TestSpeechRecognition(unittest.TestCase):
    def setUp(self):
        self.config = Config()
        self.recognizer = SpeechRecognizer(self.config)
    
    def test_recognizer_setup(self):
        """Testa se o reconhecedor foi configurado corretamente"""
        self.assertIsNotNone(self.recognizer.recognizer)
    
    def test_microphone_list(self):
        """Testa listagem de microfones"""
        import speech_recognition as sr
        try:
            mics = sr.Microphone.list_microphone_names()
            self.assertIsInstance(mics, list)
        except Exception as e:
            self.fail(f"Falha ao listar microfones: {e}")

if __name__ == '__main__':
    unittest.main()
```

### Testes de Integração

```python
# tests/test_integration.py
import unittest
import time
from modules.serial_communication import ArduinoController
from modules.config_manager import Config

class TestIntegration(unittest.TestCase):
    def setUp(self):
        self.config = Config()
        try:
            self.arduino = ArduinoController(self.config)
        except:
            self.skipTest("Arduino não conectado")
    
    def test_letter_sending(self):
        """Testa envio de letras individuais"""
        test_letters = ['A', 'B', 'C']
        
        for letter in test_letters:
            with self.subTest(letter=letter):
                try:
                    self.arduino.send_letter(letter)
                    time.sleep(1)  # Aguardar movimento
                except Exception as e:
                    self.fail(f"Falha ao enviar {letter}: {e}")
    
    def tearDown(self):
        if hasattr(self, 'arduino'):
            self.arduino.close()
```

## 📊 Métricas e Monitoramento

### Sistema de Logs

```python
# utils/logger.py
import logging
import os
from datetime import datetime

def setup_logger(level=logging.INFO):
    """Configura sistema de logging"""
    
    # Criar diretório de logs
    os.makedirs('logs', exist_ok=True)
    
    # Nome do arquivo com timestamp
    log_file = f"logs/tradutor_{datetime.now().strftime('%Y%m%d_%H%M%S')}.log"
    
    # Configurar formatação
    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
    )
    
    # Handler para arquivo
    file_handler = logging.FileHandler(log_file)
    file_handler.setFormatter(formatter)
    
    # Handler para console
    console_handler = logging.StreamHandler()
    console_handler.setFormatter(formatter)
    
    # Configurar logger raiz
    logger = logging.getLogger()
    logger.setLevel(level)
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)
    
    return logger
```

### Métricas de Performance

```python
# utils/metrics.py
import time
from functools import wraps

class PerformanceMetrics:
    def __init__(self):
        self.metrics = {}
    
    def time_function(self, func_name):
        """Decorator para medir tempo de execução"""
        def decorator(func):
            @wraps(func)
            def wrapper(*args, **kwargs):
                start_time = time.time()
                result = func(*args, **kwargs)
                end_time = time.time()
                
                execution_time = end_time - start_time
                self.record_metric(func_name, execution_time)
                
                return result
            return wrapper
        return decorator
    
    def record_metric(self, name, value):
        """Registra uma métrica"""
        if name not in self.metrics:
            self.metrics[name] = []
        self.metrics[name].append(value)
    
    def get_average(self, name):
        """Calcula média de uma métrica"""
        if name in self.metrics:
            return sum(self.metrics[name]) / len(self.metrics[name])
        return 0
    
    def report(self):
        """Gera relatório de performance"""
        for name, values in self.metrics.items():
            avg = self.get_average(name)
            print(f"{name}: {avg:.3f}s (média de {len(values)} execuções)")
```

## 🔧 Ferramentas de Desenvolvimento

### Makefile para Automação

```makefile
# Makefile
.PHONY: install test run clean

# Instalar dependências
install:
	python -m pip install --upgrade pip
	pip install -r requirements.txt

# Executar testes
test:
	python -m pytest tests/ -v

# Executar sistema principal
run:
	python src/python/main.py

# Limpeza de arquivos temporários
clean:
	find . -type f -name "*.pyc" -delete
	find . -type d -name "__pycache__" -delete
	rm -rf logs/*.log

# Verificar código
lint:
	flake8 src/python/
	pylint src/python/

# Gerar documentação
docs:
	sphinx-build -b html docs/ docs/_build/
```

### Scripts de Automação

```bash
#!/bin/bash
# scripts/deploy.sh

echo "🚀 Iniciando deploy do Tradutor de Libras"

# Verificar Python
python --version || { echo "Python não encontrado"; exit 1; }

# Criar ambiente virtual se não existir
if [ ! -d "venv" ]; then
    echo "Criando ambiente virtual..."
    python -m venv venv
fi

# Ativar ambiente virtual
source venv/bin/activate

# Instalar dependências
echo "Instalando dependências..."
pip install -r requirements.txt

# Executar testes
echo "Executando testes..."
python -m pytest tests/ || { echo "Testes falharam"; exit 1; }

# Verificar Arduino
echo "Verificando conexão com Arduino..."
python scripts/check_arduino.py || { echo "Arduino não encontrado"; exit 1; }

echo "✅ Deploy concluído com sucesso!"
echo "Execute: python src/python/main.py"
```