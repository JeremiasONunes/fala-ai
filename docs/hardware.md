# 🧩 Componentes e Hardware

## 🖥️ Lista de Componentes

### Componentes Principais

| Componente | Quantidade | Função | Especificações |
|------------|------------|--------|----------------|
| Arduino Uno R3 ou Nano | 1 | Controlar servos e comunicação serial | ATmega328P, 5V, 16MHz |
| Servomotor SG90 | 5 | Controlar dedos da mão 3D | 180°, 4.8-6V, 2.5kg/cm |
| Protoboard | 1 | Montagem de circuitos | 830 pontos mínimo |
| Jumpers M-M | 20+ | Conexões elétricas | Diversos tamanhos |
| Fonte Externa | 1 | Alimentação dos servos | 5V / 2A mínimo |
| Mão Robótica 3D | 1 | Estrutura mecânica | Impressa em PLA/ABS |

### Componentes Opcionais

| Componente | Função | Observações |
|------------|--------|-------------|
| Capacitor 1000µF | Estabilização de energia | Recomendado para servos |
| Resistor 220Ω | LED indicador | Para feedback visual |
| LED | Indicação de status | Verde/Vermelho |
| Buzzer | Feedback sonoro | Opcional |

## 🔌 Esquema de Conexões

### Pinagem do Arduino

```
Arduino Uno R3:
├── Servo Polegar (Dedo 1)    → Pino Digital 3
├── Servo Indicador (Dedo 2)  → Pino Digital 5  
├── Servo Médio (Dedo 3)      → Pino Digital 6
├── Servo Anelar (Dedo 4)     → Pino Digital 9
├── Servo Mindinho (Dedo 5)   → Pino Digital 10
├── GND                       → GND (Comum)
└── 5V                        → VCC Servos (via fonte externa)
```

### Diagrama de Ligação

```
┌─────────────────┐    ┌──────────────┐    ┌─────────────┐
│   Computador    │    │   Arduino    │    │   Servos    │
│                 │USB │              │PWM │             │
│   Python + API  ├────┤ Uno R3/Nano  ├────┤ SG90 (x5)   │
│                 │    │              │    │             │
└─────────────────┘    └──────┬───────┘    └─────────────┘
                              │
                       ┌──────┴───────┐
                       │ Fonte 5V/2A  │
                       │   Externa    │
                       └──────────────┘
```

## ⚡ Alimentação e Energia

### Cálculo de Consumo
- **Cada Servo SG90**: ~500mA (pico)
- **5 Servos**: 2.5A (pico total)
- **Arduino**: ~50mA
- **Total Estimado**: 2.6A

### Recomendações de Fonte
- **Tensão**: 5V regulada
- **Corrente**: Mínimo 3A (margem de segurança)
- **Tipo**: Fonte chaveada com boa regulação
- **Conectores**: Plug P4 ou bornes

### Esquema de Alimentação
```
Fonte 5V/3A
├── VCC → Arduino VIN (se >7V) ou 5V
├── VCC → Linha positiva da protoboard
└── GND → GND comum (Arduino + Servos)
```

## 🖨️ Mão Robótica 3D

### Especificações de Impressão
- **Material**: PLA (recomendado) ou ABS
- **Resolução**: 0.2mm de camada
- **Preenchimento**: 20-30%
- **Suporte**: Necessário para dedos
- **Tempo**: ~8-12 horas total

### Peças Necessárias
1. **Base da Mão** (1x)
2. **Dedos Articulados** (5x)
3. **Conectores para Servos** (5x)
4. **Parafusos M3** (10x)
5. **Porcas M3** (10x)

### Montagem Mecânica
1. Imprimir todas as peças
2. Remover suportes e fazer acabamento
3. Instalar servos nos conectores
4. Fixar dedos na base
5. Conectar servos aos dedos
6. Testar movimentação manual

## 🔧 Ferramentas Necessárias

### Para Eletrônica
- Ferro de solda (opcional)
- Alicate desencapador
- Multímetro
- Chaves de fenda pequenas

### Para Montagem 3D
- Chaves Allen (2mm, 2.5mm)
- Lima para acabamento
- Furadeira com brocas pequenas
- Parafusos e porcas M3

## ⚠️ Considerações de Segurança

### Elétrica
- Verificar polaridade dos servos
- Não exceder 6V nos servos
- Usar GND comum obrigatório
- Fonte com proteção contra curto

### Mecânica  
- Testar movimentos sem força excessiva
- Verificar folgas nas articulações
- Lubrificar pontos de atrito
- Limitar curso dos servos por software

## 📐 Dimensões e Especificações

### Mão Robótica
- **Comprimento**: ~20cm
- **Largura**: ~8cm  
- **Altura**: ~3cm
- **Peso**: ~300g (com servos)

### Workspace Necessário
- **Área de Trabalho**: 30x30cm mínimo
- **Altura**: 15cm livre
- **Ventilação**: Boa para servos
- **Acesso**: USB para programação