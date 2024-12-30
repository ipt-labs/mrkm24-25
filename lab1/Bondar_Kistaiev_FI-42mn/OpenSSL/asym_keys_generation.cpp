#include "asym_keys_generation.h"
#include "generic.h"

#include <openssl/rsa.h>
#include <openssl/dsa.h>

void measure_RSA_keygen_time()
{
    const uint32_t samples_num = 3;
    uint32_t rsa_key_sizes[] = { 1024, 2048, 4096 };
    uint16_t repetitions[] = { 1000, 300, 100 };
    std::vector<float> rsa_key_times[samples_num];
    std::vector<EVP_PKEY*> keys_collections[] = { std::vector<EVP_PKEY*>(1000, NULL),
                                                  std::vector<EVP_PKEY*>(300, NULL),
                                                  std::vector<EVP_PKEY*>(100, NULL)
                                                 };

    for (uint32_t i = 0; i < samples_num; ++i)
    {
        rsa_key_times[i] = measure_time(repetitions[i], [](uint32_t key_size, EVP_PKEY** ppkey, uint32_t idx) { 
                *ppkey = EVP_RSA_gen(key_size); 
            }, keys_collections[i], rsa_key_sizes[i]);
    }
}

void measure_DSA_keygen_time()
{
    const uint32_t samples_num = 4;
    uint32_t p_size[] = { 1024, 2048, 2048, 3072 };
    uint32_t q_size[] = { 160, 224, 256, 256 };
    int gindex = 1;

    uint16_t repetitions[] = { 1000, 300, 300, 100 };
    std::vector<float> dsa_key_times[samples_num];
    std::vector<EVP_PKEY*> keys_collections[] = { std::vector<EVP_PKEY*>(1000, NULL),
                                                  std::vector<EVP_PKEY*>(300, NULL),
                                                  std::vector<EVP_PKEY*>(300, NULL),
                                                  std::vector<EVP_PKEY*>(100, NULL)
                                                };

    EVP_PKEY* key_handler = nullptr;
    EVP_PKEY_CTX* pctx = NULL;
    pctx = EVP_PKEY_CTX_new_from_name(NULL, "DSA", NULL);
    EVP_PKEY_paramgen_init(pctx);

    OSSL_PARAM params[5];
    for (uint32_t i = 0; i < samples_num; ++i)
    {
        params[0] = OSSL_PARAM_construct_uint("pbits", &p_size[i]);
        params[1] = OSSL_PARAM_construct_uint("qbits", &q_size[i]);
        params[2] = OSSL_PARAM_construct_int("gindex", &gindex);
        params[3] = OSSL_PARAM_construct_utf8_string("digest", (char*)"SHA384", 0);
        params[4] = OSSL_PARAM_construct_end();
        EVP_PKEY_CTX_set_params(pctx, params);

        dsa_key_times[i] = measure_time(repetitions[i], [](EVP_PKEY_CTX* ctx, EVP_PKEY** ppkey, uint32_t idx) {
            EVP_PKEY_generate(ctx, ppkey);
        }, keys_collections[i], pctx);
    }

    EVP_PKEY_CTX_free(pctx);
}