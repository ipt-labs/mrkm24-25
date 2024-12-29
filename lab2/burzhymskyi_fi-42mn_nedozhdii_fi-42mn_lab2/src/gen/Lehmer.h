//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_LEHMERLOW_H
#define ASYMCRYPTOLABS_LEHMERLOW_H

#include <vector>

using std::vector;

class Lehmer
{
private:
    const uint64_t a = (1 << 16) + 1;
    const uint64_t c = 119;
    static uint64_t truncLowBits(const uint64_t& t)
    {
        return uint64_t (t & 255);
    }

    static uint64_t truncHighBits(const uint64_t& t)
    {
        return uint64_t((t>>24) & 255);
    }

public:
    vector<uint64_t> generateLow(uint64_t x0, const uint32_t n)
    {
        vector<uint64_t> y;
        size_t j = 0;
        uint64_t x_c;
        uint64_t tmp = 0;
        uint64_t m = (uint64_t)1 << 32;
        for(size_t i = 0; i < n; ++i)
        {
            x_c = (a * x0 + c) % m;
            tmp |= truncLowBits(x_c) << (j*8);
            //tmp |= x_c << j*32;
            j++;
            x0 = x_c;
            if(j % 8 == 0)
            {
                y.push_back(tmp);
                tmp = 0;
                j = 0;
            }
        }
        std::reverse(y.begin(),y.end());
        return y;
    }

    vector<uint64_t> generateHigh(uint64_t x0, const uint32_t n)
    {
        vector<uint64_t> y;
        size_t j = 0;
        uint64_t x_c;
        uint64_t tmp = 0;
        uint64_t m = (uint64_t)1 << 32;
        for(size_t i = 0; i < n; ++i)
        {
            x_c = (a * x0 + c) % m;
            tmp |= truncHighBits(x_c) << (j*8);// make normal order of bits? now it pushes in reversed variant, but we need continuous flow of bits
            j++;
            x0 = x_c;
            if(j % 8 == 0)
            {
                y.push_back(tmp);
                tmp = 0;
                j = 0;
            }
        }
        std::reverse(y.begin(),y.end());

        return y;
    }

};

#endif //ASYMCRYPTOLABS_LEHMERLOW_H
