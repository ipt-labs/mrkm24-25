import time
import random
import string
from Crypto.Cipher import ChaCha20
from Crypto.Random import get_random_bytes


def generate_random_strings(num_strings=1000000):
    random_strings = [
        ''.join(random.choices(string.ascii_letters + string.digits, k=500))
        for _ in range(num_strings)
    ]
    return random_strings


def measure_encryption_decryption_time(strings):
    key = get_random_bytes(32)
    total_time = 0

    for plaintext in strings:
        plaintext_bytes = plaintext.encode('utf-8')

        start_time = time.time()

        cipher = ChaCha20.new(key=key)
        ciphertext = cipher.encrypt(plaintext_bytes)
        nonce = cipher.nonce

        cipher = ChaCha20.new(key=key, nonce=nonce)
        decrypted_text = cipher.decrypt(ciphertext)

        end_time = time.time()

        assert decrypted_text.decode('utf-8') == plaintext, "Decryption failed"

        total_time += (end_time - start_time)

    return total_time

random_strings = generate_random_strings()

total_time = measure_encryption_decryption_time(random_strings)
print(f"Total time taken for encryption and decryption of {len(random_strings)} strings: {total_time:.6f} seconds")
