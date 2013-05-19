
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"
#include "exceptions/BadKeyLength.hpp"

#include <algorithm>
#include <string>
#include <vector>

class Transposition : public StringCipher
{
public:

   const ClassicalType encode(const ClassicalType &clear_text)
   {
      const std::vector<ClassicalType> table(setStartingTable(clear_text));
      const std::vector<ClassicalType> s_table(swapColumns(table));

      return read(s_table);
   }

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      const std::vector<ClassicalType> table(setTable(cipher_text));
      const std::vector<ClassicalType> s_table(swapColumnsDecode(table));

      return readFinalTable(s_table);
   }

   void setKey(const std::vector<uint8_t> &key)
   {
      if (key.empty())
      {
         throw EmptyKey("Your key is empty.");
      }

      this->key = key;
   }

protected:
   virtual const std::vector<ClassicalType> setTable(const ClassicalType &data) = 0;
   virtual const ClassicalType read(const std::vector<ClassicalType> &table) = 0;
   virtual const std::vector<ClassicalType> swapColumnsDecode(const std::vector<ClassicalType> &table) = 0;
   
   const std::vector<ClassicalType> setStartingTable(const ClassicalType &data);
   const std::vector<ClassicalType> swapColumns(const std::vector<ClassicalType> &table);
   const ClassicalType readFinalTable(const std::vector<ClassicalType> &table) const;

   std::vector<uint8_t> key;
};

// Transposition en ligne : permutation des colonnes du tableau et lecture en ligne.

class TranspositionRows : public Transposition
{
public:

   const std::vector<ClassicalType> setTable(const ClassicalType &data)
   {
      return setStartingTable(data);
   }
   
   const std::vector<ClassicalType> swapColumnsDecode(const std::vector<ClassicalType> &table)
   {
      return swapColumns(table);
   }

   const ClassicalType read(const std::vector<ClassicalType> &table)
   {
      return readFinalTable(table);
   }
};

// Transposition en colonnes : permutation des colonnes du tableau et lecture en colonne.

class TranspositionColumns : public Transposition
{
public:
// ICI : incomplète => on ajoute pas de X => on doit connaître datalen % key_len
// et faire attention à la dernière ligne.
   const std::vector<ClassicalType> setTable(const ClassicalType &data)
   {
      const uint8_t key_len = key.size();
      const uint32_t data_len = data.length();
      const uint32_t rows = data_len / key_len;
      std::vector<ClassicalType> table(rows, "");

      for (uint32_t i = 0; i < rows; i++)
      {
         for (uint32_t j = i; j < data_len; j += rows)
         {
            table[i].reserve(key_len);
            table[i] += data[j];
         }
      }

      return table;
   }
// ICI : incomplète => on ajoute pas de X => on doit connaître datalen % key_len
// et faire attention à la dernière ligne.
   const std::vector<ClassicalType> swapColumnsDecode(const std::vector<ClassicalType> &table)
   {
      const uint8_t key_len = key.size();
      const unsigned int rows = table.size();
      std::vector<ClassicalType> s_table(rows, "");

      for (unsigned int i = 0; i < rows; ++i)
      {
         s_table[i].reserve(key_len);
         for (uint8_t j = 0; j < key_len; ++j)
         {
            s_table[i] += table[i][key[j]];
         }
      }

      return s_table;
   }
// ICI : incomplète => on ajoute pas de X => on doit connaître datalen % key_len
// et faire attention à la dernière ligne.
   const ClassicalType read(const std::vector<ClassicalType> &table)
   {
      const uint8_t key_len = key.size();
      ClassicalType data = "";
      data.reserve(table.size() * key_len);

      for (uint8_t i = 0; i < key_len; i++)
      {
         for (const auto str : table)
         {
            data += str[i];
         }
      }

      return data;
   }
};

// Double columns transposition.

class TranspositionDouble : public StringCipher
{
private:
   std::vector<uint8_t> second_key;
   std::vector<uint8_t> key;

public:

   void setKey(const std::vector<uint8_t> &key)
   {
      this->key = key;
   }
   
   void setSecondKey(const std::vector<uint8_t> &second_key)
   {
      this->second_key = second_key;
   }
   
   const ClassicalType encode(const ClassicalType &clear_text)
   {
      TranspositionColumns *TC = new TranspositionColumns();
      TC->setKey(key);
      const ClassicalType first_encoded = TC->encode(clear_text);
      TC->setKey(second_key);
      
      return TC->encode(first_encoded);
   }
   
   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      TranspositionColumns *TC = new TranspositionColumns();
      TC->setKey(second_key);
      const ClassicalType first_decoded = TC->decode(cipher_text);
      TC->setKey(key);
      
      return TC->decode(first_decoded);
   }
};

#endif
