import os

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.hashes import SHA256


def generate_aes_key() -> bytes:
    """Generates a random AES key.

    Returns:
        bytes: A randomly generated 256-bit AES key.
    """
    return os.urandom(32)


def encrypt_with_aes(key: bytes, plaintext: str) -> tuple[bytes, bytes]:
    """Encrypts plaintext using AES encryption with CFB mode.

    Args:
        key (bytes): The AES encryption key.
        plaintext (str): The plaintext message to encrypt.

    Returns:
        tuple[bytes, bytes]: A tuple containing the initialization vector (IV) and the ciphertext.
    """
    iv = os.urandom(16)
    cipher = Cipher(algorithms.AES(key), modes.CFB(iv), backend=default_backend())
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext.encode()) + encryptor.finalize()

    return iv, ciphertext


def generate_rsa_keypair() -> tuple[rsa.RSAPrivateKey, rsa.RSAPublicKey]:
    """Generates an RSA private and public key pair.

    Returns:
        tuple[rsa.RSAPrivateKey, rsa.RSAPublicKey]: A tuple containing the RSA private key and public key.
    """
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048, backend=default_backend())
    public_key = private_key.public_key()

    return private_key, public_key


def sign_data(private_key: rsa.RSAPrivateKey, data: bytes) -> bytes:
    """Signs data using an RSA private key.

    Args:
        private_key (rsa.RSAPrivateKey): The RSA private key used for signing.
        data (bytes): The data to sign.

    Returns:
        bytes: The generated signature.
    """
    return private_key.sign(
        data, padding.PSS(mgf=padding.MGF1(SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256()
    )


def verify_signature(public_key: rsa.RSAPublicKey, signature: bytes, data: bytes) -> bool:
    """Verifies an RSA signature.

    Args:
        public_key (rsa.RSAPublicKey): The RSA public key used for verification.
        signature (bytes): The signature to verify.
        data (bytes): The original data.

    Returns:
        bool: True if the signature is valid, False otherwise.
    """
    try:
        public_key.verify(
            signature,
            data,
            padding.PSS(mgf=padding.MGF1(SHA256()), salt_length=padding.PSS.MAX_LENGTH),
            hashes.SHA256(),
        )
        return True
    except Exception:
        return False


if __name__ == '__main__':
    # AES encryption example
    original_message = 'Secure Message'
    aes_key = generate_aes_key()
    iv, ciphertext = encrypt_with_aes(aes_key, original_message)

    print(f'Original Message: {original_message}')
    print(f'AES Key: {aes_key.hex()}')
    print(f'Initialization Vector (IV): {iv.hex()}')
    print(f'Ciphertext: {ciphertext.hex()}')

    # RSA key generation and signing example
    private_key, public_key = generate_rsa_keypair()
    data = b'Important data'
    signature = sign_data(private_key, data)

    print(f'Data to Sign: {data.decode()}')
    print(f'Signature: {signature.hex()}')

    # Verify the signature
    is_valid = verify_signature(public_key, signature, data)
    print(f'Signature Valid: {is_valid}')
