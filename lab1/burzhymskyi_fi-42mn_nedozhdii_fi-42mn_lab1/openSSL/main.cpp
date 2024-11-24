#include <cstring>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>

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

int main() {
  std::string input = "Hrrlo, OpenSSL!";
  std::cout << "Input: " << input << std::endl;

  std::string hash = sha256(input);
  std::cout << "SHA256 Hash: " << hash << std::endl;

  return 0;
}
