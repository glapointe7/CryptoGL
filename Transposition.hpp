
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class Transposition : public StringCipher
{
public:
   typedef String Key;

   const String encode(const String &clear_text)
   {
      setStartingTable(clear_text);
      const std::map<char, unsigned int> sorted_key = sortKey();
      const std::vector<String> s_table = swapColumnsEncode(sorted_key);
      return read(s_table);
   }
   
   const String decode(const String &cipher_text)
   {
      setTable(cipher_text);
      const std::map<char, unsigned int> sorted_key = sortKey();
      const std::vector<String> s_table = swapColumnsEncode(sorted_key);
      return readFinalTable(s_table);
   }
   
   void setKey(const Key &key)
   {
      this->key = key;
   }
   
   void setKeyRow(std::vector<unsigned int>& key_row)
   {
      this->key_row = key_row;
   }
   
protected:
   virtual void setTable(const std::string &data) = 0;
   virtual std::string read(const std::vector<std::string>&) const = 0;
   
   void setStartingTable(const std::string &data);
   std::map<char, unsigned int> sortKey() const;
   std::vector<std::string> swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const;
   std::vector<std::string> swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const;
   std::string readFinalTable(const std::vector<std::string>&) const;
   
   std::string key;
   std::vector<unsigned int> key_row;
   std::vector<std::string> table;
};

// Transposition en ligne : permutation des colonnes du tableau et lecture en ligne.

class TranspositionRows : public Transposition
{
public:
   void setTable(const std::string &data)
   {
      setStartingTable(data);
   }
   
   std::string read(const std::vector<std::string> &s_table) const
   {
      return readFinalTable(s_table);
   }
};

// Transposition en ligne : permutation des colonnes du tableau et lecture en colonne.
class TranspositionColumns : public Transposition
{
public:
   void setTable(const std::string &data)
   {
      unsigned int key_len = key.length();
      unsigned int data_len = data.length();
      unsigned int table_size = data_len / key_len;

      for (unsigned int i = 0; i < key_len; i++)
      {
         std::string row;
         row.reserve(key_len);
         for (unsigned int j = 0; j < data_len; j += table_size)
         {
            row += data[j];
         }
         table.push_back(row);
      }
   }
   
   std::string read(const std::vector<std::string> &s_table) const
   {
      unsigned int key_len = key.length();
      std::string data = "";
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
   std::string read(const std::vector<std::string> &s_table) const
   {
      std::vector<std::string> col_table;
      for (auto row : key_row)
      {
         col_table.push_back(s_table[row]);
      }

      return readFinalTable(col_table);
   }

   void setTable(const std::string &data)
   {
      setStartingTable(data);
      unsigned int key_len = key_row.size();
      std::vector<std::string> tmp;

      for (unsigned int i = 0; i < key_len; i++)
      {
         auto pos = std::find(key_row.begin(), key_row.end(), i);
         tmp.push_back(table[*pos]);
      }
      table = tmp;
   }
};

#endif	/* TRANSPOSITIONBASE_HPP */

