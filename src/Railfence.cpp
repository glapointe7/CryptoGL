#include "Railfence.hpp"

#include <string>
#include <utility>

#include "exceptions/EmptyKey.hpp"

RedefenceZigzag::RedefenceZigzag(const KeyType &key, const KeyType &key_levels)
{
   if(key.size() == 1)
   {
      KeyType my_key;
      my_key.reserve(key[0]);
      for(int32_t i = 0; i < key[0]; ++i)
      {
         my_key.push_back(i);
      }
      setKey(my_key);
      this->max_level = key[0];
   }
   else
   {
      setKey(key);
      this->max_level = key.size();
   }

   if (key_levels.empty())
   {
      throw EmptyKey("Your key is empty.");
   }

   this->key_levels = key_levels;
}

const RedefenceZigzag::ClassicalType RedefenceZigzag::encode(const ClassicalType &clear_text)
{
   const uint32_t clear_len = clear_text.length();
   const KeyType key = getKey();
   ClassicalType crypted;
   crypted.reserve(clear_len);

   std::vector<ClassicalType> rows;
   rows.resize(max_level);
   
   // Add the offset to the clear_text.
   const uint32_t real_offset = offset % key_levels[0];
   ClassicalType text_with_offset(real_offset, '.');
   text_with_offset.reserve(clear_len + real_offset);
   text_with_offset += clear_text;

   const uint32_t text_with_offset_len = clear_len + real_offset;
   int32_t step = (key_levels[0] - 1) * 2;
   uint32_t j = 0;
   for (uint32_t i = 0; i < text_with_offset_len - step; i += step)
   {
      // Encode in basic Railfence with level as the key.
      const ClassicalType data(text_with_offset.substr(i, step));
      for (int32_t k = 0; k < key_levels[j]; ++k)
      {
         for (int32_t l = 0; l < step; ++l)
         {
            if (l % step == k || l % step == step - k)
            {
               rows[k] += alpha[alpha.find(data[l])];
            }
         }
      }

      j = (j + 1) % key_levels.size();
      step = (key_levels[j] - 1) * 2;
   }
   
   const int32_t rest = text_with_offset_len % step;
   const ClassicalType data(text_with_offset.substr(text_with_offset_len - rest));
   if(rest < key_levels[j])
   {
      for(int32_t i = 0; i < rest; ++i)
      {
         rows[i] += alpha[alpha.find(data[i])];
      }
   }
   else
   {
      for(int32_t i = 0; i < key_levels[j]; ++i)
      {
         for (int32_t l = 0; l < rest; ++l)
         {
            if (l % rest == i || l % rest == step - i)
            {
               rows[i] += alpha[alpha.find(data[l])];
            }
         }
      }
   }

   // Remove the offset character '.'.
   for (uint32_t k = 0; k < real_offset; ++k)
   {
      rows[k].erase(rows[k].front());
   }

   for (const auto key_num : key)
   {
      crypted += rows[key_num];
   }

   return crypted;
}

const RedefenceZigzag::ClassicalType RedefenceZigzag::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted(cipher_len, 'A');

   /*auto step = std::make_pair((key - 1) * 2, 0);
   uint32_t k = 0;

   for (uint32_t i = 0; i < key; i++)
   {
      uint32_t j = i;
      decrypted[j] = cipher_text[k];
      k++;
      j += step.first;
      while (j < cipher_len)
      {
         if (step.first != 0)
         {
            decrypted[j] = cipher_text[k];
            k++;
         }
         j += step.second;
         if (j < cipher_len && step.second != 0)
         {
            decrypted[j] = cipher_text[k];
            k++;
         }
         j += step.first;
      }
      step.first -= 2;
      step.second += 2;
   }*/

   return decrypted;
}