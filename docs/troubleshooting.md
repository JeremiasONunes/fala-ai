# 🔧 Troubleshooting

## 🚨 Problemas Críticos

### Sistema Não Inicia

#### Erro: "Python não encontrado"
```bash
'python' is not recognized as an internal or external command
```

**Soluções:**
1. Instalar Python 3.10+ de [python.org](https://python.org)
2. Adicionar Python ao PATH do sistema
3. Usar `python3` em vez de `python` (Linux/macOS)
4. Verificar instalação: `python --version`

#### Erro: "Módulo não encontrado"
```python
ModuleNotFoundError: No module named 'transformers'
```

**Soluções:**
1. Ativar ambiente virtual: `venv\Scripts\activate`
2. Instalar dependências: `pip install -r requirements.txt`
3. Verificar ambiente: `pip list`
4. Reinstalar se necessário: `pip install --force-reinstall transformers`

#### Erro: "Arduino não encontrado"
```
serial.serialutil.SerialException: could not open port 'COM3'
```

**Soluções:**
1. Verificar conexão USB
2. Instalar drivers do Arduino
3. Verificar porta no Device Manager (Windows)
4. Testar com Arduino IDE
5. Usar detecção automática:
   ```python
   import serial.tools.list_ports
   ports = serial.tools.list_ports.comports()
   for port in ports:
       print(f"{port.device}: {port.description}")
   ```

## 🎤 Problemas de Áudio

### Microfone Não Funciona

#### Erro: "Dispositivo de áudio não encontrado"
```python
sounddevice.PortAudioError: Device unavailable
```

**Diagnóstico:**
```python
import sounddevice as sd
print(sd.query_devices())
```

**Soluções Windows:**
1. Verificar Configurações > Privacidade > Microfone
2. Permitir acesso para aplicativos desktop
3. Testar microfone em Configurações > Som
4. Atualizar drivers de áudio
5. Reiniciar serviço de áudio:
   ```cmd
   net stop audiosrv
   net start audiosrv
   ```

**Soluções Linux:**
```bash
# Verificar dispositivos
arecord -l

# Instalar PulseAudio
sudo apt install pulseaudio pulseaudio-utils

# Reiniciar PulseAudio
pulseaudio -k
pulseaudio --start

# Testar gravação
arecord -d 3 -f cd test.wav
aplay test.wav
```

**Soluções macOS:**
1. Verificar Preferências > Segurança > Privacidade > Microfone
2. Permitir acesso para Terminal/IDE
3. Testar com aplicativo nativo

### Qualidade de Áudio Ruim

#### Sintomas:
- Reconhecimento impreciso
- Ruído excessivo
- Volume baixo

**Soluções:**
1. **Ajustar configurações de áudio:**
   ```python
   # config.py
   SAMPLE_RATE = 16000  # Padrão Whisper
   CHANNELS = 1         # Mono
   AUDIO_DURATION = 5   # Aumentar duração
   ```

2. **Filtrar ruído:**
   ```python
   import numpy as np
   from scipy import signal
   
   def filter_audio(audio, sample_rate):
       # Filtro passa-alta para remover ruído baixo
       sos = signal.butter(5, 300, 'hp', fs=sample_rate, output='sos')
       return signal.sosfilt(sos, audio)
   ```

3. **Normalizar volume:**
   ```python
   def normalize_audio(audio):
       return audio / np.max(np.abs(audio))
   ```

## 🤖 Problemas de IA

### Modelo Não Carrega

#### Erro: "Out of memory"
```python
torch.cuda.OutOfMemoryError: CUDA out of memory
```

**Soluções:**
1. Usar modelo menor:
   ```python
   # Em vez de whisper-base, usar:
   model = "openai/whisper-tiny"
   ```

2. Forçar CPU:
   ```python
   asr = pipeline("automatic-speech-recognition", 
                  model="openai/whisper-tiny",
                  device="cpu")
   ```

3. Limpar cache:
   ```bash
   # Windows
   rmdir /s %USERPROFILE%\.cache\huggingface
   
   # Linux/macOS
   rm -rf ~/.cache/huggingface/
   ```

### Reconhecimento Impreciso

#### Sintomas:
- Palavras erradas
- Texto em inglês em vez de português
- Caracteres especiais

**Soluções:**
1. **Usar modelo em português:**
   ```python
   # Modelo específico para português (se disponível)
   model = "jonatasgrosman/wav2vec2-large-xlsr-53-portuguese"
   ```

2. **Pós-processamento:**
   ```python
   import re
   
   def clean_text(text):
       # Remover caracteres especiais
       text = re.sub(r'[^A-Za-z\s]', '', text)
       # Converter para maiúsculas
       text = text.upper()
       # Remover espaços extras
       text = ' '.join(text.split())
       return text
   ```

3. **Configurar idioma:**
   ```python
   # Para SpeechRecognition (alternativo)
   import speech_recognition as sr
   r = sr.Recognizer()
   
   with sr.Microphone() as source:
       audio = r.listen(source)
   
   text = r.recognize_google(audio, language='pt-BR')
   ```

## ⚡ Problemas de Hardware

### Servos Não Se Movem

#### Diagnóstico Básico:
```cpp
// Código de teste Arduino
#include <Servo.h>

Servo teste;

void setup() {
    Serial.begin(9600);
    teste.attach(9);
    Serial.println("Testando servo no pino 9");
}

void loop() {
    teste.write(0);
    delay(1000);
    teste.write(90);
    delay(1000);
    teste.write(180);
    delay(1000);
}
```

#### Problemas de Alimentação:

**Sintomas:**
- Servos tremem
- Movimentos erráticos
- Arduino reinicia

**Soluções:**
1. **Verificar fonte externa:**
   - Tensão: 5V ±0.2V
   - Corrente: Mínimo 3A
   - GND comum obrigatório

2. **Adicionar capacitor:**
   ```
   Fonte 5V ----[Capacitor 1000µF]---- VCC Servos
                      |
                     GND
   ```

3. **Verificar conexões:**
   ```
   Arduino GND ---- Fonte GND ---- Servo GND (todos conectados)
   ```

### Comunicação Serial Instável

#### Sintomas:
- Comandos perdidos
- Caracteres corrompidos
- Timeout frequente

**Diagnóstico:**
```python
# test_serial.py
import serial
import time

arduino = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)

for i in range(10):
    comando = f"TEST{i}\n"
    arduino.write(comando.encode())
    
    resposta = arduino.readline().decode().strip()
    print(f"Enviado: {comando.strip()} | Recebido: {resposta}")
    
    time.sleep(0.5)

arduino.close()
```

**Soluções:**
1. **Configurar timeout:**
   ```python
   arduino = serial.Serial('COM3', 9600, timeout=2)
   ```

2. **Adicionar flush:**
   ```python
   arduino.write(comando.encode())
   arduino.flush()  # Garantir envio
   ```

3. **Verificar cabo USB:**
   - Usar cabo de dados (não apenas carregamento)
   - Testar cabo diferente
   - Verificar conexões

## 🔧 Problemas Mecânicos

### Mão Robótica Travada

#### Sintomas:
- Dedos não se movem
- Movimentos limitados
- Ruído excessivo

**Soluções:**
1. **Verificar montagem:**
   - Parafusos muito apertados
   - Peças mal alinhadas
   - Falta de lubrificação

2. **Calibrar limites:**
   ```cpp
   // Limitar curso dos servos
   void moverServo(int servo, int angulo) {
       angulo = constrain(angulo, 10, 170);  // Evitar extremos
       servos[servo].write(angulo);
   }
   ```

3. **Testar individualmente:**
   ```cpp
   // Testar cada servo separadamente
   void testarServo(int pin) {
       Servo teste;
       teste.attach(pin);
       
       for(int pos = 0; pos <= 180; pos += 10) {
           teste.write(pos);
           delay(500);
       }
   }
   ```

## 📊 Ferramentas de Diagnóstico

### Script de Diagnóstico Completo

```python
#!/usr/bin/env python3
"""
Script de diagnóstico completo do sistema
"""

import sys
import importlib
import serial.tools.list_ports
import sounddevice as sd

def verificar_python():
    """Verifica versão do Python"""
    version = sys.version_info
    if version.major == 3 and version.minor >= 10:
        print(f"✅ Python {version.major}.{version.minor}.{version.micro}")
        return True
    else:
        print(f"❌ Python {version.major}.{version.minor} (necessário 3.10+)")
        return False

def verificar_dependencias():
    """Verifica se todas as dependências estão instaladas"""
    dependencias = [
        'transformers', 'torch', 'serial', 
        'sounddevice', 'numpy', 'scipy'
    ]
    
    for dep in dependencias:
        try:
            importlib.import_module(dep)
            print(f"✅ {dep}")
        except ImportError:
            print(f"❌ {dep} não encontrado")
            return False
    
    return True

def verificar_arduino():
    """Verifica conexão com Arduino"""
    portas = serial.tools.list_ports.comports()
    arduino_encontrado = False
    
    print("\nPortas seriais disponíveis:")
    for porta in portas:
        print(f"  {porta.device}: {porta.description}")
        if 'Arduino' in porta.description or 'CH340' in porta.description:
            arduino_encontrado = True
            print(f"✅ Arduino encontrado em {porta.device}")
    
    if not arduino_encontrado:
        print("❌ Arduino não encontrado")
    
    return arduino_encontrado

def verificar_audio():
    """Verifica dispositivos de áudio"""
    try:
        devices = sd.query_devices()
        print(f"\n✅ {len(devices)} dispositivos de áudio encontrados")
        
        # Encontrar dispositivo de entrada padrão
        default_input = sd.default.device[0]
        if default_input is not None:
            print(f"✅ Dispositivo de entrada padrão: {devices[default_input]['name']}")
            return True
        else:
            print("❌ Nenhum dispositivo de entrada encontrado")
            return False
            
    except Exception as e:
        print(f"❌ Erro ao verificar áudio: {e}")
        return False

def main():
    print("🔍 DIAGNÓSTICO DO SISTEMA")
    print("=" * 40)
    
    resultados = []
    
    # Verificações
    resultados.append(verificar_python())
    resultados.append(verificar_dependencias())
    resultados.append(verificar_arduino())
    resultados.append(verificar_audio())
    
    # Resultado final
    print("\n" + "=" * 40)
    if all(resultados):
        print("🎉 Sistema pronto para uso!")
    else:
        print("⚠️  Problemas encontrados - verifique os itens marcados com ❌")

if __name__ == "__main__":
    main()
```

### Monitor de Performance

```python
# monitor.py
import time
import psutil
import threading
from datetime import datetime

class SystemMonitor:
    def __init__(self):
        self.running = False
        self.stats = {
            'cpu': [],
            'memory': [],
            'serial_errors': 0,
            'audio_errors': 0
        }
    
    def start_monitoring(self):
        """Inicia monitoramento em thread separada"""
        self.running = True
        thread = threading.Thread(target=self._monitor_loop)
        thread.daemon = True
        thread.start()
    
    def _monitor_loop(self):
        """Loop principal de monitoramento"""
        while self.running:
            # CPU e memória
            cpu = psutil.cpu_percent()
            memory = psutil.virtual_memory().percent
            
            self.stats['cpu'].append(cpu)
            self.stats['memory'].append(memory)
            
            # Manter apenas últimos 60 valores (1 minuto)
            if len(self.stats['cpu']) > 60:
                self.stats['cpu'].pop(0)
                self.stats['memory'].pop(0)
            
            time.sleep(1)
    
    def get_report(self):
        """Gera relatório de performance"""
        if not self.stats['cpu']:
            return "Nenhum dado coletado"
        
        avg_cpu = sum(self.stats['cpu']) / len(self.stats['cpu'])
        avg_memory = sum(self.stats['memory']) / len(self.stats['memory'])
        
        return f"""
Relatório de Performance ({datetime.now().strftime('%H:%M:%S')})
================================================
CPU média: {avg_cpu:.1f}%
Memória média: {avg_memory:.1f}%
Erros seriais: {self.stats['serial_errors']}
Erros de áudio: {self.stats['audio_errors']}
        """
    
    def stop(self):
        """Para o monitoramento"""
        self.running = False
```

## 📞 Suporte Adicional

### Logs Detalhados

Para ativar logs detalhados:

```python
import logging

# Configurar nível DEBUG
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('debug.log'),
        logging.StreamHandler()
    ]
)
```

### Contatos e Recursos

- **Documentação**: Consulte outros arquivos em `docs/`
- **Código fonte**: Verifique `src/` para implementações
- **Logs**: Analise arquivos em `logs/`
- **Testes**: Execute scripts em `tests/`

### Reportar Problemas

Ao reportar problemas, inclua:

1. **Versão do sistema operacional**
2. **Versão do Python** (`python --version`)
3. **Log completo do erro**
4. **Passos para reproduzir**
5. **Hardware utilizado**
6. **Configurações modificadas**