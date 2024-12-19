#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS


#include <chrono>
#include <iostream>
#include <format>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define to_ms(time) std::chrono::duration_cast<std::chrono::microseconds>((time)).count() 
#define to_sec(time) float(to_ms(time)) / float(1000'000)

template <typename... Args>
std::vector<float> measure_time(uint32_t iterations, auto Function, auto& result_collection, Args... args)
{
    std::vector<float> time_measurments(iterations, 0.f);
    std::chrono::steady_clock::time_point start, stop;

    for (uint32_t i = 0; i < iterations; ++i)
    {
        start = std::chrono::high_resolution_clock::now();
        Function(args..., &result_collection[i], i);
        stop = std::chrono::high_resolution_clock::now();
        time_measurments[i] = to_sec(stop - start);
    }

    return time_measurments;
}

extern void print_PKEY_to_BIO(EVP_PKEY*);

extern std::vector<uint8_t> generate_sample(uint32_t size_bytes);

extern void write_times_to_file(const std::vector<float>& times, const std::string& filename);