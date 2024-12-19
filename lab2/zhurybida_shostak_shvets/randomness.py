from Crypto.Random import get_random_bytes
import time
import math

def generate_random_bytes(size):
    """Generate random bytes and measure the time taken."""
    start_time = time.time()
    random_data = get_random_bytes(size)
    elapsed_time = time.time() - start_time
    print(f"Generated Random Bytes: {random_data.hex()[:64]}... (truncated)")
    return random_data, elapsed_time

def calculate_entropy(data):
    """Calculate the Shannon entropy of the given byte data."""
    probabilities = [data.count(byte) / len(data) for byte in set(data)]
    return -sum(p * math.log2(p) for p in probabilities)

from Crypto.PublicKey import RSA

def generate_rsa_keypair(key_size=2048):
    """Generate an RSA key pair and measure the time taken."""
    start_time = time.time()
    key = RSA.generate(key_size)
    elapsed_time = time.time() - start_time
    print(f"Generated RSA Key Pair ({key_size}-bit)")
    return key, elapsed_time

def compare_random_and_rsa(size, key_size):
    """Compare random byte generation and RSA key generation."""
    print("\n--- Testing Random Byte Generation ---")
    random_bytes, random_time = generate_random_bytes(size)
    random_entropy = calculate_entropy(random_bytes)

    print("\n--- Testing RSA Key Generation ---")
    rsa_key, rsa_time = generate_rsa_keypair(key_size)

    print("\n--- Comparison Results ---")
    print(f"Random Bytes Generation (Size: {size} bytes):")
    print(f"  Time Taken: {random_time:.6f} seconds")
    print(f"  Entropy: {random_entropy:.6f} bits per byte")

    print(f"\nRSA Key Generation ({key_size}-bit):")
    print(f"  Time Taken: {rsa_time:.6f} seconds")
    print(f"  Public Key Length: {len(rsa_key.publickey().export_key())} bytes")
    print(f"  Entropy: {calculate_entropy(rsa_key.publickey().export_key())} bits per byte")

    compare_random_and_rsa(size=256, key_size=2048)