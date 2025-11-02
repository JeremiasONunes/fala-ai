#!/usr/bin/env python3
"""
Módulo de reconhecimento de voz usando SpeechRecognition
"""

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
                
                self.logger.info(f" Fale algo (você tem {self.config.timeout}s)...")
                audio = self.recognizer.listen(
                    source, 
                    timeout=self.config.timeout,
                    phrase_time_limit=self.config.phrase_time_limit
                )
            
            self.logger.info(" Processando...")
            texto = self.recognizer.recognize_google(
                audio, 
                language=self.config.speech_language
            ).upper().strip()
            
            self.logger.info(f" Texto reconhecido: '{texto}'")
            return texto
            
        except sr.WaitTimeoutError:
            self.logger.warning(" Timeout - nenhum áudio detectado")
            return None
        except sr.UnknownValueError:
            self.logger.warning(" Não foi possível entender o áudio")
            return None
        except sr.RequestError as e:
            self.logger.error(f" Erro na API: {e}")
            return None