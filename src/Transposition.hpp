
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
   const ClassicalType readFinalTable(const std::vector<ClassicalType> &table);

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
            //const uint8_t pos = std::find(key.begin(), key.end(), j) - key.begin();
            s_table[i] += table[i][key[j]];
         }
      }

      return s_table;
   }

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

// Chiffre à transposition double : permutation des colonnes puis des lignes du tableau.

class TranspositionDouble : public Transposition
{
private:
   std::vector<uint32_t> key_row;

   const std::vector<ClassicalType> swapRows(const std::vector<ClassicalType> &table)
   {
      std::vector<ClassicalType> s_table(table.size(), "");
      const uint32_t key_len = key_row.size();
      
      for (uint32_t j = 0; j < key_len; ++j)
      {
         const uint32_t pos = std::find(key_row.begin(), key_row.end(), j) - key_row.begin();
         s_table[j] = table[pos];
      }

      return s_table;
   }
   
   const std::vector<ClassicalType> swapRowsEncode(const std::vector<ClassicalType> &table)
   {
      std::vector<ClassicalType> s_table(table.size(), "");
      const uint32_t key_len = key_row.size();
      
      for (uint32_t j = 0; j < key_len; ++j)
      {
         s_table[j] = table[key_row[j]];
      }

      return s_table;
   }
   
   const std::vector<ClassicalType> swapColumnsDecode(const std::vector<ClassicalType> &table)
   {
      return swapColumns(table);
   }

   const ClassicalType read(const std::vector<ClassicalType> &table)
   {
      std::vector<ClassicalType> s_table(swapRowsEncode(table));

      return readFinalTable(s_table);
   }

   const std::vector<ClassicalType> setTable(const ClassicalType &data)
   {
      std::vector<ClassicalType> table(setStartingTable(data));
      std::vector<ClassicalType> s_table(swapRows(table));

      return s_table;
   }

public:

   void setKeyRow(const std::vector<uint32_t> &key_row)
   {
      if (key_row.size() != key.size())
      {
         throw BadKeyLength("Your rows key have to be the same length as the columns key.", key_row.size());
      }

      this->key_row = key_row;
   }
};

#endif