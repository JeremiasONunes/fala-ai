# 📖 Manual de Uso

## 🚀 Iniciando o Sistema

### 1. Preparação Inicial

Antes de usar o sistema, certifique-se de que:

- ✅ Todas as dependências estão instaladas
- ✅ Arduino está conectado e programado
- ✅ Mão robótica está montada e conectada
- ✅ Microfone está funcionando
- ✅ Servos estão calibrados

### 2. Ativação do Ambiente

```bash
# Navegar para o diretório do projeto
cd tradutor-libras

# Ativar ambiente virtual
# Windows:
venv\Scripts\activate
# Linux/macOS:
source venv/bin/activate
```

### 3. Execução do Sistema

```bash
# Executar o sistema principal
python src/python/main.py
```

## 🎤 Usando o Reconhecimento de Voz

### Interface do Sistema

Ao iniciar, você verá:

```
🧠 TRADUTOR DE VOZ PARA LIBRAS
================================
✅ Modelo de IA carregado: whisper-tiny
✅ Arduino conectado em: COM3
✅ Microfone configurado
✅ Sistema pronto!

Pressione ENTER para começar a falar...
```

### Comandos Básicos

| Comando | Ação |
|---------|------|
| `ENTER` | Iniciar gravação de voz |
| `ESC` | Parar gravação |
| `Ctrl+C` | Encerrar sistema |
| `h` | Mostrar ajuda |
| `t` | Testar servos |

### Processo de Tradução

1. **Pressione ENTER** para iniciar
2. **Fale claramente** no microfone (3-5 segundos)
3. **Aguarde** o processamento da IA
4. **Observe** a mão robótica executar os movimentos
5. **Repita** o processo para novas palavras

## 🤖 Controle da Mão Robótica

### Movimentos Suportados

O sistema atualmente suporta o **alfabeto manual da Libras** (A-Z):

| Letra | Descrição do Movimento |
|-------|------------------------|
| A | Punho fechado, polegar para fora |
| B | Dedos estendidos, polegar dobrado |
| C | Dedos curvados formando "C" |
| D | Apenas indicador estendido |
| E | Punho completamente fechado |
| F | Indicador e polegar unidos, outros estendidos |
| G | Indicador apontando horizontalmente |
| H | Indicador e médio estendidos horizontalmente |
| I | Mindinho estendido |
| J | Mindinho fazendo movimento de "J" |
| ... | (continua para todas as letras) |

### Calibração dos Servos

Se os movimentos não estiverem corretos:

1. **Acesse o modo de calibração**:
   ```bash
   python src/python/calibrate.py
   ```

2. **Teste cada servo individualmente**:
   ```
   Calibração de Servos
   ====================
   1. Testar Polegar
   2. Testar Indicador  
   3. Testar Médio
   4. Testar Anelar
   5. Testar Mindinho
   6. Salvar configurações
   ```

3. **Ajuste os ângulos** conforme necessário

## 🔧 Configurações Avançadas

### Arquivo de Configuração

Edite o arquivo `.env` para personalizar:

```bash
# .env
ARDUINO_PORT=COM3          # Porta do Arduino
BAUD_RATE=9600            # Velocidade de comunicação
MODEL_NAME=openai/whisper-tiny  # Modelo de IA
SERVO_DELAY=1.0           # Delay entre movimentos (segundos)
AUDIO_DURATION=3          # Duração da gravação (segundos)
DEBUG=True                # Modo debug
```

### Configurações de Áudio

```python
# config.py - Seção de áudio
SAMPLE_RATE = 16000       # Taxa de amostragem (Hz)
CHANNELS = 1              # Mono (1) ou Estéreo (2)
CHUNK_SIZE = 1024         # Tamanho do buffer
AUDIO_DEVICE = 0          # ID do dispositivo de áudio
```

### Configurações dos Servos

```python
# config.py - Seção de servos
SERVO_PINS = [3, 5, 6, 9, 10]  # Pinos dos servos
SERVO_MIN_ANGLE = 0             # Ângulo mínimo
SERVO_MAX_ANGLE = 180           # Ângulo máximo
MOVEMENT_SPEED = 50             # Velocidade do movimento
```

## 📊 Monitoramento e Logs

### Visualização em Tempo Real

Durante o uso, o sistema mostra:

