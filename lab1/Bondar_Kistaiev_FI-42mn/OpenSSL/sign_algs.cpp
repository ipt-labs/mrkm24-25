#include "sign_algs.h"
#include "generic.h"

#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/params.h>
#include <openssl/core_names.h>

void measure_RSA_sign_verify_time()
{
    const uint32_t samples_num = 3;
    uint32_t message_sz = 32;
    uint32_t rsa_key_sizes[] = { 1024, 2048, 4096 };
    uint16_t repetitions[] = { 4000, 1000, 300 };
    std::vector<float> rsa_key_times[3];
    std::vector<float> rsa_sign_times[3];
    std::vector<float> rsa_verify_times[3];
    std::vector<EVP_PKEY*> keys_collections[] = { std::vector<EVP_PKEY*>(repetitions[0], NULL),
                                                  std::vector<EVP_PKEY*>(repetitions[1], NULL),
                                                  std::vector<EVP_PKEY*>(repetitions[2], NULL)
    };

    std::vector<std::vector<uint8_t>> messages[samples_num] =
    {
        std::vector<std::vector<uint8_t>>(repetitions[0]),
        std::vector<std::vector<uint8_t>>(repetitions[1]),
        std::vector<std::vector<uint8_t>>(repetitions[2])
    };
    for (uint32_t i = 0; i < samples_num; ++i)
        for (auto& msg : messages[i])
            msg = generate_sample(message_sz);


    for (uint32_t i = 0; i < samples_num; ++i)
    {
        // Key generation
        rsa_key_times[i] = measure_time(repetitions[i], [](uint32_t key_size, EVP_PKEY** ppkey, uint32_t idx) {
            *ppkey = EVP_RSA_gen(key_size);
            }, keys_collections[i], rsa_key_sizes[i]);

        // Signature contexts
        std::vector<EVP_PKEY_CTX*> rsa_ctxs;
        for (auto& pkey : keys_collections[i])
        {
            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL);
            if (ctx == NULL)
                std::cout << "CTX error!" << std::endl;

            if (EVP_PKEY_sign_init(ctx) <= 0)
                std::cout << "Sign Init error!" << std::endl;

            if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0)
                std::cout << "Padding signature!" << std::endl;

            if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
                std::cout << "MD error!" << std::endl;

            rsa_ctxs.push_back(ctx);
        }
        std::vector<std::vector<uint8_t>> signatures(repetitions[i]);

        // Signature
        rsa_sign_times[i] = measure_time(repetitions[i], [&rsa_ctxs](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<uint8_t>* signature, uint32_t idx) {
            size_t sign_size;
            if (EVP_PKEY_sign(rsa_ctxs[idx], NULL, &sign_size, messages_vec[idx].data(), messages_vec[idx].size()) <= 0)
                std::cout << "Sign length error!" << std::endl;
            signature->resize(sign_size);

            if (EVP_PKEY_sign(rsa_ctxs[idx], signature->data(), &sign_size, (unsigned char*)messages_vec[idx].data(), messages_vec[idx].size()) <= 0)
            {
                std::cout << "Signature error!" << std::endl;
                ERR_print_errors_fp(stderr);
            }
        }, signatures, messages[i]);

        // Clear contexts and create new for verification
        for (uint32_t j = 0; j < rsa_ctxs.size(); ++j)
        {
            EVP_PKEY_CTX_free(rsa_ctxs[j]);

            rsa_ctxs[j] = EVP_PKEY_CTX_new(keys_collections[i][j], NULL /* no engine */);
            if (rsa_ctxs[j] == NULL)
                std::cout << "CTX error!" << std::endl;

            if (EVP_PKEY_verify_init(rsa_ctxs[j]) <= 0)
                std::cout << "Verify init error!" << std::endl;

            if (EVP_PKEY_CTX_set_rsa_padding(rsa_ctxs[j], RSA_PKCS1_PADDING) <= 0)
                std::cout << "Padding verify!" << std::endl;

            if (EVP_PKEY_CTX_set_signature_md(rsa_ctxs[j], EVP_sha256()) <= 0)
                std::cout << "Verify md error!" << std::endl;
        }

        // Verification
        rsa_verify_times[i] = measure_time(repetitions[i], [&rsa_ctxs, &rsa_key_sizes, &i](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<uint8_t>* signature, uint32_t idx) {
            size_t verify_size = signature->size();
            int ret = EVP_PKEY_verify(rsa_ctxs[idx], signature->data(), verify_size, (unsigned char*)messages_vec[idx].data(), messages_vec[idx].size());
            if (ret < 0)
            {
                std::cout << "RSA (" << rsa_key_sizes[i] << ") Verify ERR!" << std::endl;
                ERR_print_errors_fp(stderr);
            }
            else if (ret == 0)
                std::cout << "RSA (" << rsa_key_sizes[i] << ") Message is not verified!" << std::endl;
            else
                /*std::cout << "RSA (" << rsa_key_sizes[i] << ") Verify OK!" << std::endl*/;
        }, signatures, messages[i]);



        write_times_to_file(rsa_key_times[i], std::format("results/RSA_keygen_{}.txt", rsa_key_sizes[i]));
        write_times_to_file(rsa_sign_times[i], std::format("results/RSA_signature_{}.txt", rsa_key_sizes[i]));
        write_times_to_file(rsa_verify_times[i], std::format("results/RSA_verify_{}.txt", rsa_key_sizes[i]));
    }
}

