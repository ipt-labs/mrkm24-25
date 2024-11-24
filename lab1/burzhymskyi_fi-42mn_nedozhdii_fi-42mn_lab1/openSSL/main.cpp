#include <chrono>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <random>
#include <sstream>
#include <string>
#include <vector>

std::string sha256(const std::string &data) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, data.c_str(), data.size());
  SHA256_Final(hash, &sha256);

  std::ostringstream oss;
  for (unsigned char c : hash) {
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
  }
  return oss.str();
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
  int count = input.size();
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

#define CHACHA20_KEY_SIZE 32  // 256-bit key
#define CHACHA20_NONCE_SIZE 8 // 64-bit nonce

std::string encryptChaCha20(const std::string &plaintext,
                            const unsigned char *key,
                            const unsigned char *nonce) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Failed to create cipher context");
  }

  std::string ciphertext(plaintext.size(), '\0');
  int outlen;

  if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), nullptr, key, nonce) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to initialize encryption");
  }

  if (EVP_EncryptUpdate(
          ctx, reinterpret_cast<unsigned char *>(&ciphertext[0]), &outlen,
          reinterpret_cast<const unsigned char *>(plaintext.c_str()),
          plaintext.size()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to encrypt data");
  }

  EVP_CIPHER_CTX_free(ctx);
  return ciphertext;
}
void generateRandomBytes(unsigned char *buffer, size_t size) {
  if (!RAND_bytes(buffer, size)) {
    throw std::runtime_error("Failed to generate random bytes");
  }
}
std::string decryptChaCha20(const std::string &ciphertext,
                            const unsigned char *key,
                            const unsigned char *nonce) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Failed to create cipher context");
  }

  std::string plaintext(ciphertext.size(), '\0');
  int outlen;

  if (EVP_DecryptInit_ex(ctx, EVP_chacha20(), nullptr, key, nonce) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to initialize decryption");
  }

  if (EVP_DecryptUpdate(
          ctx, reinterpret_cast<unsigned char *>(&plaintext[0]), &outlen,
          reinterpret_cast<const unsigned char *>(ciphertext.c_str()),
          ciphertext.size()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to decrypt data");
  }

  EVP_CIPHER_CTX_free(ctx);
  return plaintext;
}
void testChaCha20(const std::vector<std::string> &input) {

  unsigned char key[CHACHA20_KEY_SIZE];
  unsigned char nonce[CHACHA20_NONCE_SIZE];
  generateRandomBytes(key, CHACHA20_KEY_SIZE);
  generateRandomBytes(nonce, CHACHA20_NONCE_SIZE);

  // Encrypt the plaintext
  int count = input.size();
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count; ++i) {
    std::string ciphertext = encryptChaCha20(input[i], key, nonce);
    std::string decryptedText = decryptChaCha20(ciphertext, key, nonce);
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;
}

void testDSA() {}

int main() {

  size_t count = 1000000;
  std::vector<std::string> input = generateStringsInRange(count);

  std::cout << "end generation";
  testChaCha20(input);
  testDSA();
  testSHA256(input);

  return 0;
}
