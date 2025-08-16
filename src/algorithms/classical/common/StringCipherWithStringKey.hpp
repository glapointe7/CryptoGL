/*
 * Manage String cipher keys of String type.
 */
#pragma once

#include "StringCipher.hpp"

namespace CryptoGL
{
    class StringCipherWithStringKey : public StringCipher
    {
    public:
        using KeyType = String;
        
        KeyType getKey() const
        {
            return key;
        }
        
        virtual void setKey(const KeyType &key);

    protected:
        StringCipherWithStringKey() { }

        void checkKey(const KeyType &key) const;

    private:
        KeyType key;
    };
}