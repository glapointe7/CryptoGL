#include "Padding.hpp"

using namespace CryptoGL;

BytesVector Padding::zeros(BytesVector data, const uint8_t block_size)
{
   const uint8_t remaining = data.size() % block_size;
   if (remaining != 0)
   {
      data.insert(data.end(), block_size - remaining, 0);
   }

   return data;
}

BytesVector Padding::remainingValue(BytesVector data, const uint8_t block_size)
{
   const uint8_t remaining_bytes = block_size - (data.size() % block_size);
   data.reserve(data.size() + remaining_bytes);
   data.insert(data.end(), remaining_bytes, remaining_bytes);

   return data;
}

BytesVector Padding::_10Star(BytesVector data, const uint8_t block_size)
{
   const uint64_t data_len = data.size();

   /* Append '1' bit to the message. */
   data.reserve(data_len + block_size);
   data.push_back(0x80);

   /* Append enough '0' bits such that the length will be a multiple of 'block_size'. */
   const uint8_t bytes_to_append = block_size - ((data_len + 1) % block_size);
   if (bytes_to_append != block_size)
   {
      data.insert(data.end(), bytes_to_append, 0);
   }

   return data;
}

BytesVector Padding::_10Star1(BytesVector data, const uint8_t block_size, 
        const uint8_t first_byte, const uint8_t last_byte)
{
   const uint64_t data_len = data.size();
   data.reserve(data_len + block_size + 1);

   if(!((data_len + 1) % block_size))
   {
      data.push_back(0x81);
   }
   else
   {
      data.push_back(first_byte);
      data.insert(data.end(), block_size - ((data_len + 1) % block_size), 0);
      data.back() = last_byte;
   }

   return data;
}