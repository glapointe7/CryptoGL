
#include "Transposition.hpp"

const std::vector<Transposition::ClassicalType> 
Transposition::setStartingTable(const ClassicalType &data)
{
   std::vector<ClassicalType> table;
   const uint8_t key_len = key.size();
   ClassicalType full_data(appendChars(data, key_len, 'X'));
   const unsigned int data_len = full_data.length();
   table.reserve(data_len / key_len);

   // We fill the grid that will be read by column. 
   for (unsigned int i = 0; i < data_len; i += key_len)
   {
      table.push_back(full_data.substr(i, key_len));
   }
   
   return table;
}

const Transposition::ClassicalType 
Transposition::readFinalTable(const std::vector<ClassicalType> &table)
{
   ClassicalType data = "";
   data.reserve(table.size() * key.size());
   for (const auto str : table)
   {
      data += str;
   }

   return data;
}

const std::vector<Transposition::ClassicalType> 
Transposition::swapColumns(const std::vector<ClassicalType> &table)
{
   const uint8_t key_len = key.size();
   const unsigned int rows = table.size();
   std::vector<ClassicalType> s_table(rows, "");

   for (unsigned int i = 0; i < rows; ++i)
   {
      s_table[i].reserve(key_len);
      for (uint8_t j = 0; j < key_len; ++j)
      {
         const uint8_t pos = std::find(key.begin(), key.end(), j) - key.begin();
         s_table[i] += table[i][pos];
      }
   }
   
   return s_table;
}