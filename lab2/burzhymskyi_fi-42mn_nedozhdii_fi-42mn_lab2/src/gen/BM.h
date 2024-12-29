//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_BM_H
#define ASYMCRYPTOLABS_BM_H
#include <gmp.h>
#include <iostream>
#include <vector>

using std::vector;

class BM
{
public:
    mpz_t a, p, q, r;
    BM()
    {
        mpz_init(a);
        mpz_init(p);
        mpz_init(q);
        mpz_init(r);
        mpz_set_str(a, "5B88C41246790891C095E2878880342E88C79974303BD0400B090FE38A688356", 16);
        mpz_set_str(p, "CEA42B987C44FA642D80AD9F51F10457690DEF10C83D0BC1BCEE12FC3B6093E3", 16);
        mpz_set_str(q, "675215CC3E227D3216C056CFA8F8822BB486F788641E85E0DE77097E1DB049F1", 16);

        mpz_sub_ui(r, p, 1);
        mpz_div_ui(r, r, 2);
    }

    ~BM()
    {
        mpz_clear(a);
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(r);
    }

    vector<uint64_t> generate(const uint32_t n)
    {
        vector<uint64_t> y;
        mpz_t T;
        mpz_init(T);
        gmp_randstate_t rand_state;
        gmp_randinit_mt(rand_state);
        unsigned long seed = time(NULL);
        gmp_randseed_ui(rand_state, seed);
        mpz_urandomm(T, rand_state, p);
        uint64_t out = 0;
        uint16_t j = 0;
        for(size_t i = 0; i < n; ++i)
        {
            mpz_powm(T, a, T, p);
            if(mpz_cmp(T, r) < 0)
                out |= uint64_t(1) << (63 - j);
            ++j;
            if(j % 64 == 0)
            {
                y.push_back(out);
                j = 0;
                out = 0;
            }
        }
        mpz_clear(T);
        return y;
    }

    vector<uint64_t> generate256(const uint32_t& n)
    {
        vector<uint64_t> y;
        mpz_t T,res,cmp1,cmp2;
        mpz_init(T);
        mpz_init(res);
        mpz_init(cmp1);
        mpz_init(cmp2);
        mpz_sub_ui(res, p, 1);
        mpz_div_ui(res,res,256);

        gmp_randstate_t rand_state;
        gmp_randinit_mt(rand_state);
        unsigned long seed = time(NULL);
        gmp_randseed_ui(rand_state, seed);
        mpz_urandomm(T, rand_state, p);
        uint64_t out = 0;
        uint16_t j = 0;
        for(size_t i = 0; i < n; ++i)
        {
            mpz_powm(T, a, T, p);
            uint16_t k = 0;
            while(k <= (1<<8) - 1)
            {
                mpz_mul_ui(cmp1, res, k);
                mpz_mul_ui(cmp2, res, k + 1);

                if(mpz_cmp(T, cmp1) > 0 && mpz_cmp(T, cmp2) <= 0)
                    break;
                ++k;
            }
            out |= (k<< j*8);
            if(j % 8 == 0)
            {
                y.push_back(out);
                j = 0;
                out = 0;
            }
        }
        std::reverse(y.begin(), y.end());
        mpz_clear(T);

        return y;
    }


};

#endif
