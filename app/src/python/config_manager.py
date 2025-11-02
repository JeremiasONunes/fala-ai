#!/usr/bin/env python3
"""
Gerenciamento de configurações do sistema
"""

import os
from dataclasses import dataclass

@dataclass
class Config:
    # Serial
    arduino_port: str = os.getenv('ARDUINO_PORT', 'COM5')
    baud_rate: int = int(os.getenv('BAUD_RATE', '9600'))
    
    # Speech Recognition
    speech_language: str = os.getenv('SPEECH_LANGUAGE', 'pt-BR')
    timeout: int = 20
    phrase_time_limit: int = 20
    
    # Servos
    servo_delay: float = 1.0  # segundos entre movimentos
    
    # Debug
    debug: bool = os.getenv('DEBUG', 'False').lower() == 'true'