#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

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

int main() {
  size_t count = 1000000;
  std::vector<std::string> input = generateStringsInRange(count);

  std::cout << "end generation";
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count; ++i) {
    sha256(input[i]);
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;

  return 0;
}
