
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.hpp"

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
      this->key = key;
   }
   
protected:
   virtual void setTable(const ClassicalType &data) = 0;
   virtual std::string read(const std::vector<ClassicalType>&) const = 0;
   
   void setStartingTable(const ClassicalType &data);
   const std::map<char, unsigned int> sortKey() const;
   std::vector<ClassicalType> swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const;
   std::vector<ClassicalType> swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const;
   ClassicalType readFinalTable(const std::vector<ClassicalType>&) const;
   
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
   
   ClassicalType read(const std::vector<ClassicalType> &s_table) const
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
      unsigned int key_len = key.length();
      unsigned int data_len = data.length();
      unsigned int table_size = data_len / key_len;

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
   
   ClassicalType read(const std::vector<ClassicalType> &s_table) const
   {
      unsigned int key_len = key.length();
      ClassicalType data = "";
      data.reserve(s_table.size() * key_len);
      for (unsigned int i = 0; i < key_len; i++)
      {
         for (auto str : s_table)
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
   void setKeyRow(std::vector<unsigned int>& key_row)
   {
      this->key_row = key_row;
   }
   
   ClassicalType read(const std::vector<ClassicalType> &s_table) const
   {
      std::vector<ClassicalType> col_table;
      for (auto row : key_row)
      {
         col_table.push_back(s_table[row]);
      }

      return readFinalTable(col_table);
   }

   void setTable(const ClassicalType &data)
   {
      setStartingTable(data);
      unsigned int key_len = key_row.size();
      std::vector<ClassicalType> tmp;

      for (unsigned int i = 0; i < key_len; i++)
      {
         auto pos = std::find(key_row.begin(), key_row.end(), i);
         tmp.push_back(table[*pos]);
      }
      table = tmp;
   }
   
private:
   std::vector<unsigned int> key_row;
};

#endif