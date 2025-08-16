#pragma once

#include "../common/SquareCipher.hpp"
#include "../common/CipherGrid.hpp"

namespace CryptoGL
{

    class Adfgvx : public SquareCipher
    {
    public:

        explicit Adfgvx(const KeyType &key, const ClassicalType &grid_key) 
            : grid_key(grid_key)
        {
            setKey(key);
        }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
        Int32Vector getPermutationKey() const;

        const CipherGrid grid_key;

        static const ClassicalType code;
    };
}