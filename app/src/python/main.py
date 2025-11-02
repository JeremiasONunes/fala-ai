#!/usr/bin/env python3
"""
TRADUTOR DE VOZ PARA LIBRAS v1.0.0
Sistema principal de integração
Desenvolvido por: Jeremias Maya
"""

import sys
import os
import logging

# Adicionar path dos módulos
sys.path.append(os.path.join(os.path.dirname(__file__)))

from config_manager import Config
from modules.speech_recognition_module import SpeechRecognizer
from modules.serial_communication import ArduinoController
from utils.logger import setup_logger
from utils.text_utils import clean_text_for_libras

class LibrasTranslator:
    def __init__(self):
        self.config = Config()
        self.logger = setup_logger(
            logging.DEBUG if self.config.debug else logging.INFO
        )
        self.speech = None
        self.arduino = None
        
    def initialize(self):
        """Inicializa todos os componentes"""
        self.logger.info(" TRADUTOR DE VOZ PARA LIBRAS v1.0.0")
        self.logger.info("=" * 50)
        
        try:
            # Inicializar reconhecimento de voz
            self.logger.info("Inicializando reconhecimento de voz...")
            self.speech = SpeechRecognizer(self.config)
            
            # Inicializar Arduino
            self.logger.info("Conectando ao Arduino...")
            self.arduino = ArduinoController(self.config)
            
            # Testar conexão
            if self.arduino.test_connection():
                self.logger.info(" Sistema inicializado com sucesso!")
                return True
            else:
                self.logger.error(" Falha no teste de conexão")
                return False
                
        except Exception as e:
            self.logger.error(f" Erro na inicialização: {e}")
            return False
    
    def run(self):
        """Loop principal do sistema"""
        if not self.initialize():
            return
            
        self.logger.info("\n Sistema pronto! Comandos:")
        self.logger.info("- Pressione ENTER para falar")
        self.logger.info("- Digite 'q' para sair")
        self.logger.info("- Digite 'test' para testar Arduino")
        
        try:
            while True:
                print("\n" + "="*30)
                choice = input("Comando: ").strip().lower()
                
                if choice == 'q':
                    break
                elif choice == 'test':
                    self._test_arduino()
                elif choice == '' or choice == 'falar':
                    self._process_speech()
                else:
                    print("Comando inválido")
                    
        except KeyboardInterrupt:
            self.logger.info("\n Sistema encerrado pelo usuário")
        finally:
            self._cleanup()
    
    def _process_speech(self):
        """Processa reconhecimento de voz"""
        texto = self.speech.listen_and_convert()
        
        if texto:
            # Limpar texto (remover acentos e manter apenas letras)
            texto_limpo = clean_text_for_libras(texto)
            
            print(f"\nTexto original: {texto}")
            print(f"Texto limpo: {texto_limpo}")
            print(f"Letras detectadas: {list(texto_limpo)}")
            
            if not texto_limpo:
                print("Nenhuma letra valida encontrada")
                return
            
            confirm = input("Executar movimentos? (s/N): ").strip().lower()
            if confirm == 's':
                self.arduino.send_text(texto_limpo)
                print("Movimentos executados!")
            else:
                print("Movimentos cancelados")
        else:
            print("Nenhum texto reconhecido")
    
    def _test_arduino(self):
        """Testa conexão com Arduino"""
        print(" Testando Arduino...")
        if self.arduino.test_connection():
            print(" Arduino funcionando!")
        else:
            print(" Problema com Arduino")
    
    def _cleanup(self):
        """Limpeza ao encerrar"""
        if self.arduino:
            self.arduino.close()
        self.logger.info(" Sistema encerrado")

def main():
    """Função principal"""
    translator = LibrasTranslator()
    translator.run()

if __name__ == "__main__":
    main()