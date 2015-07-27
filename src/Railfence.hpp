/*
 * Need review to improve code for optimization.
 * Need more comments on code.
 */
#ifndef RAILFENCE_HPP
#define RAILFENCE_HPP

#include "StringCipherWithPermutationKey.hpp"

#include <list>

namespace CryptoGL
{
    class RedefenceZigzag : public StringCipherWithPermutationKey
    {
    public:
        RedefenceZigzag(const KeyType &key, const KeyType &key_levels);

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;
        
        void setOffset(const uint32_t offset)
        {
            this->offset = offset;
        }

    private:
        /* Encode data block with the basic railfence cipher. */
        void addEncodedText(Vector<ClassicalType> &rows, const ClassicalType &data,
                const int32_t i, const int32_t level, const int32_t mod) const;

        /* Redefence : Replace each row following the permutation key. */
        ClassicalType swapRowsWithPermutationKey(const Vector<ClassicalType> &rows, const uint32_t length) const;
        Vector<std::list<int8_t> > getFirstDecoding(const ClassicalType &cipher_text, int32_t &last) const;

        uint32_t offset = 0;
        uint32_t max_level;
        KeyType key_levels;
    };

    /*
     * Railfence cipher : 
     */
    class Railfence : public RedefenceZigzag
    {
    public:
        explicit Railfence(const int32_t key)
            : RedefenceZigzag(KeyType(1, key), KeyType(1, key)) { }
    };

    class Redefence : public RedefenceZigzag
    {
    public:
        explicit Redefence(const KeyType &key)
            : RedefenceZigzag(key, KeyType(1, key.size())) { }
    };
}

#endif