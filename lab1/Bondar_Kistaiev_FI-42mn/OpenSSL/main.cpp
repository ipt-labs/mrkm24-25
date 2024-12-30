#include <iostream>
#include "generic.h"

//#include "asym_keys_generation.h"
#include "math_funcs.h"
#include "hash_algs.h"
#include "sign_algs.h"
#include "sym_algs.h"

int main() 
{
    RAND_poll();
    if (!RAND_status()) {
        std::cerr << "Insufficient randomness!" << std::endl;
    }

    std::cout << "Big Primes!" << std::endl;
    measure_GenBigPrime_time();
    std::cout << "SHA256!" << std::endl;
    measure_SHA256_time();
    std::cout << "SHA3-512!" << std::endl;
    measure_SHA3_512_time();
    std::cout << "RSA!" << std::endl;
    measure_RSA_sign_verify_time();
    std::cout << "DSA!" << std::endl;
    measure_DSA_sign_verify_time();
    std::cout << "AES!" << std::endl;
    measure_AES_enc_dec_time();
    std::cout << "ChaCha20!" << std::endl;
    measure_ChaCha20_enc_dec_time();

    return 0;
}
