from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
import random
import math
from decimal import Decimal, getcontext

def digital_signature_attacks(key, message):
    print("\n[АТАКИ НА ЦИФРОВИЙ ПІДПИС]")
    
    # Створення підпису
    hash_obj = SHA256.new(message)
    original_signature = pkcs1_15.new(key).sign(hash_obj)
    
    # Атака на цілісність повідомлення
    def message_tampering_attack():
        print("\n1. Атака на цілісність повідомлення:")
        tampered_message = message + b" additional information for lab"
        
        try:
            # Перевірка, чи підпис валідний для зміненого повідомлення
            hash_tampered = SHA256.new(tampered_message)
            pkcs1_15.new(key.publickey()).verify(hash_tampered, original_signature)
            print("ВРАЗЛИВІСТЬ: Підпис валідний для зміненого повідомлення!")
        except (ValueError, TypeError):
            print("Система захищена від атаки підміни повідомлення")
    
    # Демонстрація складності підробки підпису
    def signature_forgery_attack():
        print("\n2. Аналіз складності підробки підпису:")
        
        key_size = key.size_in_bits()
        print(f"Розмір ключа: {key_size} біт")
        
        #кількість комбінацій
        possible_combinations = 2 ** key_size
        print(f"Теоретична кількість можливих комбінацій: {possible_combinations}")
        
        # Оцінка часу підбору
        getcontext().prec = 500 
        possible_combinations_decimal = Decimal(possible_combinations)
        estimated_time_seconds = possible_combinations_decimal / Decimal(10**12)
        if estimated_time_seconds > Decimal(10**9):
            estimated_time_years = estimated_time_seconds.ln() / Decimal(60 * 60 * 24 * 365).ln()
            print(f"Приблизний час підробки підпису: {estimated_time_years:.1f} років")
        else:
            years = estimated_time_seconds / (365 * 24 * 60 * 60)
            print(f"Приблизний час підробки підпису: {years:.1f} років")
            
        print("Висновок: Пряма підробка підпису практично неможлива")
    # Виклик функцій атак
    message_tampering_attack()
    signature_forgery_attack()

# Генерація ключа
key = RSA.generate(2048)
message = b"Info for lab"

# Запуск аналізу
digital_signature_attacks(key, message)