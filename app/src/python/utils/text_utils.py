#!/usr/bin/env python3
"""
Utilitários para processamento de texto
"""

import unicodedata

def remove_accents(text):
    """Remove acentos do texto"""
    if not text:
        return text
    
    # Normalizar e remover acentos
    normalized = unicodedata.normalize('NFD', text)
    without_accents = ''.join(c for c in normalized if unicodedata.category(c) != 'Mn')
    
    return without_accents.upper()

def clean_text_for_libras(text):
    """Limpa texto para uso com Libras (apenas letras A-Z)"""
    if not text:
        return ""
    
    # Remover acentos
    clean_text = remove_accents(text)
    
    # Manter apenas letras
    letters_only = ''.join(c for c in clean_text if c.isalpha())
    
    return letters_only