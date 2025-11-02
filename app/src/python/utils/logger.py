#!/usr/bin/env python3
"""
Sistema de logging do projeto
"""

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