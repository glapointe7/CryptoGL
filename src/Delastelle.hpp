
#ifndef DELASTELLE_HPP
#define DELASTELLE_HPP

#include "SquareCipher.hpp"
#include <string>

#include "exceptions/Exception.hpp"

namespace CryptoGL
{
    class Delastelle : public SquareCipher
    {
    public:
       Delastelle(const KeyType &key, const ClassicalType &alpha, const uint32_t block_length)
          : SquareCipher(key, alpha), block_len(block_length) {}

       Delastelle(const KeyType &key, const uint32_t block_length)
          : Delastelle(key, ClassicalType::grid_uppercase_fr, block_length) {}

       explicit Delastelle(const KeyType &key) : Delastelle(key, 5) {}

       ClassicalType encode(const ClassicalType &clear_text) override;
       ClassicalType decode(const ClassicalType &cipher_text) override;

       void setBlockLength(const uint32_t block_len);

    private:
       using ZeroBlockLength = Exception;

       uint32_t block_len;
    };
}

#endif