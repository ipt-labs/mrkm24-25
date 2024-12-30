//
// Created by akhzulag on 02.10.2023.
//

#ifndef ASYMCRYPTOLABS_TEST_H
#define ASYMCRYPTOLABS_TEST_H
#include <iostream>
#include <vector>
#include <random>
#include <boost/math/distributions/chi_squared.hpp>

using std::vector;
using boost::math::quantile;
using boost::math::chi_squared_distribution;
using std::pair;

namespace TEST
{

    bool uniformDistribution(const vector<uint64_t>& s, double alpha)
    {
        double byteV[256];
        for(size_t i = 0; i < 256; ++i)
        {
            byteV[i] = 0;
        }

        for(size_t j = 0; j < s.size(); ++j)
        {
            for(size_t i = 0; i < 8; ++i)
            {
                ++byteV[(s[j] >> (i*8)) & 255];
            }
        }

        double m = (double)s.size() * 8;
        double n = m/256;
        double ro = 0;
        for(size_t i = 0; i < 256; ++i)
        {
            ro += (byteV[i] - n)*(byteV[i] - n)/n;
        }

        double x2 = quantile(chi_squared_distribution<double>(255),1- alpha);
        std::cout<<"ro(x) = "<< ro << ", "<<"quantile"<<(alpha)<<" = "<< x2 <<" ";
        if(ro <= x2)
            return 1;
        return 0;

    }

    bool independence(const vector<uint64_t>& s, double alpha)
    {
        vector<pair<uint8_t, uint8_t>> y;
        double bytesV[256][256];

        for (size_t i = 0; i < 256; ++i)
        {
            for (size_t j = 0; j < 256; ++j)
            {
                bytesV[i][j] = 0;
            }
        }

        for(size_t i = 0; i < s.size(); ++i)
        {
            uint64_t out = s[i];
            for(size_t j = 1; j < 8; ++j)
            {
                ++bytesV[(out >> 8*(j-1)) & 255][(out >> 8*j) & 255];
            }
            if(i + 1 < s.size())
                ++bytesV[out & 255][(s[i+1] >> 56) & 255];

        }

        double v[256], a[256];
        double n = 0;
        for(size_t i = 0; i < 256; ++i)
        {
            v[i] = 0;
            a[i] = 0;
        }
        for(size_t i = 0; i < 256; ++i)
        {
            for(size_t j = 0; j < 256; ++j)
            {
                v[i] += bytesV[i][j];
                a[i] += bytesV[j][i];
                n += bytesV[i][j];
            }
        }

        double ro = 0;
        for(size_t i = 0; i < 256; ++i)
        {
            for (size_t j = 0; j < 256; ++j)
            {
                ro += (bytesV[i][j]/v[i])*(bytesV[i][j])/(a[j]);
            }
        }
        ro = (ro - 1) * n;

        double x2 = quantile(chi_squared_distribution<double>(255*255),1 - alpha);
        std::cout<<"ro(x) = "<< ro << ", "<<"quantile"<<(alpha)<<" = "<< x2 <<" ";
        if(ro <= x2)
            return 1;
        return 0;


    }

    bool uniformity(const vector<uint64_t>& s, size_t r, double alpha)
    {
        double byteV[256][r];
        for(size_t i = 0; i < 256; ++i)
        {
            for(size_t j = 0; j < r; ++j)
                byteV[i][j] = 0;
        }

        size_t m1 = s.size()*8 / r;
        size_t sSize = m1 * r;
        size_t lim = sSize / 8;
        size_t k = 0;//counter byte
        size_t ri = 0;
        for(size_t i = 0; i < lim; ++i)
        {
            for(size_t j = 0; j < 8; ++j)
            {
                ++byteV[(s[i] >> j*8) & 255][ri];
                if(k == m1)
                {
                    k = 0;
                    ++ri;
                }
                k++;
            }
        }
        double v[256], a[r];
//        double n = 0;
        for(size_t i = 0; i < 256; ++i)
        {
            v[i] = 0;
        }

        for(size_t j = 0; j < r; ++j)
        {
            a[j] = 0;
        }


        for(size_t i = 0; i < 256; ++i)
        {
            for(size_t j = 0; j < r; ++j)
            {
                v[i] += byteV[i][j];
            }
        }

        for(size_t j = 0; j < r; ++j)
        {
            for(size_t i = 0; i < 256; ++i)
            {
                a[j] += byteV[i][j];
            }
        }

        std::cout<<"sd";
        double ro = 0;
        for(size_t i = 0; i < 256; ++i)
        {
            for (size_t j = 0; j < r; ++j)
            {
                ro += (byteV[i][j]/v[i])*(byteV[i][j])/(a[j]);
            }
        }
        ro = (ro - 1) * (double)sSize;

        double x2 = quantile(chi_squared_distribution<double>(255*(r-1)),1 - alpha);
        std::cout<<"ro(x) = "<< ro << ", "<<"quantile"<<(alpha)<<" = "<< x2 <<" ";
        if(ro <= x2)
            return 1;
        return 0;
    }


};


#endif //ASYMCRYPTOLABS_TEST_H
