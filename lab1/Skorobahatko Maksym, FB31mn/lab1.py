import time
import os
import psutil
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from OpenSSL import crypto

def memory_usage():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / 1024

# AES encryption & decryption via PyCryptodome
def test_pycryptodome():
    key = get_random_bytes(16)
    cipher = AES.new(key, AES.MODE_EAX)
    plaintext = b'This is a test message.'
    
    start_time = time.time()
    ciphertext, tag = cipher.encrypt_and_digest(plaintext)
    encryption_time = time.time() - start_time
    
    start_time = time.time()
    cipher = AES.new(key, AES.MODE_EAX, nonce=cipher.nonce)
    decrypted = cipher.decrypt_and_verify(ciphertext, tag)
    decryption_time = time.time() - start_time
    
    return encryption_time, decryption_time, memory_usage()

# AES encryption & decryption via Cryptography
def test_cryptography():
    key = get_random_bytes(16)
    cipher = Cipher(algorithms.AES(key), modes.EAX())
    plaintext = b'This is a test message.'
    
    start_time = time.time()
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()
    encryption_time = time.time() - start_time
    
    start_time = time.time()
    decryptor = cipher.decryptor()
    decrypted = decryptor.update(ciphertext) + decryptor.finalize()
    decryption_time = time.time() - start_time
    
    return encryption_time, decryption_time, memory_usage()

# AES encryption & decryption via OpenSSL
def test_openssl():
    key = get_random_bytes(16)
    plaintext = b'This is a test message.'
    
    start_time = time.time()
    cipher = crypto.Cipher(crypto.AES, key, crypto.MODE_EAX)
    ciphertext = cipher.update(plaintext) + cipher.finalize()
    encryption_time = time.time() - start_time
    
    start_time = time.time()
    cipher = crypto.Cipher(crypto.AES, key, crypto.MODE_EAX)
    decrypted = cipher.update(ciphertext) + cipher.finalize()
    decryption_time = time.time() - start_time
    
    return encryption_time, decryption_time, memory_usage()

def main():
    print("PyCryptodome test ...")
    pd_enc_time, pd_dec_time, pd_mem = test_pycryptodome()
    print(f"PyCryptodome - Encryption Time: {pd_enc_time:.6f}s, Decryption Time: {pd_dec_time:.6f}s, Memory Usage: {pd_mem:.2f} KB")

    print("Cryptography test ...")
    c_enc_time, c_dec_time, c_mem = test_cryptography()
    print(f"Cryptography - Encryption Time: {c_enc_time:.6f}s, Decryption Time: {c_dec_time:.6f}s, Memory Usage: {c_mem:.2f} KB")

    print("OpenSSL test ...")
    o_enc_time, o_dec_time, o_mem = test_openssl()
    print(f"OpenSSL - Encryption Time: {o_enc_time:.6f}s, Decryption Time: {o_dec_time:.6f}s, Memory Usage: {o_mem:.2f} KB")

if __name__ == "__main__":
    main()