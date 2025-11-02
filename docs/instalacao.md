# 🚀 Guia de Instalação

## 📋 Pré-requisitos

### Sistema Operacional
- Windows 10/11 (recomendado)
- Linux Ubuntu 20.04+
- macOS 10.15+

### Hardware Mínimo
- **RAM**: 4GB (8GB recomendado)
- **Processamento**: Intel i3 ou AMD equivalente
- **Armazenamento**: 2GB livres
- **USB**: Porta USB 2.0+ para Arduino
- **Áudio**: Microfone funcional

## 🔧 Instalação Passo a Passo

### 1. Preparação do Ambiente

#### Windows
```cmd
# Verificar versão do Python
python --version

# Se não tiver Python, baixar de python.org
# Versão recomendada: 3.11
```

#### Linux (Ubuntu/Debian)
```bash
# Atualizar sistema
sudo apt update && sudo apt upgrade -y

# Instalar Python e dependências
sudo apt install python3 python3-pip python3-venv git

# Instalar dependências de áudio
sudo apt install portaudio19-dev python3-pyaudio
```

#### macOS
```bash
# Instalar Homebrew (se não tiver)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar Python
brew install python

# Instalar dependências de áudio
brew install portaudio
```

### 2. Clonagem e Configuração do Projeto

```bash
# Criar diretório do projeto
mkdir tradutor-libras
cd tradutor-libras

# Criar ambiente virtual
python -m venv venv

# Ativar ambiente virtual
# Windows:
venv\Scripts\activate
# Linux/macOS:
source venv/bin/activate
```

### 3. Instalação das Dependências Python

```bash
# Atualizar pip
pip install --upgrade pip

# Criar arquivo requirements.txt
cat > requirements.txt << EOF
transformers>=4.21.0
torch>=1.12.0
pyserial>=3.5
sounddevice>=0.4.5
numpy>=1.21.0
scipy>=1.9.0
speechrecognition>=3.10.0
matplotlib>=3.5.0
python-dotenv>=0.19.0
EOF

# Instalar dependências
pip install -r requirements.txt
```

### 4. Configuração do Arduino IDE

