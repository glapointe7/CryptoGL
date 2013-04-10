
#include "TranspositionBase.hpp"

void TranspositionBase::setStartingTable(const std::string &data)
{
   unsigned int key_len = key.length();

   std::string full_data(data);
   full_data.append(key_len - (data.length() % key_len), 'X');
   unsigned int data_len = full_data.length();

   // On remplit la grille qu'on doit lire en colonne. 
   for (unsigned int i = 0; i < data_len; i += key_len)
   {
      table.push_back(full_data.substr(i, key_len));
   }
}

std::string TranspositionBase::readFinalTable(const std::vector<std::string>& s_table) const
{
   std::string data = "";
   data.reserve(s_table.size() * key.length());
   for (auto str : s_table)
   {
      data += str;
   }

   return data;
}

// Trie la clé en ordre alphabétique en conservant les positions originales de chaque lettre.

std::map<char, unsigned int> TranspositionBase::sortKey() const
{
   std::map<char, unsigned int> sorted_key;
   unsigned int key_len = key.length();

   for (unsigned int i = 0; i < key_len; i++)
   {
      sorted_key.insert(std::make_pair(key[i], i));
   }

   return sorted_key;
}

// Ordonner les lignes selon l'ordre alphabétique des lettres de la clé.

std::vector<std::string>
TranspositionBase::swapColumnsDecode(const std::map<char, unsigned int>& sorted_key) const
{
   std::vector<std::string> sorted_table;
   unsigned int key_len = key.length();
   unsigned int rows = table.size();

   for (unsigned int i = 0; i < rows; i++)
   {
      std::string row(key_len, 'A');
      unsigned int j = 0;
      for (auto pair : sorted_key)
      {
         row[key.find(pair.first)] = table[i][j];
         j++;
      }
      sorted_table.push_back(row);
   }

   return sorted_table;
}

std::vector<std::string>
TranspositionBase::swapColumnsEncode(const std::map<char, unsigned int>& sorted_key) const
{
   std::vector<std::string> sorted_table;
   unsigned int key_len = key.length();
   unsigned int rows = table.size();

   for (unsigned int i = 0; i < rows; i++)
   {
      std::string row;
      row.reserve(key_len);
      for (auto pair : sorted_key)
      {
         row += table[i][pair.second];
      }
      sorted_table.push_back(row);
   }

   return sorted_table;
}