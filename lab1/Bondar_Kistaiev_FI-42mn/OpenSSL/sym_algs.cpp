#include "sym_algs.h"
#include "generic.h"
#include <algorithm>

bool compare_vectors(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2) {
    if (vec1.size() != vec2.size())
        return false;

    for (size_t i = 0; i < vec1.size(); ++i)
        if (vec1[i] != vec2[i])
            return false;

    return true;
}

void measure_AES_enc_dec_time()
{
    const uint32_t samples_num = 3;
    uint32_t aes_key_sizes[] = { 128, 192, 256 };
    uint32_t aes_block_size_bytes = 16;


    uint16_t repetitions[] = { 300, 300, 300 };
    const EVP_CIPHER* enc_algs[3] = { EVP_aes_128_cbc(), EVP_aes_192_cbc(), EVP_aes_256_cbc()};
    std::vector<float> aes_enc_times[3];
    std::vector<float> aes_dec_times[3];

    std::vector<std::vector<uint8_t>> messages(std::max({ repetitions[0], repetitions[1], repetitions[2] }));
    for (auto& msg : messages)
        msg = generate_sample(1'048'5760u);


    std::vector<std::vector<uint8_t>> ivs(std::max({ repetitions[0], repetitions[1], repetitions[2] }));
    for (auto& iv : ivs)
        iv = generate_sample(aes_block_size_bytes);

    for (uint32_t i = 0; i < samples_num; ++i)
    {
        std::vector<std::vector<uint8_t>> keys(repetitions[i]);
        for (auto& key : keys)
            key = generate_sample(aes_key_sizes[i] / 8);

        std::vector<EVP_CIPHER_CTX*> aes_ctxs(repetitions[i], NULL);
        for (uint32_t j = 0; j < aes_ctxs.size(); ++j)
        {
            aes_ctxs[j] = EVP_CIPHER_CTX_new();
            if (!aes_ctxs[j]) std::cout << "Encryption CTX error!" << std::endl;

        }
        std::vector<std::vector<uint8_t>> ciphertexts(repetitions[i]);
        for (uint32_t j = 0; j < ciphertexts.size(); ++j)
            ciphertexts[j].resize((messages[j].size() * 3) / 2);

        // Ecryption
        aes_enc_times[i] = measure_time(repetitions[i], [&aes_ctxs](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<std::vector<uint8_t>>& ivs_vec, std::vector<std::vector<uint8_t>>& keys_vec, const EVP_CIPHER* ENC_ALG, std::vector<uint8_t>* ciphertext, uint32_t idx) {
            int len;
            int ciphertext_len;

            // Initialize encryption operation
            if (EVP_EncryptInit_ex(aes_ctxs[idx], ENC_ALG, NULL, keys_vec[idx].data(), ivs_vec[idx].data()) != 1)
                std::cout << "Encryption init error!" << std::endl;;

            // Encrypt the plaintext
            if (EVP_EncryptUpdate(aes_ctxs[idx], ciphertext->data(), &len, messages_vec[idx].data(), messages_vec[idx].size()) != 1)
                std::cout << "Encryption update error!" << std::endl;;
            ciphertext_len = len;

            // Finalize encryption
            if (EVP_EncryptFinal_ex(aes_ctxs[idx], ciphertext->data() + len, &len) != 1)
                std::cout << "Encryption finalize error!" << std::endl;;

            ciphertext_len += len;
            ciphertext->resize(ciphertext_len);
        }, ciphertexts, messages, ivs, keys, enc_algs[i]);



        for (uint32_t j = 0; j < aes_ctxs.size(); ++j)
        {
            EVP_CIPHER_CTX_free(aes_ctxs[j]);
            aes_ctxs[j] = EVP_CIPHER_CTX_new();
            if (!aes_ctxs[j]) std::cout << "Decryption CTX error!" << std::endl;
        }
        std::vector<std::vector<uint8_t>> plaintexts(repetitions[i]);
        for (uint32_t j = 0; j < plaintexts.size(); ++j)
            plaintexts[j].resize((messages[j].size() * 3) / 2);

        // Decryption
        aes_dec_times[i] = measure_time(repetitions[i], [&aes_ctxs](std::vector<std::vector<uint8_t>>& ciphertexts_vec, std::vector<std::vector<uint8_t>>& ivs_vec, std::vector<std::vector<uint8_t>>& keys_vec, const EVP_CIPHER* ENC_ALG, std::vector<uint8_t>* plaintext, uint32_t idx) {
            int len;
            int plaintext_len;

            // Initialize decryption operation
            if (EVP_DecryptInit_ex(aes_ctxs[idx], ENC_ALG, NULL, keys_vec[idx].data(), ivs_vec[idx].data()) != 1)
                std::cout << "Decryption init error!" << std::endl;

            // Encrypt the plaintext
            if (EVP_DecryptUpdate(aes_ctxs[idx], plaintext->data(), &len, ciphertexts_vec[idx].data(), ciphertexts_vec[idx].size()) != 1)
                std::cout << "Decryption update error!" << std::endl;
            plaintext_len = len;

            // Finalize decryption
            if (EVP_DecryptFinal_ex(aes_ctxs[idx], plaintext->data() + len, &len) != 1)
            {
                std::cout << "Decryption finalize error!" << std::endl;
                ERR_print_errors_fp(stderr);
            }

            plaintext_len += len;
            plaintext->resize(plaintext_len);
        }, plaintexts, ciphertexts, ivs, keys, enc_algs[i]);

        // Comparison
        for (uint32_t j = 0; j < plaintexts.size(); ++j)
            if (!compare_vectors(plaintexts[j], messages[j]))
                std::cout << "Comparison failed for (key_sz, msg_id): (" << aes_key_sizes[i] << ", " << j << ")" << std::endl;

        write_times_to_file(aes_enc_times[i], std::format("results/AES_encryption_{}.txt", aes_key_sizes[i]));
        write_times_to_file(aes_dec_times[i], std::format("results/AES_decryption_{}.txt", aes_key_sizes[i]));
    }
}

