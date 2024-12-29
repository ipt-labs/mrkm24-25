//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_BUILTINGEN_H
#define ASYMCRYPTOLABS_BUILTINGEN_H

#include <random>

class BuiltinGen {
public:
    vector<uint64_t> generator(const uint32_t n)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        vector<uint64_t> y;

        std::uniform_int_distribution<> distribution(0, 1);
        uint64_t out = 0;
        size_t j = 0;

        for (int i = 0; i < n; ++i) {
            uint64_t randomBit = distribution(gen);
            out |= randomBit << (63 - j);
            ++j;
            if(j % 64 == 0)
            {
                y.push_back(out);
                j = 0;
                out = 0;
            }
        }

        return y;
    }
};


#endif //ASYMCRYPTOLABS_BUILTINGEN_H
