/*
 * Manage String cipher keys of String type.
 */
#ifndef STRINGCIPHERWITHSTRINGKEY_HPP
#define STRINGCIPHERWITHSTRINGKEY_HPP

#include "StringCipher.hpp"

#include <string>
#include <vector>

namespace CryptoGL
{
    class StringCipherWithStringKey : public StringCipher
    {
    public:
       using KeyType = String;

       KeyType getKey() const noexcept { return key; }
       virtual void setKey(const KeyType &key);

    protected:
       StringCipherWithStringKey() {}
       virtual ~StringCipherWithStringKey() {}

       void checkKey(const KeyType &key) const;

    private:   
       KeyType key;
    };
}

#endif