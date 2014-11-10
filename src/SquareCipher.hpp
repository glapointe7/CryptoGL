/*
 * All string ciphers that uses a square grid.
 */
#ifndef SQUARECIPHER_HPP
#define SQUARECIPHER_HPP

#include "StringCipherWithStringKey.hpp"
#include "String.hpp"
#include "CipherGrid.hpp"

#include <vector>

namespace CryptoGL
{
    class SquareCipher : public StringCipherWithStringKey
    {
    protected:   
       const CipherGrid grid;

       SquareCipher() {}

       /* Create nXn grid depending of the length of alpha which must be a perfect square. */
       SquareCipher(const KeyType &key, const ClassicalType &alpha) : grid(key, alpha)
       {
          setAlpha(alpha);
          setKey(key);
       }

       /* Default : Create 5X5 grid with unique english uppercase letters with the key. */
       explicit SquareCipher(const KeyType &key) : SquareCipher(key, ClassicalType::grid_uppercase_fr) {}

       virtual ~SquareCipher() {}

       virtual ClassicalType encode(const ClassicalType &) = 0;
       virtual ClassicalType decode(const ClassicalType &) = 0;
    };
}

#endif