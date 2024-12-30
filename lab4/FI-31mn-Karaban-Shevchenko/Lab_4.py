import os

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.asymmetric.dh import DHPrivateKey, DHPublicKey
from cryptography.hazmat.primitives.kdf.hkdf import HKDF


def string_to_int(data: str) -> int:
    """Convert a string to an integer using UTF-8 encoding.

    Args:
        data (str): The input string to convert.

    Returns:
        int: The integer representation of the input string.
    """
    return int.from_bytes(data.encode('utf-8'), 'big')


def int_to_string(data: int) -> str:
    """Convert an integer back to a string using UTF-8 decoding.

    Args:
        data (int): The integer to convert back to a string.

    Returns:
        str: The decoded string, or an error message if decoding fails.
    """
    try:
        return data.to_bytes((data.bit_length() + 7) // 8, 'big').decode('utf-8')
    except UnicodeDecodeError:
        return '[Decryption failed: Non-decodable data]'


def generate_keys() -> tuple[DHPrivateKey, DHPublicKey]:
    """Generate a private-public key pair using Diffie-Hellman.

    Returns:
        tuple[DHPrivateKey, DHPublicKey]: A tuple containing the private and public keys.
    """
    parameters = dh.generate_parameters(generator=2, key_size=512)
    private_key = parameters.generate_private_key()
    public_key = private_key.public_key()

    return private_key, public_key


def encrypt(public_key: DHPublicKey, message: str) -> tuple[int, int]:
    """Encrypt a message using the recipient's public key.

    Args:
        public_key (DHPublicKey): The recipient's public key.
        message (str): The plaintext message to encrypt.

    Returns:
        tuple[int, int]: A tuple containing the ephemeral public key component and the ciphertext.
    """
    shared_parameters = public_key.parameters()
    ephemeral_private = shared_parameters.generate_private_key()
    ephemeral_public = ephemeral_private.public_key()
    shared_key = ephemeral_private.exchange(public_key)
    derived_key = HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'elgamal-encryption',
    ).derive(shared_key)
    message_int = string_to_int(message)
    ciphertext = (message_int * int.from_bytes(derived_key, 'big')) % shared_parameters.parameter_numbers().p

    return ephemeral_public.public_numbers().y, ciphertext


def decrypt(private_key: DHPrivateKey, ephemeral_key_y: int, ciphertext: int) -> str:
    """Decrypt a ciphertext using the recipient's private key.

    Args:
        private_key (DHPrivateKey): The recipient's private key.
        ephemeral_key_y (int): The ephemeral public key component from the encryption.
        ciphertext (int): The encrypted message as an integer.

    Returns:
        str: The decrypted plaintext message.
    """
    parameters = private_key.parameters()
    ephemeral_public_numbers = dh.DHPublicNumbers(ephemeral_key_y, parameters.parameter_numbers())
    ephemeral_public_key = ephemeral_public_numbers.public_key()
    shared_key = private_key.exchange(ephemeral_public_key)
    derived_key = HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'elgamal-encryption',
    ).derive(shared_key)
    shared_key_int = int.from_bytes(derived_key, 'big')
    p = parameters.parameter_numbers().p
    plaintext_int = (ciphertext * pow(shared_key_int, -1, p)) % p

    return int_to_string(plaintext_int)


def sign(private_key: DHPrivateKey, message: str) -> tuple[int, int]:
    """Sign a message using the sender's private key.

    Args:
        private_key (DHPrivateKey): The sender's private key.
        message (str): The plaintext message to sign.

    Returns:
        tuple[int, int]: The signature as a tuple of (r, s).
    """
    parameters = private_key.parameters()
    p = parameters.parameter_numbers().p
    q = (p - 1) // 2
    k = os.urandom(32)
    k = int.from_bytes(k, 'big') % q

    while k in (0, (p - 1) % k):
        k = os.urandom(32)
        k = int.from_bytes(k, 'big') % q

    r = pow(2, k, p)
    k_inv = pow(k, -1, q)
    message_int = string_to_int(message)
    s = (k_inv * (message_int - private_key.private_numbers().x * r)) % q

    return r, s


def verify(public_key: DHPublicKey, message: str, signature: tuple[int, int]) -> bool:
    """Verify a signature using the sender's public key.

    Args:
        public_key (DHPublicKey): The sender's public key.
        message (str): The plaintext message that was signed.
        signature (tuple[int, int]): The signature to verify as (r, s).

    Returns:
        bool: True if the signature is valid, False otherwise.
    """
    r, s = signature
    parameters = public_key.parameters()
    p = parameters.parameter_numbers().p
    q = (p - 1) // 2

    if not (0 < r < p) or not (0 < s < q):
        return False

    message_int = string_to_int(message)
    v1 = (pow(public_key.public_numbers().y, r, p) * pow(r, s, p)) % p
    v2 = pow(2, message_int, p)

    return v1 == v2


def brute_force_attack(public_key: DHPublicKey, ephemeral_y: int, ciphertext: int, max_attempts: int) -> str | None:
    """Attempt to brute-force the private key of the receiver.

    Args:
        public_key (DHPublicKey): The recipient's public key.
        ephemeral_y (int): The ephemeral public key component from encryption.
        ciphertext (int): The encrypted message as an integer.
        max_attempts (int): The maximum number of attempts for brute-forcing.

    Returns:
        str | None: The decrypted plaintext if successful, otherwise None.
    """
    parameters = public_key.parameters()
    p = parameters.parameter_numbers().p

    for i in range(max_attempts):
        private_key_candidate = i
        ephemeral_public_numbers = dh.DHPublicNumbers(ephemeral_y, parameters.parameter_numbers())
        ephemeral_public_key = ephemeral_public_numbers.public_key()
        shared_key = private_key_candidate * ephemeral_public_key.public_numbers().y % p
        derived_key = HKDF(
            algorithm=hashes.SHA256(),
            length=32,
            salt=None,
            info=b'elgamal-encryption',
        ).derive(shared_key.to_bytes((shared_key.bit_length() + 7) // 8, 'big'))
        shared_key_int = int.from_bytes(derived_key, 'big')
        plaintext_int = (ciphertext * pow(shared_key_int, -1, p)) % p
        plaintext = int_to_string(plaintext_int)
        print(f"Attempt {i + 1}: Decrypted Message = '{plaintext}' with candidate key {private_key_candidate}")

        if plaintext == 'Hello, ElGamal with Python!':
            print(f'Success! Decrypted message matches with candidate private key {private_key_candidate}')
            return plaintext

    print('Brute-force attack failed to find the correct key.')
    return None


if __name__ == '__main__':
    message = 'Hello, ElGamal with Python!'

    # Generate key pairs
    private_key, public_key = generate_keys()

    # Encryption
    ephemeral_y, ciphertext = encrypt(public_key, message)
    print(f'Original Message: {message}')
    print(f'Ciphertext: (ephemeral_y={ephemeral_y}, ciphertext={ciphertext})')

    # Decryption
    decrypted_message = decrypt(private_key, ephemeral_y, ciphertext)
    print(f'Decrypted Message: {decrypted_message}')

    # Signing
    signature = sign(private_key, message)
    print(f'Signature: {signature}')

    # Verification
    is_valid = verify(public_key, message, signature)
    print(f'Signature Valid: {is_valid}')

    # Brute-force attack
    print(f"\n{'=' * 40}\nLet's try brute-force attack!\n{'=' * 40}")
    max_attempts = 1000000
    result = brute_force_attack(public_key, ephemeral_y, ciphertext, max_attempts)
    print(f'Brute-force attack result: {result}')
