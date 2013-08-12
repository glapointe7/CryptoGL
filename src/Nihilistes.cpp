#include "Nihilistes.hpp"

#include "String.hpp"

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

Nihilistes::Nihilistes(const KeyType &key, const KeyType &second_key)
: SquareCipher(key)
{
   checkKey(second_key);
   this->second_key = second_key;
}

const Nihilistes::ClassicalType Nihilistes::encode(const ClassicalType &clear_text)
{
   const uint32_t clear_len = clear_text.length();
   const KeyType key = getKey();
   const uint32_t second_key_len = second_key.length();
   ClassicalType crypted;
   crypted.reserve(clear_len * 2);

   const Grid grid(getGrid(key + alpha));

   uint32_t i = 0;
   for (const auto c : clear_text)
   {
      const auto pos_c = getCharCoordinates(c, grid);
      const auto pos_key = getCharCoordinates(second_key[i], grid);

      const uint16_t final_value = (((pos_c.second + 1) * 10) + pos_c.first + 1 +
              ((pos_key.second + 1) * 10) + pos_key.first + 1) % 100;
      if (final_value < 10)
      {
         crypted.push_back('0');
      }
      
      // Convert integer to string in a faster way (2x faster with 138 Ko of text).
      // append method is faster than calling the constructor.
      char str[2];
      String::uintToString(final_value, str);
      crypted.append(str);
      i = (i + 1) % second_key_len;
   }
   
   return crypted;
}

const Nihilistes::ClassicalType Nihilistes::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   const KeyType key = getKey();
   const uint32_t second_key_len = second_key.length();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len / 2);
   const Grid grid(getGrid(key + alpha));

   uint32_t j = 0;
   for (uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto pos_key = getCharCoordinates(second_key[j], grid);
      const uint8_t key_value = ((pos_key.second+1) * 10) + pos_key.first + 1;
      const uint8_t value = (atoi(cipher_text.substr(i, 2).c_str()) + 100 - key_value) % 100;
      const uint8_t last_digit = value % 10;
      decrypted += grid[((value - last_digit) / 10) - 1][last_digit - 1];
      j = (j + 1) % second_key_len;
   }
   
   return decrypted;
}