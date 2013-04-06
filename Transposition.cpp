
#include "Transposition.hpp"

#include <map>
#include <vector>
#include <string>

Transposition::Transposition()
{
   
}

void Transposition::setKey(const std::string &key)
{
   this->key = key;
}

std::vector<std::string> Transposition::setTable(const std::string &data) const
{
   std::vector<std::string> table;
   unsigned int key_len = key.length();
   
   // On remplit la grille qu'on doit lire en colonne. 
   // Pour faciliter le code, on la transpose.
   unsigned int data_len = data.length();
   std::string tmp;
   tmp.reserve(data_len);
   for (unsigned int i = 0; i < key_len; i++)
   {
      tmp = "";
      for (unsigned int k = i; k < data_len; k += key_len)
      {
         tmp += data[k];
      }
      table.push_back(tmp);
   }
   
   return table;
}

// Ordonner les lignes selon l'ordre alphabétique des lettres de la clé.
std::vector<std::string> Transposition::sortColon(const std::vector<std::string> &table) const
{
   unsigned int key_len = key.length();
   std::map<char, unsigned int> sorted_key;
   
   for(unsigned int i = 0; i < key_len; i++)
   {
      sorted_key.insert(std::make_pair(key[i], i));
   }
   
   std::vector<std::string> sorted_table;
   for(auto pair : sorted_key)
   {
      sorted_table.push_back(table[pair.second]);
   }
   
   return sorted_table;
}