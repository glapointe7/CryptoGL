
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"
#include "exceptions/BadKeyLength.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class Transposition : public StringCipher
{
public:
   typedef ClassicalType Key;

   const ClassicalType encode(const ClassicalType &clear_text)
   {
      setStartingTable(clear_text);
      const std::map<char, unsigned int> sorted_key = sortKey();
      const std::vector<ClassicalType> s_table = swapColumnsEncode(sorted_key);
      return read(s_table);
   }
   
   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      setTable(cipher_text);
      const std::vector<ClassicalType> s_table = swapColumnsDecode(sortKey());
      return readFinalTable(s_table);
   }
   
   void setKey(const Key &key)
   {
      if(key.empty())
      {
         throw EmptyKey("Your key is empty.");
      }
      
      const char c = badAlphaFound(key);
      if(c != 0)
      {
         throw BadChar("Your key contains at least one character that is not in your alphabet.", c);
      }
      
      this->key = key;
   }
   
protected:
   virtual void setTable(const ClassicalType &data) = 0;
   virtual const ClassicalType read(const std::vector<ClassicalType>&) const = 0;
   
   void setStartingTable(const ClassicalType &data);
   const std::map<char, unsigned int> sortKey() const;
   const std::vector<ClassicalType> swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const;
   const std::vector<ClassicalType> swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const;
   const ClassicalType readFinalTable(const std::vector<ClassicalType>&) const;
   
   Key key;
   std::vector<ClassicalType> table;
};

// Transposition en ligne : permutation des colonnes du tableau et lecture en ligne.

class TranspositionRows : public Transposition
{
public:
   void setTable(const ClassicalType &data)
   {
      setStartingTable(data);
   }
   
   const ClassicalType read(const std::vector<ClassicalType> &s_table) const
   {
      return readFinalTable(s_table);
   }
};

// Transposition en colonnes : permutation des colonnes du tableau et lecture en colonne.
class TranspositionColumns : public Transposition
{
public:
   void setTable(const ClassicalType &data)
   {
      const unsigned int key_len = key.length();
      const unsigned int data_len = data.length();
      const unsigned int table_size = data_len / key_len;

      for (unsigned int i = 0; i < table_size; i++)
      {
         std::string row;
         row.reserve(key_len);
         for (unsigned int j = i; j < data_len; j += table_size)
         {
            row += data[j];
         }
         table.push_back(row);
      }
   }
   
   const ClassicalType read(const std::vector<ClassicalType> &s_table) const
   {
      const unsigned int key_len = key.length();
      ClassicalType data = "";
      data.reserve(s_table.size() * key_len);
      for (unsigned int i = 0; i < key_len; i++)
      {
         for (const auto str : s_table)
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
public:
   void setKeyRow(const std::vector<uint8_t> &key_row)
   {
      if(key_row.size() != key.length())
      {
         throw BadKeyLength("Your rows key have to be the same length as the columns key.", key_row.size());
      }
      
      this->key_row = key_row;
   }
   
   const ClassicalType read(const std::vector<ClassicalType> &s_table) const
   {
      std::vector<ClassicalType> col_table;
      col_table.reserve(s_table.size());
      
      for (const auto row : key_row)
      {
         col_table.push_back(s_table[row-1]);
      }

      return readFinalTable(col_table);
   }

   void setTable(const ClassicalType &data)
   {
      setStartingTable(data);

      uint32_t i = 0;
      for (const auto pos : key_row)
      {
         std::swap(table[i], table[pos-1]);
         i++;
      }
   }
   
private:
   std::vector<uint8_t> key_row;
};

#endif