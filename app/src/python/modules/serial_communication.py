#!/usr/bin/env python3
"""
Módulo de comunicação serial com Arduino
"""

import serial
import time
import logging
from serial.tools import list_ports

class ArduinoController:
    def __init__(self, config):
        self.config = config
        self.logger = logging.getLogger(__name__)
        self.connection = None
        self._connect()
        
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
            port = self.config.arduino_port
            
            # Tentar detectar automaticamente se porta não especificada
            if not port or port == 'AUTO':
                port = self._find_arduino_port()
                
            if not port:
                raise Exception("Arduino não encontrado")
                
            self.connection = serial.Serial(
                port=port,
                baudrate=self.config.baud_rate,
                timeout=1
            )
            time.sleep(2)  # Aguardar inicialização
            
            self.logger.info(f"Conectado ao Arduino em {port}")
            
        except Exception as e:
            self.logger.error(f"Erro na conexao: {e}")
            raise
    
    def send_text(self, texto):
        """Envia texto letra por letra para o Arduino"""
        if not self.connection:
            self.logger.error("Conexao nao estabelecida")
            return
            
        self.logger.info(f"Enviando texto: {texto}")
        
        try:
            for letra in texto:
                if letra.isalpha():
                    self.send_letter(letra)
                    time.sleep(self.config.servo_delay)
            
            # Reset final apenas
            self.reset_servos()
            
        except Exception as e:
            self.logger.error(f"Erro ao enviar texto: {e}")
            # Tentar reconectar
            try:
                self.connection.close()
                time.sleep(1)
                self._connect()
            except:
                pass
    
    def reset_servos(self):
        """Retorna todos os servos para posição 0"""
        try:
            if not self.connection or not self.connection.is_open:
                self.logger.warning("Conexao perdida, tentando reconectar...")
                self._connect()
            
            self.logger.info("Retornando servos para posicao 0...")
            command = "RESET\n"
            self.connection.write(command.encode())
            self.connection.flush()
            
            response = self.connection.readline().decode().strip()
            if response:
                self.logger.debug(f"Arduino: {response}")
                
        except Exception as e:
            self.logger.error(f"Erro ao resetar servos: {e}")
            # Não falhar o sistema por erro de reset
    
    def reset_servos_between_letters(self):
        """Reset rápido entre letras"""
        try:
            if self.connection and self.connection.is_open:
                command = "RESET\n"
                self.connection.write(command.encode())
                self.connection.flush()
                time.sleep(0.5)  # Pausa para reset
        except Exception as e:
            self.logger.error(f"Erro no reset entre letras: {e}")
            # Continuar mesmo com erro de reset
    
    def send_letter(self, letra):
        """Envia uma letra específica"""
        try:
            command = f"{letra}\n"
            self.connection.write(command.encode())
            self.connection.flush()
            self.logger.debug(f"Enviado: {letra}")
            
            # Aguardar confirmação do Arduino
            response = self.connection.readline().decode().strip()
            if response:
                self.logger.debug(f"Arduino: {response}")
                
        except Exception as e:
            self.logger.error(f"Erro ao enviar {letra}: {e}")
    
    def test_connection(self):
        """Testa a conexão com Arduino"""
        try:
            # Apenas verifica se a conexão está ativa
            return self.connection and self.connection.is_open
        except:
            return False
    
    def close(self):
        """Fecha a conexão serial"""
        if self.connection and self.connection.is_open:
            self.connection.close()
            self.logger.info("Conexao serial fechada")