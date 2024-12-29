//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_L89_H
#define ASYMCRYPTOLABS_L89_H
#include <bitset>

using std::bitset;
using std::vector;

const bitset<89> one = 0x1;

class L89
{
    bitset<89> x;

    uint64_t genState() {
        bitset<89> l = ((x >> 38) & one) ^ (x & one);
        uint64_t out = x[0];
        x = (x >> 1) ^ (l << 88);
        return out;
    }

public:
    vector<uint64_t> generate(const uint64_t& x0, const uint64_t& n)
    {
        vector<uint64_t> y;
        x = bitset<89>(x0);
        size_t j = 0;
        uint64_t out = 0;
        for(uint64_t i = 0; i < n; ++i)
        {
            out = out | (genState() << (63 - j));
            ++j;
            if(j % 64 == 0)// Якщо це не працює у L20, то не працює і тут
            {
                y.push_back(out);
                j = 0;
                out = 0;
            }
        }

        if(j != 0)
            y.push_back(out);

        return y;
    }

};

#endif //ASYMCRYPTOLABS_L89_H
