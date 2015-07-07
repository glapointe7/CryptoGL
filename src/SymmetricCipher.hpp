/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#ifndef SYMMETRICCIPHER_HPP
#define SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include "exceptions/BadKeyLength.hpp"

namespace CryptoGL
{
    class SymmetricCipher : public Cipher<BytesVector>
    {
    protected:
        virtual ~SymmetricCipher() { }

        virtual BytesVector encode(const BytesVector &) override = 0;
        virtual BytesVector decode(const BytesVector &) override = 0;
        
        /* Check the key provided by the user and set it if the key is valid. */
        virtual void setKey(const BytesVector &key)
        {
            const uint8_t key_len = key.size();
            if (key_len != 16)
            {
                throw BadKeyLength("Your key has to be 16 bytes length.", key_len);
            }

            this->key = key;
        }

        BytesVector key;
    };
}

#endif