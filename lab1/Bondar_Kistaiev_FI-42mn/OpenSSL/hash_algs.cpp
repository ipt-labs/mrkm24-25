#include "hash_algs.h"
#include "generic.h"

#include <openssl/sha.h>

#include <random>
#include <numeric>

#define NOTEBOOK_SIZE 1'048'576'00u
#define DATA_SLICE_SIZE 1'048'5760u

#define MIN_START 0u
#define MAX_START NOTEBOOK_SIZE - DATA_SLICE_SIZE

void measure_SHA256_time()
{
    uint32_t md_size = 32;
    uint32_t repetitions = 10000;
    std::vector<float> sha256_times(repetitions, 0.f);
    
    std::vector<uint8_t> notebook = generate_sample(NOTEBOOK_SIZE); // 10 MB sample range

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> distr(MIN_START, MAX_START);

    std::vector<std::vector<uint8_t>> saved_mds(repetitions, std::vector<uint8_t>(md_size, 0));

    sha256_times = measure_time(repetitions, [&distr, &gen, &notebook](std::vector<uint8_t>* md_save, uint32_t idx) {
         SHA256(&notebook[distr(gen)], DATA_SLICE_SIZE, &(*md_save)[0]);
    }, saved_mds);

    write_times_to_file(sha256_times, "results/SHA256_results.txt");
}


void measure_SHA3_512_time()
{
    size_t md_size = 64;
    uint32_t repetitions = 10000;
    std::vector<float> sha3_512_times(repetitions, 0.f);

    std::vector<uint8_t> notebook = generate_sample(NOTEBOOK_SIZE); // 10 MB sample range

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> distr(MIN_START, MAX_START);

    std::vector<std::vector<uint8_t>> saved_mds(repetitions, std::vector<uint8_t>(md_size, 0));

    sha3_512_times = measure_time(repetitions, [&distr, &gen, &md_size, &notebook](std::vector<uint8_t>* md_save, uint32_t idx) {
        EVP_Q_digest(NULL, "SHA3-512", NULL, &notebook[distr(gen)], DATA_SLICE_SIZE, &(*md_save)[0], &md_size);
    }, saved_mds);

    write_times_to_file(sha3_512_times, "results/SHA3-512_results.txt");
}