#### Download e Instalação
1. Baixar Arduino IDE 2.0+ de [arduino.cc](https://www.arduino.cc/en/software)
2. Instalar seguindo o assistente
3. Abrir Arduino IDE

#### Configuração da Board
1. Conectar Arduino via USB
2. Em **Tools > Board**, selecionar "Arduino Uno"
3. Em **Tools > Port**, selecionar porta COM (Windows) ou /dev/ttyUSB (Linux)
4. Testar com sketch básico:

```cpp
void setup() {
  Serial.begin(9600);
  Serial.println("Arduino conectado!");
}

void loop() {
  delay(1000);
}
```

### 5. Teste de Conectividade

#### Teste Python → Arduino

```python
# test_serial.py
import serial
import time

try:
    arduino = serial.Serial('COM3', 9600)  # Ajustar porta
    time.sleep(2)
    
    arduino.write(b'TEST\n')
    response = arduino.readline().decode().strip()
    print(f"Arduino respondeu: {response}")
    
    arduino.close()
    print("✅ Comunicação serial OK!")
    
except Exception as e:
    print(f"❌ Erro na comunicação: {e}")
```

#### Teste de Áudio

```python
# test_audio.py
import sounddevice as sd
import numpy as np

print("Dispositivos de áudio:")
print(sd.query_devices())

print("\nGravando 3 segundos...")
audio = sd.rec(int(3 * 16000), samplerate=16000, channels=1)
sd.wait()
print("✅ Gravação concluída!")

# Reproduzir gravação
print("Reproduzindo...")
sd.play(audio, 16000)
sd.wait()
print("✅ Áudio OK!")
```

### 6. Teste do Modelo de IA

```python
# test_ai.py
from transformers import pipeline
import numpy as np

print("Carregando modelo Whisper...")
asr = pipeline("automatic-speech-recognition", 
               model="openai/whisper-tiny")
print("✅ Modelo carregado!")

# Teste com áudio sintético
sample_rate = 16000
duration = 2
t = np.linspace(0, duration, int(sample_rate * duration))
audio = np.sin(2 * np.pi * 440 * t)  # Tom de 440Hz

try:
    result = asr(audio)
    print(f"Resultado do teste: {result}")
    print("✅ IA funcionando!")
except Exception as e:
    print(f"❌ Erro na IA: {e}")
```

## 🔧 Configuração Avançada

### 1. Arquivo de Configuração

```python
# config.py
import os
from dataclasses import dataclass

@dataclass
class Config:
    # Serial
    ARDUINO_PORT = os.getenv('ARDUINO_PORT', 'COM3')
    BAUD_RATE = int(os.getenv('BAUD_RATE', '9600'))
    
    # Audio
    SAMPLE_RATE = 16000
    CHANNELS = 1
    CHUNK_SIZE = 1024
    
    # IA
    MODEL_NAME = os.getenv('MODEL_NAME', 'openai/whisper-tiny')
    DEVICE = 'cpu'
    
    # Servos (ângulos em graus)
    SERVO_MIN = 0
    SERVO_MAX = 180
    SERVO_DELAY = 0.5  # segundos entre movimentos
```

### 2. Variáveis de Ambiente

```bash
# .env
ARDUINO_PORT=COM3
BAUD_RATE=9600
MODEL_NAME=openai/whisper-tiny
DEBUG=True
```

### 3. Script de Inicialização

```python
# setup.py
import os
import sys
import subprocess

def verificar_dependencias():
    """Verifica se todas as dependências estão instaladas"""
    try:
        import transformers
        import torch
        import serial
        import sounddevice
        print("✅ Todas as dependências estão instaladas")
        return True
    except ImportError as e:
        print(f"❌ Dependência faltando: {e}")
        return False

def detectar_arduino():
    """Detecta automaticamente a porta do Arduino"""
    import serial.tools.list_ports
    
    portas = serial.tools.list_ports.comports()
    for porta in portas:
        if 'Arduino' in porta.description or 'CH340' in porta.description:
            print(f"✅ Arduino encontrado em: {porta.device}")
            return porta.device
    
    print("❌ Arduino não encontrado")
    return None

def main():
    print("🚀 Configuração do Tradutor de Libras")
    print("=" * 40)
    
    # Verificar Python
    if sys.version_info < (3, 10):
        print("❌ Python 3.10+ necessário")
        return
    
    print(f"✅ Python {sys.version}")
    
    # Verificar dependências
    if not verificar_dependencias():
        print("Execute: pip install -r requirements.txt")
        return
    
    # Detectar Arduino
    porta = detectar_arduino()
    if porta:
        # Criar arquivo .env
        with open('.env', 'w') as f:
            f.write(f"ARDUINO_PORT={porta}\n")
            f.write("BAUD_RATE=9600\n")
            f.write("MODEL_NAME=openai/whisper-tiny\n")
        print("✅ Arquivo .env criado")
    
    print("\n🎉 Configuração concluída!")
    print("Execute: python main.py")

if __name__ == "__main__":
    main()
```

## 🐛 Solução de Problemas Comuns

### Erro: "Porta COM não encontrada"

**Windows:**
```cmd
# Listar portas disponíveis
mode
# ou usar Device Manager
```

**Linux:**
```bash
# Listar portas USB
ls /dev/ttyUSB*
ls /dev/ttyACM*

# Dar permissão (se necessário)
sudo usermod -a -G dialout $USER
# Fazer logout/login após este comando
```

### Erro: "Modelo não carrega"

```bash
# Limpar cache
rm -rf ~/.cache/huggingface/

# Reinstalar transformers
pip uninstall transformers
pip install transformers
```

### Erro: "Microfone não funciona"

**Windows:**
- Verificar Configurações > Privacidade > Microfone
- Permitir acesso ao microfone para aplicativos desktop

**Linux:**
```bash
# Instalar PulseAudio
sudo apt install pulseaudio pulseaudio-utils

# Testar microfone
arecord -l
```

### Erro: "Servos não se movem"

1. Verificar alimentação externa (5V/2A)
2. Confirmar GND comum
3. Testar servos individualmente
4. Verificar conexões dos pinos

## ✅ Checklist Final

- [ ] Python 3.10+ instalado
- [ ] Ambiente virtual criado e ativado
- [ ] Dependências Python instaladas
- [ ] Arduino IDE configurado
- [ ] Arduino conectado e reconhecido
- [ ] Comunicação serial funcionando
- [ ] Microfone testado e funcionando
- [ ] Modelo de IA carregado com sucesso
- [ ] Servos conectados e testados
- [ ] Arquivo .env configurado
- [ ] Todos os testes passando

## 🎯 Próximos Passos

Após a instalação completa:

1. Ler o [Manual de Uso](manual-uso.md)
2. Executar o sistema completo
3. Calibrar os servomotores
4. Testar com palavras simples
5. Consultar [Troubleshooting](troubleshooting.md) se necessário