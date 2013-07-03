
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipherWithPermutationKey.hpp"

#include <algorithm>
#include <string>
#include <vector>

/* Abstract class for all transposition cipher types. */
class Transposition : public StringCipherWithPermutationKey
{
protected:
   typedef std::vector<ClassicalType> Table;

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   virtual const Table createTable(const ClassicalType &data);
   virtual const Table createIncompleteTable(const ClassicalType &data);
   virtual const ClassicalType readPermutedTable(const Table &table);
};

class TranspositionCompleteRows : public Transposition
{
public:
   explicit TranspositionCompleteRows(const KeyType &key) { setKey(key); }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(createTable(clear_text));

      return readPermutedTable(table);
   }

   virtual const ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createTable(cipher_text));

      return readPermutedTable(table);
   }
   
private:
   virtual const ClassicalType readPermutedTable(const Table &table) final;
};

class TranspositionIncompleteRows : public Transposition
{
public:
   explicit TranspositionIncompleteRows(const KeyType &key) { setKey(key); }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(createIncompleteTable(clear_text));

      return readPermutedTable(table);
   }

   virtual const ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createIncompleteTable(cipher_text));

      return readPermutedTable(table);
   }
   
private:
   virtual const ClassicalType readPermutedTable(const Table &table) final;
};

class TranspositionCompleteColumns : public Transposition
{
public:
   explicit TranspositionCompleteColumns(const KeyType &key) { setKey(key); }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(Transposition::createTable(clear_text));

      return readPermutedTable(table);
   }

   virtual const ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createTable(cipher_text));

      return Transposition::readPermutedTable(table);
   }

private:
   virtual const Table createTable(const ClassicalType &data) final;
   virtual const ClassicalType readPermutedTable(const Table &table);
};

class TranspositionIncompleteColumns : public Transposition
{
public:
   explicit TranspositionIncompleteColumns(const KeyType &key) { setKey(key); }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final
   {
      const Table table(Transposition::createIncompleteTable(clear_text));

      return readPermutedTable(table);
   }

   virtual const ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const Table table(createIncompleteTable(cipher_text));

      return Transposition::readPermutedTable(table);
   }

private:
   virtual const Table createIncompleteTable(const ClassicalType &data) final;
   virtual const ClassicalType readPermutedTable(const Table &table) final;
};


class TranspositionDouble : public Transposition
{
private:
   KeyType second_key;
   
   
public:
   /* Constructor receives 2 keys : 'first_key' for the first encryption key. */
   TranspositionDouble(const KeyType &first_key, const KeyType &second_key) 
   { 
      setKey(first_key); 
      this->second_key = second_key;
   }
   
   const ClassicalType encode(const ClassicalType &clear_text)
   {
      TranspositionIncompleteColumns *TIC_first = new TranspositionIncompleteColumns(getKey());
      const ClassicalType first_encoded = TIC_first->encode(clear_text);
      delete TIC_first;
      
      TranspositionIncompleteColumns *TIC_second = new TranspositionIncompleteColumns(second_key);
  
      return TIC_second->encode(first_encoded);
   }
   
   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      const KeyType key = getKey();
      TranspositionIncompleteColumns *TIC_second = new TranspositionIncompleteColumns(second_key);
      const ClassicalType first_decoded = TIC_second->decode(cipher_text);
      delete TIC_second;
      
      TranspositionIncompleteColumns *TIC_first = new TranspositionIncompleteColumns(key);
      
      return TIC_first->decode(first_decoded);
   }
};

#endif