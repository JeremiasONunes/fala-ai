# 💻 Software e Dependências

## 🐍 Ambiente Python

### Versão Requerida
- **Python**: 3.8+ (recomendado 3.10+)
- **Sistema**: Windows 10/11, Linux, macOS

### Bibliotecas Principais

| Biblioteca | Versão | Função |
|------------|--------|--------|
| `speechrecognition` | >=3.10.0 | Reconhecimento de fala |
| `pyaudio` | >=0.2.11 | Captura de áudio do microfone |
| `pyserial` | >=3.5 | Comunicação serial |
| `numpy` | >=1.21.0 | Processamento numérico |

### Bibliotecas Opcionais

| Biblioteca | Função | Uso |
|------------|--------|-----|
| `matplotlib` | Visualização | Debug e análise |


## 🔧 Arduino IDE

### Versão e Configuração
- **Arduino IDE**: 2.0+ (recomendado)
- **Board**: Arduino Uno/Nano
- **Processor**: ATmega328P
- **Port**: Detectado automaticamente

### Bibliotecas Arduino

```cpp
#include <Servo.h>  // Biblioteca padrão do Arduino
```

### Configurações de Upload
- **Baud Rate**: 115200 (upload)
- **Programmer**: AVRISP mkII
- **Board Manager**: Arduino AVR Boards

## 📦 Instalação das Dependências

### 1. Criar Ambiente Virtual

```bash
# Windows
python -m venv venv
venv\Scripts\activate

# Linux/macOS  
python3 -m venv venv
source venv/bin/activate
```

### 2. Instalar Dependências Python

```bash
pip install --upgrade pip
pip install speechrecognition pyaudio pyserial numpy
```

### 3. Arquivo requirements.txt

```txt
speechrecognition>=3.10.0
pyaudio>=0.2.11
pyserial>=3.5
numpy>=1.21.0
matplotlib>=3.5.0
```

### 4. Instalação via requirements

```bash
pip install -r requirements.txt
```

## 🎤 APIs de Reconhecimento de Fala

### SpeechRecognition (Interface)

```python
import speech_recognition as sr

# Configurar interface
r = sr.Recognizer()

# Capturar áudio do microfone
with sr.Microphone() as source:
    audio = r.listen(source)

# Enviar para API do Google e receber texto
text = r.recognize_google(audio, language='pt-BR')
```

### APIs Disponíveis

| API | Qualidade | Velocidade | Conectividade |
|-----|-----------|------------|---------------|
| Google Speech API | Excelente | Rápida | Online |
| Sphinx (Local) | Boa | Média | Offline |
| Wit.ai API | Muito boa | Rápida | Online |
| Microsoft Bing API | Excelente | Rápida | Online |

## 🔌 Configuração Serial

### Detecção de Porta

```python
import serial.tools.list_ports

def encontrar_arduino():
    portas = serial.tools.list_ports.comports()
    for porta in portas:
        if 'Arduino' in porta.description or 'CH340' in porta.description:
            return porta.device
    return None
```

### Configuração da Conexão

```python
import serial
import time

arduino = serial.Serial(
    port='COM3',        # Windows
    # port='/dev/ttyUSB0',  # Linux
    # port='/dev/cu.usbmodem',  # macOS
    baudrate=9600,
    timeout=1
)
time.sleep(2)  # Aguardar inicialização
```

## 🎤 Configuração de Áudio

### Teste do Microfone

```python
import speech_recognition as sr

def testar_microfone():
    r = sr.Recognizer()
    
    # Listar microfones disponíveis
    print("Microfones disponíveis:")
    for index, name in enumerate(sr.Microphone.list_microphone_names()):
        print(f"{index}: {name}")
    
    # Testar microfone padrão
    with sr.Microphone() as source:
        r.adjust_for_ambient_noise(source)
        print("Microfone configurado!")
```

### Configuração Recomendada

```python
# Parâmetros do SpeechRecognition
LANGUAGE = 'pt-BR'   # Português brasileiro
TIMEOUT = 5          # Timeout para captura
PHRASE_TIME_LIMIT = 5  # Limite de tempo da frase
```

## 🛠️ Estrutura do Projeto

```
projeto-libras/
├── docs/                    # Documentação
├── src/                     # Código fonte
│   ├── python/             # Scripts Python
│   │   ├── main.py         # Script principal
│   │   ├── speech_recognition.py
│   │   ├── serial_communication.py
│   │   └── config.py
│   └── arduino/            # Código Arduino
│       └── servo_control.ino
├── models/                 # Modelos 3D
├── requirements.txt        # Dependências Python
├── README.md              # Documentação principal
└── .gitignore            # Arquivos ignorados
```

## ⚙️ Configurações do Sistema

### Variáveis de Ambiente

```bash
# .env
ARDUINO_PORT=COM3
BAUD_RATE=9600
AUDIO_DEVICE=0
SPEECH_LANGUAGE=pt-BR
```

### Arquivo de Configuração

```python
# config.py
import os
from dataclasses import dataclass

@dataclass
class Config:
    # Serial
    arduino_port: str = os.getenv('ARDUINO_PORT', 'COM3')
    baud_rate: int = int(os.getenv('BAUD_RATE', '9600'))
    
    # Audio
    sample_rate: int = 16000
    channels: int = 1
    chunk_size: int = 1024
    
    # API Configuration
    speech_language: str = os.getenv('SPEECH_LANGUAGE', 'pt-BR')
    timeout: int = 5
    phrase_time_limit: int = 5
```

## 🔍 Troubleshooting de Software

### Problemas Comuns

1. **Erro de Porta Serial**
   ```python
   # Verificar se a porta está disponível
   import serial.tools.list_ports
   print([port.device for port in serial.tools.list_ports.comports()])
   ```

2. **Erro de API**
   ```python
   # Testar diferentes APIs
   try:
       text = r.recognize_google(audio, language='pt-BR')
   except sr.RequestError:
       text = r.recognize_sphinx(audio)  # Offline como backup
   ```

3. **Erro de Áudio**
   ```python
   # Verificar microfones disponíveis
   import speech_recognition as sr
   print(sr.Microphone.list_microphone_names())
   ```

### Logs e Debug

```python
import logging

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('projeto_libras.log'),
        logging.StreamHandler()
    ]
)
```