//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_BBS_H
#define ASYMCRYPTOLABS_BBS_H

#include <iostream>
#include <vector>
#include <gmpxx.h>

using std::vector;

class BSS
{
    mpz_t a, p, q, n;


public:
    BSS()
    {
        mpz_init(p);
        mpz_init(q);
        mpz_init(n);
        mpz_set_str(p, "D5BBB96D30086EC484EBA3D7F9CAEB07", 16);
        mpz_set_str(q, "425D2B9BFDB25B9CF6C416CC6E37B59C1F", 16);
        mpz_mul(n, p, q);
    }
    ~BSS()
    {
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(n);
    }
    vector<uint64_t> generate(const uint32_t& nBites)
    {
        vector<uint64_t> y;
        mpz_t r,x;
        mpz_t res;
        mpz_init(r);
        mpz_init(x);

        gmp_randstate_t rand_state;
        gmp_randinit_mt(rand_state);
        unsigned long seed = time(NULL);
        gmp_randseed_ui(rand_state, seed);
        mpz_urandomm(r, rand_state, p);
        size_t j = 0;
        uint64_t out = 0;
        for(size_t i = 0; i < nBites; ++i)
        {
            mpz_add_ui(r, r, 2);
            mpz_mul(r, r, r);
            mpz_mod(r, r, n);
            mpz_mod_ui(x, r, 2);
            uint64_t bit = mpz_get_ui(x);
            out = out | (bit << (63 - j));
            ++j;
            if(j % 64 == 0)
            {
                y.push_back(out);
                out = 0;
                j = 0;
            }
        }

        mpz_clear(r);
        mpz_clear(x);
        return y;
    }

    vector<uint64_t> generate256(const uint32_t& nBites)
    {
        vector<uint64_t> y;
        mpz_t r,x;
        mpz_t res;
        mpz_init(r);
        mpz_init(x);

        gmp_randstate_t rand_state;
        gmp_randinit_mt(rand_state);
        unsigned long seed = time(NULL);
        gmp_randseed_ui(rand_state, seed);
        mpz_urandomm(r, rand_state, p);
        size_t j = 0;
        uint64_t out = 0;
        for(size_t i = 0; i < nBites; ++i)
        {
            mpz_add_ui(r, r, 2);
            mpz_mul(r, r, r);
            mpz_mod(r, r, n);
            mpz_mod_ui(x, r, 256);
            uint64_t bit = mpz_get_ui(x);
            out = out | (bit << j*8);
            ++j;
            if(j % 8 == 0)
            {
                y.push_back(out);
                out = 0;
                j = 0;
            }
        }

        mpz_clear(r);
        mpz_clear(x);
        std::reverse(y.begin(), y.end());
        return y;
    }



};


#endif //ASYMCRYPTOLABS_BBS_H
