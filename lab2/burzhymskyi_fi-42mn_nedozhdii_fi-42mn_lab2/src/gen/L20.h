//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_L20_H
#define ASYMCRYPTOLABS_L20_H
#include <iostream>
#include <vector>

using std::vector;


class L20
{
    uint64_t x;

    uint64_t genState()
    {
        uint64_t l = ((x >> 3) & 1) ^ ((x >> 5) & 1) ^ ((x >> 9) & 1) ^ (x & 1) ;
        uint64_t out = x & 1;
        x = (x >> 1) ^ (l << 19);
        return out;
    }

public:
    vector<uint64_t> generate(const uint64_t x0, const uint64_t n)
    {
        vector<uint64_t> y;
        x = x0;
        size_t j = 0;
        uint64_t out = 0;
        for(uint64_t i = 0; i < n; ++i)
        {
            out = out | (genState() << (63-j));
            ++j;
            if(j % 64 == 0)//!!!!!!!!! check if right 64 for this variant, also check this if in lehmer
            {
                y.push_back(out);
                j = 0;
                out = 0;
            }
        }



        return y;
    }

};


#endif //ASYMCRYPTOLABS_L20_H
