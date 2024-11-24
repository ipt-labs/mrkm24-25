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
  std::random_device rd;  // Seed generator
  std::mt19937 gen(rd()); // Random number generator
  // std::uniform_int_distribution<int> lengthDist(50, 99); // String length
  // range
  std::uniform_int_distribution<int> charDist('a',
                                              'z'); // Characters: 'a' to 'z'
  size_t strLength = 500;
  for (size_t i = 0; i < count; ++i) {
    std::string str;
    for (int j = 0; j < strLength; ++j) {
      str += static_cast<char>(charDist(gen)); // Generate random character
    }
    result.push_back(str);
  }
  return result;
}

// Example usage
int main() {
  size_t count = 1000000; // Number of strings to generate
  std::vector<std::string> input = generateStringsInRange(count);

  // Output the strings
  // for (const auto &str : input) {
  //   std::cout << "String (length " << str.length() << "): " << str <<
  //   std::endl;
  // }
  std::cout << "end generation";
  auto start = std::chrono::high_resolution_clock::now();

  // Code block to measure
  for (int i = 0; i < count; ++i) {
    sha256(input[i]);
  }

  // Stop the timer
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate duration in microseconds
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Execution time: " << duration.count() / (double)1000000
            << " microseconds" << std::endl;

  return 0;
}
// int main() {
//   std::string input = "abc123";
//   std::cout << "Input: " << input << std::endl;
//
//   std::string output = sha256(input);
//   std::cout << "SHA256 Hash: " << output << std::endl;
//
//   return 0;
// }
