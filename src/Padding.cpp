#include "Padding.hpp"

const BytesVector Padding::zeros(const BytesVector &data, const uint8_t block_size)
{
   BytesVector padded_input(data);
   const uint8_t remaining = data.size() % block_size;
   if (remaining != 0)
   {
      padded_input.insert(padded_input.end(), block_size - remaining, 0);
   }

   return padded_input;
}

const BytesVector Padding::remainingValue(const BytesVector &data, const uint8_t block_size)
{
   BytesVector padded_bytes(data);

   const uint8_t remaining_bytes = block_size - (padded_bytes.size() % block_size);
   padded_bytes.reserve(data.size() + remaining_bytes);
   padded_bytes.insert(padded_bytes.end(), remaining_bytes, remaining_bytes);

   return padded_bytes;
}

const BytesVector Padding::_10Star(const BytesVector &data, const uint8_t block_size)
{
   const uint64_t message_len = data.size();

   /* Append '1' bit to the message. */
   BytesVector padding(data);
   padding.reserve(message_len + block_size);
   padding.push_back(0x01);

   /* Append enough '0' bits such that the length will be a multiple of 'block_size'. */
   const uint8_t bytes_to_append = block_size - ((message_len + 1) % block_size);
   if (bytes_to_append != block_size)
   {
      padding.insert(padding.end(), bytes_to_append, 0);
   }

   return padding;
}

const BytesVector Padding::_10Star1(const BytesVector &data, const uint8_t block_size)
{
   BytesVector padding(data);
   padding.reserve(data.size() + block_size + 1);

   if(!((data.size() + 1) % block_size))
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(0x01);
      padding.insert(padding.end(), block_size - (padding.size() % block_size), 0);
      padding.back() = 0x80;
   }

   return padding;
}