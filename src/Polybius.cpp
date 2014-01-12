#include "Polybius.hpp" 

ClassicalType Polybius::encode(const ClassicalType &clear_text)
{  
   ClassicalType crypted;
   crypted.reserve(clear_text.length() * 2);

   const Grid grid(getGrid(getKey().append(alpha)));
   for (const auto c : clear_text)
   {
      const auto pos = getCharCoordinates(c, grid);
      crypted.push_back(pos.second + '1');
      crypted.push_back(pos.first + '1');
   }

   return crypted;
}

ClassicalType Polybius::decode(const ClassicalType &cipher_text)
{  
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len / 2);
   const Grid grid(getGrid(getKey().append(alpha)));

   for (uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto pos = std::make_pair(cipher_text[i] - '1', cipher_text[i + 1] - '1');
      decrypted.push_back(grid[pos.first][pos.second]);
   }

   return decrypted;
}