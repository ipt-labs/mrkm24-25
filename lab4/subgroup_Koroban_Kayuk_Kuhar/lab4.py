from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256

# Симуляція атаки brute force
def brute_force_simulation(message, key):
    print("\n[СИМУЛЯЦІЯ АТАКИ ПІДБОРУ]")
    
    for _ in range(5):
        test_key = RSA.generate(2048)
        
        try:
            cipher = PKCS1_OAEP.new(test_key.publickey())
            encrypted_test = cipher.encrypt(message)
            
            decipher = PKCS1_OAEP.new(test_key)
            decrypted_test = decipher.decrypt(encrypted_test)
            
            print("Спроба підбору не вдалася")
        except Exception:
            print("Спроба підбору не вдалася")


# Генерація ключей
key = RSA.generate(2048)
private_key = key.export_key()
public_key = key.publickey().export_key()
"""
print("Закритий ключ:")
print(private_key.decode()[:100] + "...")  
print("\nВідкритий ключ:")
print(public_key.decode())
"""

message = b"super secret information for Lab/super secret information for Lab"
print("початкове повідомлення: ", message)
# Шифрування
cipher = PKCS1_OAEP.new(key.publickey())
encrypted_message = cipher.encrypt(message)
print("\nЗашифроване повідомлення:")
print(encrypted_message)

# Дешифрування
decipher = PKCS1_OAEP.new(key)
decrypted_message = decipher.decrypt(encrypted_message)
print("\nРазшифроване повідомлення:")
print(decrypted_message.decode())

# Створення цифрового підпису
hash_obj = SHA256.new(message)
signature = pkcs1_15.new(key).sign(hash_obj)
print("\nЦифровий підпис:")
print(signature.hex())

# Перевірка цифрового підпису
try:
    pkcs1_15.new(key.publickey()).verify(hash_obj, signature)
    print("Підпис дійсний!")
except (ValueError, TypeError):
    print("Підпис не дійсний!")


brute_force_simulation(message, key)