void measure_ChaCha20_enc_dec_time()
{
    uint32_t key_size = 256 / 8;
    uint32_t nonce_size = 96 / 8;
    const EVP_CIPHER* cipher = EVP_chacha20();

    uint16_t repetitions = 400;
    std::vector<float> chacha20_enc_times;
    std::vector<float> chacha20_dec_times;

    std::vector<std::vector<uint8_t>> messages(repetitions);
    for (auto& msg : messages)
        msg = generate_sample(1'048'5760u);


    std::vector<std::vector<uint8_t>> nonces(repetitions);
    for (auto& n : nonces)
        n = generate_sample(nonce_size);

    std::vector<std::vector<uint8_t>> keys(repetitions);
    for (auto& key : keys)
        key = generate_sample(key_size);

    std::vector<EVP_CIPHER_CTX*> aes_ctxs(repetitions, NULL);
    for (uint32_t j = 0; j < aes_ctxs.size(); ++j)
    {
        aes_ctxs[j] = EVP_CIPHER_CTX_new();
        if (!aes_ctxs[j]) std::cout << "Encryption CTX error!" << std::endl;

    }
    std::vector<std::vector<uint8_t>> ciphertexts(repetitions);
    for (uint32_t j = 0; j < ciphertexts.size(); ++j)
        ciphertexts[j].resize(messages[j].size());

    // Ecryption
    chacha20_enc_times = measure_time(repetitions, [&aes_ctxs, &cipher](std::vector<std::vector<uint8_t>>& messages_vec, std::vector<std::vector<uint8_t>>& nonce_vec, std::vector<std::vector<uint8_t>>& keys_vec, std::vector<uint8_t>* ciphertext, uint32_t idx) {
        int len;
        int ciphertext_len;

        // Initialize encryption operation
        if (EVP_EncryptInit_ex(aes_ctxs[idx], cipher, NULL, keys_vec[idx].data(), nonce_vec[idx].data()) != 1)
            std::cout << "Encryption init error!" << std::endl;;

        // Encrypt the plaintext
        if (EVP_EncryptUpdate(aes_ctxs[idx], ciphertext->data(), &len, messages_vec[idx].data(), messages_vec[idx].size()) != 1)
            std::cout << "Encryption update error!" << std::endl;;
        ciphertext_len = len;
        ciphertext->resize(ciphertext_len);
        }, ciphertexts, messages, nonces, keys);



    for (uint32_t j = 0; j < aes_ctxs.size(); ++j)
    {
        EVP_CIPHER_CTX_free(aes_ctxs[j]);
        aes_ctxs[j] = EVP_CIPHER_CTX_new();
        if (!aes_ctxs[j]) std::cout << "Decryption CTX error!" << std::endl;
    }
    std::vector<std::vector<uint8_t>> plaintexts(repetitions);
    for (uint32_t j = 0; j < plaintexts.size(); ++j)
        plaintexts[j].resize(messages[j].size());

    // Decryption
    chacha20_dec_times = measure_time(repetitions, [&aes_ctxs, &cipher](std::vector<std::vector<uint8_t>>& ciphertexts_vec, std::vector<std::vector<uint8_t>>& ivs_vec, std::vector<std::vector<uint8_t>>& keys_vec, std::vector<uint8_t>* plaintext, uint32_t idx) {
        int len;
        int ciphertext_len;

        // Initialize encryption operation
        if (EVP_DecryptInit_ex(aes_ctxs[idx], cipher, NULL, keys_vec[idx].data(), ivs_vec[idx].data()) != 1)
            std::cout << "Decryption init error!" << std::endl;

        // Encrypt the plaintext
        if (EVP_DecryptUpdate(aes_ctxs[idx], plaintext->data(), &len, ciphertexts_vec[idx].data(), ciphertexts_vec[idx].size()) != 1)
            std::cout << "Decryption update error!" << std::endl;

        }, plaintexts, ciphertexts, nonces, keys);

    // Comparison
    for (uint32_t j = 0; j < plaintexts.size(); ++j)
        if (!compare_vectors(plaintexts[j], messages[j]))
            std::cout << "Comparison failed for ChaCha20 (key_sz, msg_id): (" << key_size << ", " << j << ")" << std::endl;

    write_times_to_file(chacha20_enc_times, "results/ChaCha20_encryption_256.txt");
    write_times_to_file(chacha20_dec_times, "results/ChaCha20_decryption_256.txt");
}
