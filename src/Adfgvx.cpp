
#include "Adfgvx.hpp"

#include "Transposition.hpp"

#include <algorithm>

using namespace CryptoGL;

const ClassicalType Adfgvx::CODE = "ADFGVX";

Int32Vector Adfgvx::getPermutationKey() const
{
    KeyType key = getKey();
    KeyType sorted_key(key);
    sorted_key.sort();

    Int32Vector perm_key;
    perm_key.reserve(key.length());
    for (const auto c : key)
    {
        perm_key.push_back(sorted_key.find(c));
    }

    return perm_key;
}

ClassicalType Adfgvx::encode(const ClassicalType &clear_text)
{
    // Take the coordinates of each letter and replace them by A,D,F,G,V or X such that
    // A=0, D=1, F=2, G=3, V=4, X=5. For example, if 'K' has coordinates (2,3), then
    // we encode K as FG.
    const KeyType key = getKey();
    ClassicalType first_encoding((clear_text.length() + key.length()) * 2);

    for (const auto c : clear_text)
    {
        const Coordinates coords = grid_key.getCharCoordinates(c);
        first_encoding.push_back(CODE[coords.y]);
        first_encoding.push_back(CODE[coords.x]);
    }

    TranspositionCompleteColumns TCC(getPermutationKey());

    return TCC.encode(first_encoding);
}

ClassicalType Adfgvx::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(cipher_len / 2);

    TranspositionCompleteColumns TCol(getPermutationKey());
    const ClassicalType first_decoding = TCol.decode(cipher_text);

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const auto coords = std::make_pair(CODE.find(first_decoding[i]), CODE.find(first_decoding[i + 1]));
        decrypted.push_back(grid_key.at(coords.first, coords.second));
    }

    return decrypted;
}