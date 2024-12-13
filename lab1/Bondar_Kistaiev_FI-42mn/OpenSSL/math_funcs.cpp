#include "math_funcs.h"
#include "generic.h"

#include <openssl/bn.h>

void measure_GenBigPrime_time()
{
    uint32_t samples_num = 3;
    uint64_t big_prime_sizes[] = { 512, 1024, 2048 };
    uint16_t repetitions[] = { 1000, 400, 100 };
    std::vector<float> big_prime_times[3];
    std::vector<BIGNUM*> primes_collections[] = { std::vector<BIGNUM*>(repetitions[0], BN_new()),
                                                  std::vector<BIGNUM*>(repetitions[1],  BN_new()),
                                                  std::vector<BIGNUM*>(repetitions[2],  BN_new())
    };

    for (uint32_t i = 0; i < samples_num; ++i)
    {
        big_prime_times[i] = measure_time(repetitions[i], [](uint32_t prime_size, BIGNUM** prime_handler, uint32_t) {
            BN_generate_prime_ex(*prime_handler, prime_size, 1, NULL, NULL, NULL);
        }, primes_collections[i], big_prime_sizes[i]);


        write_times_to_file(big_prime_times[i], std::format("results/big_primes_results_{}.txt", big_prime_sizes[i]));
    }
}
