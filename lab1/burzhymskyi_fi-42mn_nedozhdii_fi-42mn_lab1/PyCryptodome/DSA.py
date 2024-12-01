import time
import random
import string
from Crypto.PublicKey import DSA
from Crypto.Signature import DSS
from Crypto.Hash import SHA256

def generate_random_strings(num_strings=10000):
    random_strings = [
        ''.join(random.choices(string.ascii_letters + string.digits, k=500))
        for _ in range(num_strings)
    ]
    return random_strings

def measure_dsa_signature_time(strings):
    start_time = time.time()
    key = DSA.generate(2048)
    pub_key = DSA.import_key(key.publickey().export_key())
    signer = DSS.new(key, 'fips-186-3')
    verifier = DSS.new(pub_key, 'fips-186-3')
    end_time = time.time()
    keygen_time = end_time - start_time
    print(f"Total time taken for key generation: {keygen_time:.6f} seconds")

    total_time = 0

    for message in strings:
        message_bytes = message.encode('utf-8')
        hash_obj = SHA256.new(message_bytes)

        start_time = time.time()

        signature = signer.sign(hash_obj)

        hash_obj = SHA256.new(message_bytes)
        try:
            verifier.verify(hash_obj, signature)
        except ValueError:
            print("The message is not authentic.")

        end_time = time.time()

        total_time += (end_time - start_time)

    return total_time

random_strings = generate_random_strings()

total_time = measure_dsa_signature_time(random_strings)
print(f"Total time taken for signing and verifying {len(random_strings)} strings: {total_time:.6f} seconds")
