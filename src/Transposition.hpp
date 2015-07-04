
#ifndef TRANSPOSITION_HPP
#define TRANSPOSITION_HPP

#include "StringCipherWithPermutationKey.hpp"

#include <algorithm>
#include <memory>

namespace CryptoGL
{
    /* Abstract class for all transposition cipher types. */
    class Transposition : public StringCipherWithPermutationKey
    {
    protected:
       using Table = std::vector<ClassicalType>;

       Transposition() {}
       explicit Transposition(const KeyType &key) { setKey(key); }
       virtual ~Transposition() {}

       virtual ClassicalType encode(const ClassicalType &) = 0;
       virtual ClassicalType decode(const ClassicalType &) = 0;

       virtual Table createTable(ClassicalType data);
       virtual Table createIncompleteTable(const ClassicalType &data);
       virtual ClassicalType readPermutedTable(const Table &table);
    };

    class TranspositionCompleteRows : public Transposition
    {
    public:
       explicit TranspositionCompleteRows(const KeyType &key) 
          : Transposition(key) {}

       ClassicalType encode(const ClassicalType &clear_text) override
       {
          return readPermutedTable(createTable(clear_text));
       }

       ClassicalType decode(const ClassicalType &cipher_text) override
       {
          return encode(cipher_text);
       }

    private:
       ClassicalType readPermutedTable(const Table &table) override;
    };

    class TranspositionIncompleteRows : public Transposition
    {
    public:
       explicit TranspositionIncompleteRows(const KeyType &key)
           : Transposition(key) {}

       ClassicalType encode(const ClassicalType &clear_text) override
       {
          return readPermutedTable(createIncompleteTable(clear_text));
       }

       ClassicalType decode(const ClassicalType &cipher_text) override
       {
          return encode(cipher_text);
       }

    private:
       ClassicalType readPermutedTable(const Table &table) override;
    };

    class TranspositionCompleteColumns : public Transposition
    {
    public:
       explicit TranspositionCompleteColumns(const KeyType &key)
           : Transposition(key) {}

       ClassicalType encode(const ClassicalType &clear_text) override
       {
          return readPermutedTable(Transposition::createTable(clear_text));
       }

       ClassicalType decode(const ClassicalType &cipher_text) override
       {
          return Transposition::readPermutedTable(createTable(cipher_text));
       }

    private:
       Table createTable(ClassicalType data) override;
       ClassicalType readPermutedTable(const Table &table);
    };

    class TranspositionIncompleteColumns : public Transposition
    {
    public:
       explicit TranspositionIncompleteColumns(const KeyType &key)
           : Transposition(key) {}

       ClassicalType encode(const ClassicalType &clear_text) override
       {
          return readPermutedTable(Transposition::createIncompleteTable(clear_text));
       }

       ClassicalType decode(const ClassicalType &cipher_text) override
       {
          return Transposition::readPermutedTable(createIncompleteTable(cipher_text));
       }

    private:
       Table createIncompleteTable(const ClassicalType &data) override;
       ClassicalType readPermutedTable(const Table &table) override;
    };


    class TranspositionDouble : public Transposition
    {
    private:
       KeyType second_key;

    public:
       /* Constructor receives 2 keys : 'first_key' for the first encryption key. */
       TranspositionDouble(const KeyType &first_key, const KeyType &second_key) 
           : Transposition(first_key), second_key(second_key) {}

       ClassicalType encode(const ClassicalType &clear_text) override
       {
          TranspositionIncompleteColumns TIC_first(key);
          const ClassicalType first_encoded = TIC_first.encode(clear_text);

          TranspositionIncompleteColumns TIC(second_key);

          return TIC.encode(first_encoded);
       }

       ClassicalType decode(const ClassicalType &cipher_text) override
       {
          TranspositionIncompleteColumns TIC_second(second_key);
          const ClassicalType first_decoded = TIC_second.decode(cipher_text);

          TranspositionIncompleteColumns TIC(key);

          return TIC.decode(first_decoded);
       }
    };
}

#endif