```
[14:30:15] INFO - Áudio capturado (3.2s)
[14:30:16] INFO - Texto reconhecido: "HELLO"
[14:30:16] INFO - Enviando: H
[14:30:17] INFO - Enviando: E  
[14:30:18] INFO - Enviando: L
[14:30:19] INFO - Enviando: L
[14:30:20] INFO - Enviando: O
[14:30:21] INFO - Sequência concluída
```

### Arquivos de Log

Os logs são salvos em:
- `logs/tradutor_YYYYMMDD_HHMMSS.log`
- `logs/performance.log` (métricas)
- `logs/errors.log` (apenas erros)

### Métricas de Performance

```
Relatório de Performance
========================
Reconhecimento de voz: 1.234s (média)
Comunicação serial: 0.045s (média)
Movimento dos servos: 0.500s (média)
Pipeline completo: 2.156s (média)
```

## 🎯 Dicas de Uso

### Para Melhor Reconhecimento

1. **Fale claramente** e em ritmo normal
2. **Evite ruídos** de fundo
3. **Mantenha distância** de 15-30cm do microfone
4. **Use palavras simples** inicialmente
5. **Aguarde** o processamento completo

### Para Melhor Performance dos Servos

1. **Verifique a alimentação** (5V estável)
2. **Mantenha conexões firmes**
3. **Evite movimentos bruscos**
4. **Calibre regularmente**
5. **Lubrifique** articulações se necessário

### Palavras Recomendadas para Teste

| Categoria | Palavras |
|-----------|----------|
| Simples | "A", "E", "I", "O", "U" |
| Nomes | "ANA", "BOB", "EVA" |
| Cumprimentos | "OI", "OLA" |
| Objetos | "CASA", "BOLA", "GATO" |

## ⚠️ Solução de Problemas Comuns

### Problema: "Não reconhece minha voz"

**Soluções:**
1. Verificar se o microfone está funcionando
2. Ajustar volume do microfone
3. Testar com palavras mais simples
4. Verificar configurações de áudio

### Problema: "Servos não se movem"

**Soluções:**
1. Verificar alimentação externa (5V/2A)
2. Confirmar conexões dos pinos
3. Testar servos individualmente
4. Verificar comunicação serial

### Problema: "Movimentos incorretos"

**Soluções:**
1. Executar calibração dos servos
2. Verificar mapeamento das letras
3. Ajustar ângulos no código
4. Verificar montagem mecânica

### Problema: "Sistema lento"

**Soluções:**
1. Usar modelo de IA mais leve (whisper-tiny)
2. Reduzir duração da gravação
3. Otimizar configurações de áudio
4. Verificar recursos do computador

## 🔄 Manutenção Regular

### Diária
- [ ] Verificar conexões
- [ ] Testar microfone
- [ ] Limpar logs antigos

### Semanal  
- [ ] Calibrar servos
- [ ] Verificar performance
- [ ] Backup das configurações

### Mensal
- [ ] Atualizar dependências
- [ ] Verificar desgaste mecânico
- [ ] Revisar logs de erro

## 📞 Suporte e Ajuda

### Comandos de Diagnóstico

```bash
# Testar sistema completo
python src/python/test_system.py

# Verificar hardware
python src/python/check_hardware.py

# Diagnóstico de áudio
python src/python/test_audio.py

# Teste de comunicação
python src/python/test_serial.py
```

### Informações do Sistema

```bash
# Versões instaladas
python src/python/version_info.py

# Status dos componentes
python src/python/system_status.py
```

### Contato

- **Desenvolvedor**: Jeremias Maya
- **Documentação**: [docs/](../docs/)
- **Issues**: Consulte troubleshooting.md
- **Logs**: Verifique pasta `logs/`

## 🎉 Demonstração e Apresentação

### Roteiro de Demo

1. **Apresentar o sistema** (30s)
2. **Mostrar componentes** (1min)
3. **Demonstrar funcionamento** (2min)
4. **Testar com audiência** (2min)
5. **Explicar tecnologias** (1min)

### Palavras para Demo

Sequência recomendada:
1. "OI" (simples)
2. "CASA" (palavra comum)
3. "LIBRAS" (tema do projeto)
4. Nome da pessoa (personalizado)

### Preparação para Demo

- [ ] Sistema testado e funcionando
- [ ] Backup das configurações
- [ ] Microfone sem fio (opcional)
- [ ] Apresentação preparada
- [ ] Palavras de teste definidas