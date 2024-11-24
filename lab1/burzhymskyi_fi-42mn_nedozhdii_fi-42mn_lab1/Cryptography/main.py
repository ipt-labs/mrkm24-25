import random
import string
from cryptography.hazmat.primitives import hashes
import time

def generate_random_strings(num_strings=1000000):
    random_strings = [
        ''.join(random.choices(string.ascii_letters + string.digits, k=500))
        for _ in range(num_strings)
    ]
    return random_strings

def hash_strings_to_bytes(string_list):
    byte_objects = []
    for s in string_list:
        digest = hashes.Hash(hashes.SHA256())
        digest.update(s.encode('utf-8'))
        byte_objects.append(digest.finalize())
    return byte_objects

def measure_hashing_time(string_list):
    start_time = time.time()
    hashed_bytes = hash_strings_to_bytes(string_list)
    end_time = time.time()
    total_time = end_time - start_time
    print(f"Total time taken to hash all strings: {total_time:.6f} seconds")
    return hashed_bytes

print("Generating random strings...")
random_strings = generate_random_strings()

print("Hashing strings to bytes...")
hashed_bytes = measure_hashing_time(random_strings)
