//
// Created by akhzulag on 30.09.2023.
//

#ifndef ASYMCRYPTOLABS_LIBRARIAN_H
#define ASYMCRYPTOLABS_LIBRARIAN_H

#endif //ASYMCRYPTOLABS_LIBRARIAN_H

#include <vector>
#include <bitset>
#include <string>

using std::vector;
using std::bitset;
using std::string;



class Librarian
{

    uint64_t transform(char l)
    {
        uint64_t a = (uint8_t)l - (uint8_t)'a';
        return a;
    }

public:

    vector<uint64_t> generate(string input)
    {
        size_t size = 0;
        uint64_t out = 0;
        vector<uint64_t> result;
        for(uint64_t i = 0; i < input.size(); ++i)
        {
            out |= (transform(input[i]) << 8 * size);
            ++size;
            if(size % 8 == 0)
            {
                result.push_back(out);
                size = 0;
                out = 0;
            }
        }
        return result;
    }
};