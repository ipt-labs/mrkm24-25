#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <string>

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

int main() {
  std::string input = "Hello, Crypto++!";
  std::cout << "Input: " << input << std::endl;

  std::string output = sha256(input);
  std::cout << "SHA256 Hash: " << output << std::endl;

  return 0;
}
