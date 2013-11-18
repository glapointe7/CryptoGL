
#ifndef TRANSPOSITION_HPP
#define TRANSPOSITION_HPP

#include "StringCipherWithPermutationKey.hpp"

#include <algorithm>
#include <string>
#include <vector>
#include <memory>

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
   
   virtual Table createTable(const ClassicalType &data);
   virtual Table createIncompleteTable(const ClassicalType &data);
   virtual ClassicalType readPermutedTable(const Table &table);
};

class TranspositionCompleteRows : public Transposition
{
public:
   explicit TranspositionCompleteRows(const KeyType &key) 
      : Transposition(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(createTable(clear_text));

      return readPermutedTable(table);
   }

   virtual ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createTable(cipher_text));

      return readPermutedTable(table);
   }
   
private:
   virtual ClassicalType readPermutedTable(const Table &table) final;
};

class TranspositionIncompleteRows : public Transposition
{
public:
   explicit TranspositionIncompleteRows(const KeyType &key)
       : Transposition(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(createIncompleteTable(clear_text));

      return readPermutedTable(table);
   }

   virtual ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createIncompleteTable(cipher_text));

      return readPermutedTable(table);
   }
   
private:
   virtual ClassicalType readPermutedTable(const Table &table) final;
};

class TranspositionCompleteColumns : public Transposition
{
public:
   explicit TranspositionCompleteColumns(const KeyType &key)
       : Transposition(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(Transposition::createTable(clear_text));

      return readPermutedTable(table);
   }

   virtual ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createTable(cipher_text));

      return Transposition::readPermutedTable(table);
   }

private:
   virtual Table createTable(const ClassicalType &data) final;
   virtual ClassicalType readPermutedTable(const Table &table);
};

class TranspositionIncompleteColumns : public Transposition
{
public:
   explicit TranspositionIncompleteColumns(const KeyType &key)
       : Transposition(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(Transposition::createIncompleteTable(clear_text));

      return readPermutedTable(table);
   }

   virtual ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createIncompleteTable(cipher_text));

      return Transposition::readPermutedTable(table);
   }

private:
   virtual Table createIncompleteTable(const ClassicalType &data) final;
   virtual ClassicalType readPermutedTable(const Table &table) final;
};


class TranspositionDouble : public Transposition
{
private:
   KeyType second_key;
   
public:
   /* Constructor receives 2 keys : 'first_key' for the first encryption key. */
   TranspositionDouble(const KeyType &first_key, const KeyType &second_key) 
       : Transposition(first_key), second_key(second_key) {}
   
   ClassicalType encode(const ClassicalType &clear_text)
   {
      TranspositionIncompleteColumns *TIC_first = new TranspositionIncompleteColumns(getKey());
      const ClassicalType first_encoded = TIC_first->encode(clear_text);
      delete TIC_first;
      
      /* Prevent memory leak : delete can't be used. */
      TranspositionIncompleteColumns TIC(second_key);
  
      return TIC.encode(first_encoded);
   }
   
   ClassicalType decode(const ClassicalType &cipher_text)
   {
      const KeyType key = getKey();
      TranspositionIncompleteColumns *TIC_second = new TranspositionIncompleteColumns(second_key);
      const ClassicalType first_decoded = TIC_second->decode(cipher_text);
      delete TIC_second;
      
      /* Prevent memory leak : delete can't be used. */
      TranspositionIncompleteColumns TIC(key);
      
      return TIC.decode(first_decoded);
   }
};

#endif