#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

#include <cryptopp/chacha.h>
#include <cryptopp/osrng.h>

#include <cryptopp/base64.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/dsa.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace CryptoPP;
std::string sha256(const std::string &input) {
  CryptoPP::SHA256 hash;
  std::string digest;

  CryptoPP::StringSource ss(
      input, true,
      new CryptoPP::HashFilter(
          hash,
          new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest), true)));

  return digest;
}
std::vector<std::string> generateStringsInRange(size_t count) {
  std::vector<std::string> result;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> charDist('a', 'z');
  size_t strLength = 500;
  for (size_t i = 0; i < count; ++i) {
    std::string str;
    for (int j = 0; j < strLength; ++j) {
      str += static_cast<char>(charDist(gen));
    }
    result.push_back(str);
  }
  return result;
}

void testSHA256(const std::vector<std::string> &input) {
  size_t count = input.size();
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count; ++i) {
    sha256(input[i]);
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;
}

std::string generateRandomKey(size_t length) {
  CryptoPP::AutoSeededRandomPool prng;
  std::string key(length, '\0');
  prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&key[0]), key.size());
  return key;
}

std::string generateRandomNonce(size_t length) {
  CryptoPP::AutoSeededRandomPool prng;
  std::string nonce(length, '\0');
  prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&nonce[0]),
                     nonce.size());
  return nonce;
}

std::string encrypt(const std::string &plaintext, const std::string &key,
                    const std::string &nonce) {
  std::string ciphertext;
  CryptoPP::ChaCha::Encryption chacha;
  chacha.SetKeyWithIV(
      reinterpret_cast<const CryptoPP::byte *>(key.data()), key.size(),
      reinterpret_cast<const CryptoPP::byte *>(nonce.data()), nonce.size());
  CryptoPP::StringSource ss(plaintext, true,
                            new CryptoPP::StreamTransformationFilter(
                                chacha, new CryptoPP::StringSink(ciphertext)));
  return ciphertext;
}

std::string decrypt(const std::string &ciphertext, const std::string &key,
                    const std::string &nonce) {
  std::string plaintext;
  CryptoPP::ChaCha::Decryption chacha;
  chacha.SetKeyWithIV(
      reinterpret_cast<const CryptoPP::byte *>(key.data()), key.size(),
      reinterpret_cast<const CryptoPP::byte *>(nonce.data()), nonce.size());
  CryptoPP::StringSource ss(ciphertext, true,
                            new CryptoPP::StreamTransformationFilter(
                                chacha, new CryptoPP::StringSink(plaintext)));
  return plaintext;
}

// Helper function to print data in hexadecimal format
void printHex(const std::string &label, const std::string &data) {
  std::string hex;
  CryptoPP::StringSource ss(
      data, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex)));
  std::cout << label << ": " << hex << std::endl;
}

void testChaCha20(const std::vector<std::string> &input) {
  std::string key = generateRandomKey(32);    // 256 bits = 32 bytes
  std::string nonce = generateRandomNonce(8); // 96 bits = 12 bytes

  size_t count = input.size();
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count; ++i) {
    std::string ciphertext = encrypt(input[i], key, nonce);
    std::string decryptedtext = decrypt(ciphertext, key, nonce);
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;
}

void GenerateKeys(DSA::PrivateKey &privateKey, DSA::PublicKey &publicKey) {
  AutoSeededRandomPool rng;

  privateKey.GenerateRandomWithKeySize(rng, 2048);

  privateKey.MakePublicKey(publicKey);
}

std::string signMessage(const DSA::PrivateKey &privateKey,
                        const std::string &message) {
  AutoSeededRandomPool rng;
  DSA::Signer signer(privateKey);

  std::string signature;
  StringSource(message, true,
               new SignerFilter(rng, signer, new StringSink(signature)));

  return signature;
}

bool verifySignature(const DSA::PublicKey &publicKey,
                     const std::string &message, const std::string &signature) {
  DSA::Verifier verifier(publicKey);

  bool result = false;
  StringSource(signature + message, true,
               new SignatureVerificationFilter(
                   verifier, new ArraySink((byte *)&result, sizeof(result))));

  return result;
}
void testDSA() {
  DSA::PrivateKey privateKey;
  DSA::PublicKey publicKey;

  auto start = std::chrono::high_resolution_clock::now();
  GenerateKeys(privateKey, publicKey);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "DSA geygen time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;

  size_t count = 10000;
  std::vector<std::string> input = generateStringsInRange(count);
  start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count; ++i) {
    std::string signature = signMessage(privateKey, input[i]);
    verifySignature(publicKey, input[i], signature);
  }

  end = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;
}

int main() {
  size_t count = 1000000;
  std::vector<std::string> input = generateStringsInRange(count);

  std::cout << "end generation\n";
  std::cout << "SHA256:\n";
  testSHA256(input);
  std::cout << "ChaCha20:\n";
  testChaCha20(input);
  std::cout << "DSA:\n";
  testDSA();
  return 0;
}
