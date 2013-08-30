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
        const int32_t i, const int32_t level, const int32_t mod) const
{
   // Encode upto level.
   for (int32_t k = i; k < level; ++k)
   {
      rows[k].push_back(alpha[alpha.find(data[k-i])]);
   }

   // Encode from level down to 1.
   for (int32_t k = 0; k < mod; ++k)
   {
      rows[level - k - 2].push_back(alpha[alpha.find(data[level + k-i])]);
   }
}

const ClassicalType 
RedefenceZigzag::swapRowsWithPermutationKey(const std::vector<ClassicalType> &rows, const uint32_t length) const
{
   const KeyType key = getKey();
   ClassicalType crypted;
   crypted.reserve(length);
   for (const auto key_num : key)
   {
      crypted.append(rows[key_num]);
   }
   
   return crypted;
}

const ClassicalType RedefenceZigzag::encode(const ClassicalType &clear_text)
{
   const uint32_t clear_len = clear_text.length();
   std::vector<ClassicalType> rows;
   rows.resize(max_level);

   // Encode each block of 'step' characters in basic Railfence with level as the key. 
   const uint32_t key_levels_size = key_levels.size();
   const int32_t max_step = (max_level - 1) << 1;
   uint32_t j = 0, i = 0;
   int32_t step = (key_levels[j] - 1) << 1;
   
   // Encode the first block with respect to the offset.
   addEncodedText(rows, clear_text.substr(i, step - offset), offset, key_levels[j], key_levels[j]-2);
   i += step - offset;
   j = (j + 1) % key_levels_size;
   while (i < clear_len - max_step)
   {
      step = (key_levels[j] - 1) << 1;
      addEncodedText(rows, clear_text.substr(i, step), 0, key_levels[j], key_levels[j]-2);
      i += step;
      j = (j + 1) % key_levels_size;
   }

   step = (key_levels[j] - 1) << 1;
   const int32_t rest = (clear_len + offset) % step;
   const uint32_t last_iteration = clear_len - rest;
   if (rest < key_levels[j])
   {
      for (int32_t i = 0; i < rest; ++i)
      {
         rows[i].push_back(alpha[alpha.find(clear_text[last_iteration + i])]);
      }
   }
   else
   {
      addEncodedText(rows, clear_text.substr(last_iteration), 0, key_levels[j], rest - key_levels[j]);
   }
   
   return swapRowsWithPermutationKey(rows, clear_len);
}

const std::vector<std::list<int8_t> >
RedefenceZigzag::getFirstDecoding(const ClassicalType &cipher_text, int32_t &last) const
{
   const KeyType key = getKey();
   const uint32_t key_levels_size = key_levels.size();
   const int32_t cipher_len = cipher_text.length();

   uint32_t i = 1, j = 0;
   int32_t sum_step = ((key_levels[j] - 1) << 1);
   const int32_t cipher_offset_len = cipher_len + offset;
   while (sum_step < cipher_offset_len)
   {
      j = (j + 1) % key_levels_size;
      sum_step += (key_levels[j] - 1) << 1;
      ++i;
   }

   std::vector<std::list<int8_t> > decoding;
   decoding.resize(max_level);
   uint32_t sum_x = 0;
   for (const auto level : key)
   {
      uint32_t x = 0;
      
      // Bottom of the zigzag.
      if (level == 0)
      {
         x = i;
         if(offset > 0)
         {
            x = i - 1;
         }
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
               if(k == 0 && level < static_cast<int32_t>(offset))
               {
                  x++;
               }
               else
               {
                  x += 2;
               }
            }
         }

         // If cipher_len != sum_step, then we have to consider the rest apart.
         if (key_levels[j] > level)
         {
            const int32_t step = (key_levels[j] - 1) << 1;
            const int32_t prev_sum_step = 1 + sum_step - step;
            if (prev_sum_step + level <= cipher_offset_len)
            {
               x++;
               if (prev_sum_step + level == cipher_offset_len)
               {
                  last = level;
               }
            }
            if (sum_step - level < cipher_offset_len && key_levels[j] - 1 != level)
            {
               x++;
               if (sum_step - level == cipher_offset_len - 1)
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

const ClassicalType RedefenceZigzag::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   const uint32_t key_levels_size = key_levels.size();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len);

   int32_t last_char_level = 0;
   std::vector<std::list<int8_t> > first_decoded(getFirstDecoding(cipher_text, last_char_level));

   uint32_t j = 0;
   int32_t i = offset;
   while (!first_decoded[last_char_level].empty())
   {
      for (int32_t k = i; k < key_levels[j] && !first_decoded[last_char_level].empty(); ++k)
      {
         decrypted += first_decoded[k].front();
         first_decoded[k].pop_front();
      }
      i = 0;
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