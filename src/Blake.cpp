#include "Blake.hpp"

#include "Bits.hpp"

template<class DataType, uint8_t InputBlockSize> 
   constexpr uint8_t Blake<DataType, InputBlockSize>::sigma[][16];
constexpr uint64_t Blake64Bits::C[];
constexpr uint32_t Blake32Bits::C[];
constexpr uint8_t Blake32Bits::G_rotate[];
constexpr uint8_t Blake64Bits::G_rotate[];

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

void Blake32Bits::compress(UInt32Vector &int_block, UInt32Vector &hash)
{
   UInt32Vector V = initialize(hash, C);
   for(uint8_t j = 0; j < rounds; ++j)
   {
      G(V[0], V[4], V[8], V[12], int_block, j, 0);
      G(V[1], V[5], V[9], V[13], int_block, j, 2);
      G(V[2], V[6], V[10], V[14], int_block, j, 4);
      G(V[3], V[7], V[11], V[15], int_block, j, 6);
      G(V[0], V[5], V[10], V[15], int_block, j, 8);
      G(V[1], V[6], V[11], V[12], int_block, j, 10);
      G(V[2], V[7], V[8], V[13], int_block, j, 12);
      G(V[3], V[4], V[9], V[14], int_block, j, 14);
   }

   finalize(hash, V);
}

void Blake64Bits::compress(UInt64Vector &int_block, UInt64Vector &hash)
{
   UInt64Vector V = initialize(hash, C);
   for(uint8_t j = 0; j < rounds; ++j)
   {
      G(V[0], V[4], V[8], V[12], int_block, j, 0);
      G(V[1], V[5], V[9], V[13], int_block, j, 2);
      G(V[2], V[6], V[10], V[14], int_block, j, 4);
      G(V[3], V[7], V[11], V[15], int_block, j, 6);
      G(V[0], V[5], V[10], V[15], int_block, j, 8);
      G(V[1], V[6], V[11], V[12], int_block, j, 10);
      G(V[2], V[7], V[8], V[13], int_block, j, 12);
      G(V[3], V[4], V[9], V[14], int_block, j, 14);
   }

   finalize(hash, V);
}