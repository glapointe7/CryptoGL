
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Transposition : public StringCipher
{
public:
   Transposition();

   virtual ~Transposition()
   {
   }

   enum class Transposition_types : uint8_t
   {
      Columns = 0,
              Rows = 1,
              Double = 2
   };

   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;

   void setKey(const std::string &key);

protected:
   virtual void setTable(const std::string &data) = 0;
   virtual std::string read(const std::vector<std::string>&) const = 0;

   void setStartingTable(const std::string &data);
   std::string readFinalTable(const std::vector<std::string>&) const;

   std::map<char, unsigned int> sortKey() const;
   std::vector<std::string> swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const;
   std::vector<std::string> swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const;

   std::string getEncoded(const std::string& data, const Transposition_types type);
   std::string getDecoded(const std::string& data, const Transposition_types type);

   std::string key;
   std::vector<std::string> table;
};

// Transposition en ligne : permutation des colonnes du tableau et lecture en colonne.
class TranspositionColumns : public Transposition
{
public:

   std::string encode(const std::string &clear_text)
   {
      return getEncoded(clear_text, Transposition_types::Columns);
   }

   std::string decode(const std::string &cipher_text)
   {
      return getDecoded(cipher_text, Transposition_types::Columns);
   }

   // Lecture en colonne du tableau.

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

   // Remplit le tableau avec data selon la longueur de la clé représentant le nombre de colonnes.

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

};

// Transposition en ligne : permutation des colonnes du tableau et lecture en ligne.

class TranspositionRows : public Transposition
{
public:

   std::string encode(const std::string &clear_text)
   {
      return getEncoded(clear_text, Transposition_types::Rows);
   }

   std::string decode(const std::string &cipher_text)
   {
      return getDecoded(cipher_text, Transposition_types::Rows);
   }

   std::string read(const std::vector<std::string> &s_table) const
   {
      return readFinalTable(s_table);
   }

   void setTable(const std::string &data)
   {
      setStartingTable(data);
   }
};

// Chiffre à transposition double : permutation des colonnes puis des lignes du tableau.

class TranspositionDouble : public Transposition
{
public:

   std::string encode(const std::string &clear_text)
   {
      return getEncoded(clear_text, Transposition_types::Double);
   }

   std::string decode(const std::string &cipher_text)
   {
      return getDecoded(cipher_text, Transposition_types::Double);
   }

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

   void setKeyRow(std::vector<unsigned int>& key_row)
   {
      this->key_row = key_row;
   }

private:
   std::vector<unsigned int> key_row;
};

class TranspositionFactory
{
public:

   virtual ~TranspositionFactory()
   {
   }

   static Transposition* createTransposition(Transposition::Transposition_types T_type)
   {
      switch (T_type)
      {
         case Transposition::Transposition_types::Columns:
            return new TranspositionColumns();
         case Transposition::Transposition_types::Rows:
            return new TranspositionRows();
         case Transposition::Transposition_types::Double:
            return new TranspositionDouble();
      }
      throw "Transposition choice is inexistent.";
   }
};

#endif	/* TRANSPOSITION_HPP */