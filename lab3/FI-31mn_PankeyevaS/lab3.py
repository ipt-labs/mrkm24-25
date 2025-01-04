# Комірка 1: Імпорти
import os
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.asymmetric.dh import DHPrivateKey, DHPublicKey
from cryptography.hazmat.primitives.kdf.hkdf import HKDF

# Комірка 2: Визначення класу ElGamalCrypto
class ElGamalCrypto:
    @staticmethod
    def text_to_number(text: str) -> int:
        return int.from_bytes(text.encode('utf-8'), 'big')

    @staticmethod
    def number_to_text(number: int) -> str:
        try:
            return number.to_bytes((number.bit_length() + 7) // 8, 'big').decode('utf-8')
        except UnicodeDecodeError:
            return '[Помилка дешифрування: неможливо декодувати дані]'

    @staticmethod
    def create_key_pair() -> tuple[DHPrivateKey, DHPublicKey]:
        params = dh.generate_parameters(generator=2, key_size=512)
        priv_key = params.generate_private_key()
        pub_key = priv_key.public_key()
        return priv_key, pub_key

    @staticmethod
    def encode_message(pub_key: DHPublicKey, plaintext: str) -> tuple[int, int]:
        params = pub_key.parameters()
        temp_private = params.generate_private_key()
        temp_public = temp_private.public_key()
        
        shared_secret = temp_private.exchange(pub_key)
        derived_secret = HKDF(
            algorithm=hashes.SHA256(),
            length=32,
            salt=None,
            info=b'elgamal-secure-encryption',
        ).derive(shared_secret)
        
        message_num = ElGamalCrypto.text_to_number(plaintext)
        encrypted = (message_num * int.from_bytes(derived_secret, 'big')) % params.parameter_numbers().p
        
        return temp_public.public_numbers().y, encrypted

    @staticmethod
    def decode_message(priv_key: DHPrivateKey, temp_key: int, encrypted: int) -> str:
        params = priv_key.parameters()
        temp_public = dh.DHPublicNumbers(temp_key, params.parameter_numbers())
        reconstructed_key = temp_public.public_key()
        
        shared_secret = priv_key.exchange(reconstructed_key)
        derived_secret = HKDF(
            algorithm=hashes.SHA256(),
            length=32,
            salt=None,
            info=b'elgamal-secure-encryption',
        ).derive(shared_secret)
        
        secret_num = int.from_bytes(derived_secret, 'big')
        modulus = params.parameter_numbers().p
        decrypted_num = (encrypted * pow(secret_num, -1, modulus)) % modulus
        
        return ElGamalCrypto.number_to_text(decrypted_num)

    @staticmethod
    def create_signature(priv_key: DHPrivateKey, message: str) -> tuple[int, int]:
        params = priv_key.parameters()
        prime = params.parameter_numbers().p
        subgroup = (prime - 1) // 2
        
        while True:
            random_k = int.from_bytes(os.urandom(32), 'big') % subgroup
            if random_k not in (0, (prime - 1) % random_k):
                break
        
        signature_r = pow(2, random_k, prime)
        k_inverse = pow(random_k, -1, subgroup)
        message_num = ElGamalCrypto.text_to_number(message)
        signature_s = (k_inverse * (message_num - priv_key.private_numbers().x * signature_r)) % subgroup
        
        return signature_r, signature_s

    @staticmethod
    def verify_signature(pub_key: DHPublicKey, message: str, signature: tuple[int, int]) -> bool:
        sig_r, sig_s = signature
        params = pub_key.parameters()
        prime = params.parameter_numbers().p
        subgroup = (prime - 1) // 2

        if not (0 < sig_r < prime) or not (0 < sig_s < subgroup):
            return False

        message_num = ElGamalCrypto.text_to_number(message)
        verify1 = (pow(pub_key.public_numbers().y, sig_r, prime) * pow(sig_r, sig_s, prime)) % prime
        verify2 = pow(2, message_num, prime)

        return verify1 == verify2
		
# Комірка 3: Тестування системи
# Генерація ключів
private_key, public_key = ElGamalCrypto.create_key_pair()

# Повідомлення для тестування
test_message = "Привіт, це тестове повідомлення!"
print(f"Оригінальне повідомлення: {test_message}\n")

# Шифрування
temp_y, encrypted_data = ElGamalCrypto.encode_message(public_key, test_message)
print(f"Зашифровані дані:")
print(f"Тимчасовий ключ: {temp_y}")
print(f"Зашифроване повідомлення: {encrypted_data}\n")

# Дешифрування
decrypted_message = ElGamalCrypto.decode_message(private_key, temp_y, encrypted_data)
print(f"Розшифроване повідомлення: {decrypted_message}\n")

# Створення та перевірка цифрового підпису
signature = ElGamalCrypto.create_signature(private_key, test_message)
print(f"Цифровий підпис: {signature}\n")

is_valid = ElGamalCrypto.verify_signature(public_key, test_message, signature)
print(f"Перевірка підпису: {'Успішно' if is_valid else 'Помилка'}")
		