void measure_DSA_sign_verify_time()
{
    const uint32_t samples_num = 4;
    uint32_t p_size[] = { 1024, 2048, 2048, 3072 };
    uint32_t q_size[] = { 160, 224, 256, 256 };
    int gindex = 1;

    uint32_t message_sz = 1024;

    uint16_t repetitions[] = { 3000, 800, 500, 300 };
    std::vector<float> dsa_key_times[samples_num];
    std::vector<float> dsa_sign_times[samples_num];
    std::vector<float> dsa_verify_times[samples_num];
    std::vector<EVP_PKEY*> keys_collections[] = 
    { 
        std::vector<EVP_PKEY*>(repetitions[0], NULL),
        std::vector<EVP_PKEY*>(repetitions[1], NULL),
        std::vector<EVP_PKEY*>(repetitions[2], NULL),
        std::vector<EVP_PKEY*>(repetitions[3], NULL)
    };

    std::vector<std::vector<uint8_t>> messages[samples_num] = 
    { 
        std::vector<std::vector<uint8_t>>(repetitions[0]),
        std::vector<std::vector<uint8_t>>(repetitions[1]),
        std::vector<std::vector<uint8_t>>(repetitions[2]),
        std::vector<std::vector<uint8_t>>(repetitions[3]),
    };
    for (uint32_t i = 0; i < samples_num; ++i)
        for (auto& msg : messages[i])
            msg = generate_sample(message_sz);


    OSSL_PARAM params[5];
    EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_from_name(NULL, "DSA", NULL);
    for (uint32_t i = 0; i < samples_num; ++i)
    {
        EVP_PKEY_paramgen_init(pctx);

        params[0] = OSSL_PARAM_construct_uint("pbits", &p_size[i]);
        params[1] = OSSL_PARAM_construct_uint("qbits", &q_size[i]);
        params[2] = OSSL_PARAM_construct_int("gindex", &gindex);
        params[3] = OSSL_PARAM_construct_utf8_string("digest", (char*)"SHA384", 0);
        params[4] = OSSL_PARAM_construct_end();

        EVP_PKEY_CTX_set_params(pctx, params);

        // Key generation contexts
        EVP_PKEY* params_pkey = NULL;
        EVP_PKEY_paramgen(pctx, &params_pkey);

        std::vector<EVP_PKEY_CTX*> dsa_pkeygen_ctxs(repetitions[i], NULL);
        for (auto& kctx : dsa_pkeygen_ctxs)
        {
            kctx = EVP_PKEY_CTX_new(params_pkey, NULL);
            if (kctx == NULL)
                std::cout << "Keygen CTX error!" << std::endl;

            if(EVP_PKEY_keygen_init(kctx) <= 0)
                std::cout << "Keygen init error!" << std::endl;

        }

        // Key generation
        dsa_key_times[i] = measure_time(repetitions[i], [&dsa_pkeygen_ctxs](EVP_PKEY** ppkey, uint32_t idx) {
            EVP_PKEY_generate(dsa_pkeygen_ctxs[idx], ppkey);
        }, keys_collections[i]);
 
        // Signature contexts
        std::vector<EVP_PKEY_CTX*> dsa_ctxs;
        for (uint32_t j = 0; j < keys_collections[i].size(); ++j)
        {
            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(keys_collections[i][j], NULL);
            if (ctx == NULL)
                std::cout << "Sign CTX error!" << std::endl;

            if (EVP_PKEY_sign_init(ctx) <= 0)
                std::cout << "Sign Init error!" << std::endl;

            if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha384()) <= 0)
                std::cout << "MD error!" << std::endl;

            dsa_ctxs.push_back(ctx);


            EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
            if (!md_ctx)
                std::cout << "MD CTX error!" << std::endl;

            if (EVP_DigestInit_ex(md_ctx, EVP_sha384(), NULL) <= 0)
                std::cout << "MD init error!" << std::endl;

            if (EVP_DigestUpdate(md_ctx, messages[i][j].data(), messages[i][j].size()) <= 0)
                std::cout << "MD Update error!" << std::endl;

            messages[i][j].resize(48);
            uint32_t hash_size = messages[i][j].size();
            if (EVP_DigestFinal_ex(md_ctx, messages[i][j].data(), &hash_size) <= 0)
                std::cout << "MD Final error!" << std::endl;

            EVP_MD_CTX_free(md_ctx);
        }
        std::vector<std::vector<uint8_t>> signatures(repetitions[i]);


        // Signature
        dsa_sign_times[i] = measure_time(repetitions[i], [&dsa_ctxs](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<uint8_t>* signature, uint32_t idx){
            size_t sign_size;
            if (EVP_PKEY_sign(dsa_ctxs[idx], NULL, &sign_size, messages_vec[idx].data(), messages_vec[idx].size()) <= 0)
                std::cout << "Sign length error!" << std::endl;
            signature->resize(sign_size);

            if (EVP_PKEY_sign(dsa_ctxs[idx], signature->data(), &sign_size, (unsigned char*)messages_vec[idx].data(), messages_vec[idx].size()) <= 0)
                std::cout << "Signature error!" << std::endl;

        }, signatures, messages[i]);

        // Clear contexts and create new for verification
        for (uint32_t j = 0; j < dsa_ctxs.size(); ++j)
        {
            EVP_PKEY_CTX_free(dsa_ctxs[j]);

            dsa_ctxs[j] = EVP_PKEY_CTX_new(keys_collections[i][j], NULL /* no engine */);
            if (dsa_ctxs[j] == NULL)
                std::cout << "Verify CTX error!" << std::endl;

            if (EVP_PKEY_verify_init(dsa_ctxs[j]) <= 0)
                std::cout << "Verify init error!" << std::endl;

            if (EVP_PKEY_CTX_set_signature_md(dsa_ctxs[j], EVP_sha384()) <= 0)
                std::cout << "Verify md error!" << std::endl;
        }

        // Verification
        dsa_verify_times[i] = measure_time(repetitions[i], [&dsa_ctxs, &p_size, &q_size, &i](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<uint8_t>* signature, uint32_t idx) {
            size_t verify_size = signature->size();
            int ret = EVP_PKEY_verify(dsa_ctxs[idx], signature->data(), verify_size, (unsigned char*)messages_vec[idx].data(), messages_vec[idx].size());

            if (ret < 0)
            {
                std::cout << "DSA (" << p_size[i] << " : " << q_size[i] << ") Verify ERR!" << std::endl;
                ERR_print_errors_fp(stderr);
            }
            else if (ret == 0)
                std::cout << "DSA (" << p_size[i] << " : " << q_size[i] << ") Message is not verified!" << std::endl;
            else
                std::cout << "DSA (" << p_size[i] << " : " << q_size[i] << ") Verify OK!" << std::endl;
        }, signatures, messages[i]);
       
        // Free verify ctxs
        for (auto& vctx : dsa_ctxs)
        {
            EVP_PKEY_CTX_free(vctx);
        }


        write_times_to_file(dsa_key_times[i], std::format("results/DSA_keygen_{}_{}.txt", p_size[i], q_size[i]));
        write_times_to_file(dsa_sign_times[i], std::format("results/DSA_signature_{}_{}.txt", p_size[i], q_size[i]));
        write_times_to_file(dsa_verify_times[i], std::format("results/DSA_verify_{}_{}.txt", p_size[i], q_size[i]));
    }

    EVP_PKEY_CTX_free(pctx);
}
