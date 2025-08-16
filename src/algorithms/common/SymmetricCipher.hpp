/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#pragma once

#include "Cipher.hpp"
#include "../../core/exceptions/BadKeyLength.hpp"

namespace CryptoGL
{
    template <typename Type>
    class SymmetricCipher : public Cipher<Type>
    {
    protected:
        virtual Type encode(const Type &) override = 0;
        virtual Type decode(const Type &) override = 0;
        
        /* Check the key provided by the user and set it if the key is valid. */
        virtual void setKey(const Type &key)
        {
            const uint16_t key_len = key.size();
            if (key_len != 16)
            {
                throw BadKeyLength("Your key has to be 16 bytes length.", key_len);
            }

            this->key = key;
        }

        Type key;
    };
}