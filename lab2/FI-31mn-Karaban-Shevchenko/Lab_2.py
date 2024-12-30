import math
import os
import time

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa


def generate_random_bytes(size):
    """Generate random bytes using a cryptography library and measure the time taken.

    Args:
        size (int): The number of random bytes to generate.

    Returns:
        tuple: Random bytes and time taken to generate them.
    """
    start_time = time.time()
    random_data = os.urandom(size)
    elapsed_time = time.time() - start_time
    print(f'Generated Random Bytes: {random_data.hex()[:128]}... (first 128 hex characters)')

    return random_data, elapsed_time


def calculate_entropy(data):
    """Calculate the Shannon entropy of the given byte data.

    Args:
        data (bytes): The byte data to analyze.

    Returns:
        float: The entropy of the byte data in bits per byte.
    """
    probabilities = [data.count(byte) / len(data) for byte in set(data)]

    # Shannon's entropy formula: -sum(p * log2(p) for each probability p)
    return -sum(p * math.log2(p) for p in probabilities)


def generate_rsa_keypair(key_size=2048):
    """Generate an RSA key pair using a cryptography library and measure the time taken.

    Args:
        key_size (int): The size of the RSA key in bits.

    Returns:
        tuple: The RSA key pair and time taken to generate it.
    """
    start_time = time.time()
    key = rsa.generate_private_key(public_exponent=65537, key_size=key_size, backend=default_backend())
    elapsed_time = time.time() - start_time
    print(f'Generated RSA Key Pair ({key_size}-bit)')

    return key, elapsed_time


def compare_random_and_rsa(size, key_size):
    """Compare random byte generation and RSA key generation in terms of time and entropy.

    Args:
        size (int): The number of bytes for random data generation.
        key_size (int): The size of the RSA key to generate (in bits).
    """
    # Random byte generation test
    print(f"\n{'=' * 40}\nTesting Random Byte Generation ({size} bytes):")
    random_bytes, random_time = generate_random_bytes(size)
    random_entropy = calculate_entropy(random_bytes)
    print(f'• Generation Time: {random_time:.6f} seconds')
    print(f'• Entropy: {random_entropy:.6f} bits per byte')
    print(str('-' * 40))

    # RSA key generation test
    print(f"\n{'=' * 40}\nTesting RSA Key Generation ({key_size}-bit):")
    rsa_key, rsa_time = generate_rsa_keypair(key_size)
    rsa_public_key = rsa_key.public_key().public_bytes(
        encoding=serialization.Encoding.PEM, format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    rsa_key_entropy = calculate_entropy(rsa_public_key)

    print(f'• Generation Time: {rsa_time:.6f} seconds')
    print(f'• Public Key Length: {len(rsa_public_key)} bytes')
    print(f'• Public Key Entropy: {rsa_key_entropy:.6f} bits per byte')
    print(str('-' * 40))

    # Final summary and comparison
    print(f"\n{'=' * 40}\nSummary Comparison:")
    print('Random Byte Generation:')
    print(f'  - Time: {random_time:.6f} seconds')
    print(f'  - Entropy: {random_entropy:.6f} bits per byte')
    print(f'\nRSA Key Generation ({key_size}-bit):')
    print(f'  - Time: {rsa_time:.6f} seconds')
    print(f'  - Key Length: {len(rsa_public_key)} bytes')
    print(f'  - Entropy: {rsa_key_entropy:.6f} bits per byte')
    print(str('=' * 40))


# Example usage of the comparison function
compare_random_and_rsa(size=1024, key_size=2048)
