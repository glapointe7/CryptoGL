#include "Padding.hpp"

BytesVector Padding::zeros(const BytesVector &data, const uint8_t block_size)
{
   BytesVector padded_input(data);
   const uint8_t remaining = data.size() % block_size;
   if (remaining != 0)
   {
      padded_input.insert(padded_input.end(), block_size - remaining, 0);
   }

   return padded_input;
}

BytesVector Padding::remainingValue(const BytesVector &data, const uint8_t block_size)
{
   BytesVector padded_bytes(data);

   const uint8_t remaining_bytes = block_size - (padded_bytes.size() % block_size);
   padded_bytes.reserve(data.size() + remaining_bytes);
   padded_bytes.insert(padded_bytes.end(), remaining_bytes, remaining_bytes);

   return padded_bytes;
}

BytesVector Padding::_10Star(const BytesVector &data, const uint8_t block_size)
{
   const uint64_t message_len = data.size();

   /* Append '1' bit to the message. */
   BytesVector padding(data);
   padding.reserve(message_len + block_size);
   padding.push_back(0x80);

   /* Append enough '0' bits such that the length will be a multiple of 'block_size'. */
   const uint8_t bytes_to_append = block_size - ((message_len + 1) % block_size);
   if (bytes_to_append != block_size)
   {
      padding.insert(padding.end(), bytes_to_append, 0);
   }

   return padding;
}

BytesVector Padding::_10Star1(const BytesVector &data, const uint8_t block_size, 
        const uint8_t first_byte, const uint8_t last_byte)
{
   BytesVector padding(data);
   padding.reserve(data.size() + block_size + 1);

   if(!((data.size() + 1) % block_size))
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(first_byte);
      padding.insert(padding.end(), block_size - (padding.size() % block_size), 0);
      padding.back() = last_byte;
   }

   return padding;
}