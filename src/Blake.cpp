#include "Blake.hpp"

template<class DataType, uint8_t InputBlockSize> 
   constexpr uint8_t Blake<DataType, InputBlockSize>::sigma[][16];

const BytesVector Blake256::pad(const BytesVector &message) const
{
   const uint64_t initial_length = message.size();
   BytesVector padding(message);
   padding.reserve(initial_length + 128);
   
   // If initial_length + 1 is congruant to 56 (mod 64).
   if(((initial_length + 1) % 64) == 56)
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(0x80);
      
      const uint8_t zeros = (120 - (padding.size() & 0x3F)) & 0x3F;
      padding.insert(padding.end(), zeros, 0);
      padding.back() = 0x01;
   }

   return padding;
}

const BytesVector Blake512::pad(const BytesVector &message) const
{
   const uint64_t initial_length = message.size();
   BytesVector padding(message);
   padding.reserve(initial_length + 256);

   // If initial_length + 1 is congruant to 112 (mod 128).
   if(((initial_length + 1) % 128) == 112)
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(0x80);

      const uint16_t zeros = (240 - (padding.size() & 0x7F)) & 0x7F;
      padding.insert(padding.end(), zeros, 0);
      padding.back() = 0x01;
   }
   
   // Append 8 times 0x00 byte because we don't support 128-bit integer.
   padding.insert(padding.end(), 8, 0);

   return padding;
}