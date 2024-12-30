//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_GEFFE_H
#define ASYMCRYPTOLABS_GEFFE_H

#include <iostream>
#include <vector>

using std::vector;

class Geffe
{
    uint64_t x, y, s;


    void L11()
    {
        uint64_t l = x ^ ((x >> 2) & 1);
        x = (x >> 1) ^ (l << 10);
    }

    void L9()
    {
        uint64_t l = y ^ ((y >> 1) & 1) ^ ((y >> 3) & 1) ^ ((y >> 4) & 1);
        y = (y >> 1) ^ (l << 8);
    }

    void L10()
    {
        uint64_t l = s ^ ((s >> 3) & 1);
        s = (s >> 1) ^ (l << 9);
    }

    u_int64_t F(const u_int64_t &xo, const u_int64_t& yo, const u_int64_t& so)
    {
        return (so & xo) ^ ((1 ^ so) & yo);
    }

public:
    vector<uint64_t> generate(const uint64_t& x0, const uint64_t& y0, const uint64_t& s0, const uint64_t n)
    {
        vector<uint64_t> r;
        x = x0; y = y0; s = s0;
        uint64_t AZOV = 0;
        size_t j = 0;
        for(uint64_t i = 0; i < n; ++i)
        {

            AZOV |= (F(x & 1, y & 1, s & 1) << (63 - j));
            ++j;
            if(j % 64 == 0)//!!!!!!!!! check if right 64 for this variant, also check this if in lehmer
            {
                j = 0;
                r.push_back(AZOV);
                AZOV = 0;
            }

            L11();
            L10();
            L9();
        }



        return r;
    }





};
#endif //ASYMCRYPTOLABS_GEFFE_H
