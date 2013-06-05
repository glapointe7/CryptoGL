#include "Railfence.hpp"

#include "exceptions/EmptyKey.hpp"

RedefenceZigzag::RedefenceZigzag(const KeyType &key, const KeyType &key_levels)
{
   if (key.size() == 1)
   {
      KeyType my_key;
      my_key.reserve(key[0]);
      for (int32_t i = 0; i < key[0]; ++i)
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

void RedefenceZigzag::addEncodedText(std::vector<ClassicalType> &rows, const ClassicalType &data,
        const int32_t step, const int32_t level) const
{
   for (int32_t k = 0; k < level; ++k)
   {
      rows[k] += alpha[alpha.find(data[k])];
   }
   const int32_t mod = step - level;
   for (int32_t k = 0; k < mod; ++k)
   {
      rows[level - k - 2] += alpha[alpha.find(data[level + k])];
   }
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

   const uint32_t key_levels_size = key_levels.size();
   uint32_t j = 0, i = 0;
   int32_t step = (key_levels[j] - 1) * 2;
   while (i < text_with_offset_len - step)
   {
      // Encode in basic Railfence with level as the key.
      const ClassicalType data(text_with_offset.substr(i, step));
      addEncodedText(rows, data, step, key_levels[j]);
      j = (j + 1) % key_levels_size;
      i += step;
      step = (key_levels[j] - 1) * 2;
   }

   const int32_t rest = text_with_offset_len % step;
   const ClassicalType data(text_with_offset.substr(text_with_offset_len - rest));
   if (rest < key_levels[j])
   {
      for (int32_t i = 0; i < rest; ++i)
      {
         rows[i] += alpha[alpha.find(data[i])];
      }
   }
   else
   {
      addEncodedText(rows, data, rest, key_levels[j]);
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

const std::vector<std::list<int8_t> >
RedefenceZigzag::getFirstDecoding(const ClassicalType &cipher_text, int32_t &last) const
{
   const KeyType key = getKey();
   const uint32_t key_levels_size = key_levels.size();
   const int32_t cipher_len = cipher_text.length();

   uint32_t i = 1, j = 0;
   int32_t sum_step = (key_levels[j] - 1) * 2;
   while (sum_step < cipher_len)
   {
      j = (j + 1) % key_levels_size;
      sum_step += (key_levels[j] - 1) * 2;
      ++i;
   }

   std::vector<std::list<int8_t> > decoding;
   decoding.resize(max_level);
   uint32_t sum_x = 0;
   for (const auto level : key)
   {
      uint32_t x = 0;
      if (level == 0)
      {
         x = i;
      }
      else
      {
         for (uint32_t k = 0; k < i - 1; ++k)
         {
            if (key_levels[k % key_levels_size] - 1 == level)
            {
               x++;
            }
            else if (level < key_levels[k % key_levels_size] - 1)
            {
               x += 2;
            }
         }

         // If cipher_len != sum_step, then we have to consider the rest apart.
         if (key_levels[j] - 1 >= level)
         {
            const int32_t step = (key_levels[j] - 1) * 2;
            const int32_t prev_sum_step = 1 + sum_step - step;
            if (prev_sum_step + level <= cipher_len)
            {
               x++;
               if (prev_sum_step + level == cipher_len)
               {
                  last = level;
               }
            }
            if (sum_step - level < cipher_len)
            {
               x++;
               if (sum_step - level == cipher_len - 1)
               {
                  last = level;
               }
            }
         }
      }

      const ClassicalType block(cipher_text.substr(sum_x, x));
      decoding[level] = std::list<int8_t>(block.begin(), block.end());
      sum_x += x;
   }

   return decoding;
}

const RedefenceZigzag::ClassicalType RedefenceZigzag::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   const uint32_t key_levels_size = key_levels.size();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len);

   int32_t last_char_level = 0;
   std::vector<std::list<int8_t> > first_decoded(getFirstDecoding(cipher_text, last_char_level));

   uint32_t j = 0;
   while (!first_decoded[last_char_level].empty())
   {
      for (int32_t k = 0; k < key_levels[j] && !first_decoded[last_char_level].empty(); ++k)
      {
         decrypted += first_decoded[k].front();
         first_decoded[k].pop_front();
      }
      if (key_levels[j] > 1)
      {
         for (int32_t k = key_levels[j] - 2; k >= 1 && !first_decoded[last_char_level].empty(); --k)
         {
            decrypted += first_decoded[k].front();
            first_decoded[k].pop_front();
         }
      }
      j = (j + 1) % key_levels_size;
   }

   return decrypted;
}