#include "generic.h"

#include <fstream>
#include <iomanip>

#include <openssl/bio.h>
#include <openssl/applink.c>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>



void print_PKEY_to_BIO(EVP_PKEY* key) {
    BIO* bio = BIO_new_fp(stdout, BIO_NOCLOSE);

    EVP_PKEY_print_params(bio, key, 0, NULL);

    BIO_free(bio);
}


std::vector<uint8_t> generate_sample(uint32_t size_bytes)
{
    std::vector<uint8_t> data(size_bytes);

    RAND_bytes(&data[0], size_bytes);

    return data;
}

void write_times_to_file(const std::vector<float>& times, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    for (size_t i = 0; i < times.size(); ++i) {
        file << std::fixed << std::setprecision(6) << times[i] << ' ';
    }

    file.close();
}