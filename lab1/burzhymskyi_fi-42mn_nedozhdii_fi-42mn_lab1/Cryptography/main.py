from cryptography.hazmat.primitives import hashes

digest = hashes.Hash(hashes.SHA256())

digest.update(b"abc")

digest.update(b"123")

print(digest.finalize())