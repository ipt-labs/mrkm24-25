//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_WOLFRAM_H
#define ASYMCRYPTOLABS_WOLFRAM_H

#include <iostream>
#include <vector>

using std::vector;

class Wolfram
{
private:
    static uint32_t cycleLeft(uint64_t x)
    {
        uint32_t carry = 1 & (x >> 31);
        x = (x << 1) | carry ;
        return x;
    }

    static uint32_t cycleRight(uint32_t x)
    {
        uint32_t carry = 1 & x;
        x = (x >> 1) | (carry << 31);
        return x;
    }

public:

    vector<uint64_t> generate(const uint32_t n, const uint32_t r0)
    {
        vector<uint64_t> y;
        uint64_t out = 0;
        uint32_t r = r0;
        size_t j = 0;
        for(size_t i = 0; i < n; ++i)
        {
            r = cycleLeft(r) ^ (r | cycleRight(r));
            out |= (uint64_t(r % 2) << (63 - j));
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



#endif //ASYMCRYPTOLABS_WOLFRAM_H
