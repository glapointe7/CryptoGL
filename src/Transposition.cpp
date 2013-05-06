
#include "Transposition.hpp"

#include <random>

void Transposition::setStartingTable(const ClassicalType &data)
{
   const unsigned int key_len = key.length();
   ClassicalType full_data(appendChars(data, key_len, 'X'));
   const unsigned int data_len = full_data.length();

   // On remplit la grille qu'on doit lire en colonne. 
   for (unsigned int i = 0; i < data_len; i += key_len)
   {
      table.push_back(full_data.substr(i, key_len));
   }
}

const Transposition::ClassicalType
Transposition::readFinalTable(const std::vector<ClassicalType>& s_table) const
{
   ClassicalType data = "";
   data.reserve(s_table.size() * key.length());
   for (const auto str : s_table)
   {
      data += str;
   }

   return data;
}

// Trie la clé en ordre alphabétique en conservant les positions originales de chaque lettre.

const std::map<char, unsigned int> Transposition::sortKey() const
{
   std::map<char, unsigned int> sorted_key;
   const unsigned int key_len = key.length();

   for (unsigned int i = 0; i < key_len; i++)
   {
      sorted_key.insert(std::make_pair(key[i], i));
   }

   return sorted_key;
}

// Ordonner les lignes selon l'ordre alphabétique des lettres de la clé.

const std::vector<Transposition::ClassicalType>
Transposition::swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const
{
   std::vector<ClassicalType> sorted_table;
   const unsigned int key_len = key.length();
   const unsigned int rows = table.size();

   for (unsigned int i = 0; i < rows; i++)
   {
      std::string row(key_len, 'A');
      unsigned int j = 0;
      for (const auto pair : sorted_key)
      {
         row[key.find(pair.first)] = table[i][j];
         j++;
      }
      sorted_table.push_back(row);
   }

   return sorted_table;
}

const std::vector<Transposition::ClassicalType>
Transposition::swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const
{
   std::vector<ClassicalType> sorted_table;
   const unsigned int key_len = key.length();
   const unsigned int rows = table.size();

   for (unsigned int i = 0; i < rows; i++)
   {
      std::string row;
      row.reserve(key_len);
      for (const auto pair : sorted_key)
      {
         row += table[i][pair.second];
      }
      sorted_table.push_back(row);
   }

   return sorted_table;
}