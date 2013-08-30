#include "Blake.hpp"

#include "Bits.hpp"

template<class UInt> constexpr uint8_t Blake<UInt>::sigma[][16];
constexpr uint64_t Blake64Bits::C[];
constexpr uint32_t Blake32Bits::C[];
constexpr uint8_t Blake32Bits::G_rotate[];
constexpr uint8_t Blake64Bits::G_rotate[];

const BytesVector Blake256::appendPadding(const BytesVector &message) const
{
   const uint64_t initial_length = message.size();
   BytesVector padding(message);
   padding.reserve(initial_length + (in_block_length << 1));
   
   // If initial_length + 1 is congruant to 56 (mod 64).
   if(((initial_length + 1) % in_block_length) == 56)
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(0x80);
      
      const uint8_t zeros = (120 - (padding.size() & 0x3F)) & 0x3F;
      padding.insert(padding.end(), zeros - 1, 0);
      padding.push_back(0x01);
   }

   return padding;
}

const BytesVector Blake512::appendPadding(const BytesVector &message) const
{
   const uint64_t initial_length = message.size();
   BytesVector padding(message);
   padding.reserve(initial_length + (in_block_length << 1));

   // If initial_length + 1 is congruant to 112 (mod 128).
   if(((initial_length + 1) % in_block_length) == 112)
   {
      padding.push_back(0x81);
   }
   else
   {
      padding.push_back(0x80);

      const uint16_t zeros = (240 - (padding.size() & 0x7F)) & 0x7F;
      padding.insert(padding.end(), zeros - 1, 0);
      padding.push_back(0x01);
   }
   
   // Append 8 times 0x00 byte because we don't support 128-bit integer.
   padding.insert(padding.end(), 8, 0);

   return padding;
}

void Blake32Bits::G(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, 
        const UInt32Vector &block, const uint8_t r, const uint8_t i) const
{
   a += b + (block[sigma[r % 10][i]] ^ C[sigma[r % 10][i + 1]]);
   d = Bits::rotateRight(d ^ a, G_rotate[0], 32);
   c += d;
   b = Bits::rotateRight(b ^ c, G_rotate[1], 32);

   a += b + (block[sigma[r % 10][i + 1]] ^ C[sigma[r % 10][i]]);
   d = Bits::rotateRight(d ^ a, G_rotate[2], 32);
   c += d;
   b = Bits::rotateRight(b ^ c, G_rotate[3], 32);
}

void Blake64Bits::G(uint64_t &a, uint64_t &b, uint64_t &c, uint64_t &d, 
        const UInt64Vector &block, const uint8_t r, const uint8_t i) const
{
   a += b + (block[sigma[r % 10][i]] ^ C[sigma[r % 10][i + 1]]);
   d = Bits::rotateRight64(d ^ a, G_rotate[0]);
   c += d;
   b = Bits::rotateRight64(b ^ c, G_rotate[1]);

   a += b + (block[sigma[r % 10][i + 1]] ^ C[sigma[r % 10][i]]);
   d = Bits::rotateRight64(d ^ a, G_rotate[2]);
   c += d;
   b = Bits::rotateRight64(b ^ c, G_rotate[3]);
}

const BytesVector Blake32Bits::encode(const BytesVector &data)
{
   const uint64_t data_size = data.size();
   BytesVector bytes = appendPadding(data);
   appendLength<BigEndian64>(bytes, data_size << 3);
   
   const uint64_t bytes_len = bytes.size();
   const uint64_t rest = data_size % in_block_length;
   if(rest == 0 || rest > 55)
   {
      counter = 0;
   }
   else
   {
      counter = 512 - ((bytes_len - data_size) << 3);
   }
   
   UInt32Vector hash(IV);
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      UInt32Vector V = initialize(hash, C);
              
      const UInt32Vector input_block = getInputBlocks(bytes, i);
      
      for(uint8_t j = 0; j < number_of_rounds; ++j)
      {
         G(V[0], V[4], V[8], V[12], input_block, j, 0);
         G(V[1], V[5], V[9], V[13], input_block, j, 2);
         G(V[2], V[6], V[10], V[14], input_block, j, 4);
         G(V[3], V[7], V[11], V[15], input_block, j, 6);
         G(V[0], V[5], V[10], V[15], input_block, j, 8);
         G(V[1], V[6], V[11], V[12], input_block, j, 10);
         G(V[2], V[7], V[8], V[13], input_block, j, 12);
         G(V[3], V[4], V[9], V[14], input_block, j, 14);
      }
      
      finalize(hash, V);
   }
   
   return getOutput(output_size, hash);
}

const BytesVector Blake64Bits::encode(const BytesVector &data)
{
   const uint64_t data_size = data.size();
   BytesVector bytes = appendPadding(data);
   appendLength<BigEndian64>(bytes, data_size << 3);
   
   const uint64_t bytes_len = bytes.size();
   const uint64_t rest = data_size % in_block_length;
   if(rest == 0 || rest > 111)
   {
      counter = 0;
   }
   else
   {
      counter = 1024 - ((bytes_len - data_size) << 3);
   }
   
   UInt64Vector hash(IV);
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      UInt64Vector V = initialize(hash, C);
              
      const UInt64Vector input_block = getInputBlocks(bytes, i);
      
      for(uint8_t j = 0; j < number_of_rounds; ++j)
      {
         G(V[0], V[4], V[8], V[12], input_block, j, 0);
         G(V[1], V[5], V[9], V[13], input_block, j, 2);
         G(V[2], V[6], V[10], V[14], input_block, j, 4);
         G(V[3], V[7], V[11], V[15], input_block, j, 6);
         G(V[0], V[5], V[10], V[15], input_block, j, 8);
         G(V[1], V[6], V[11], V[12], input_block, j, 10);
         G(V[2], V[7], V[8], V[13], input_block, j, 12);
         G(V[3], V[4], V[9], V[14], input_block, j, 14);
      }
      
      finalize(hash, V);
   }
   
   return getOutput(output_size, hash);
}