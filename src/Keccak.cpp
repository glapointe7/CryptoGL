#include "Keccak.hpp"

#include "Bits.hpp"
#include "LittleEndian.hpp"

constexpr std::array<uint64_t, 24> Keccak::round_constants;
constexpr std::array<std::array<uint8_t, 5>, 5> Keccak::rho_offsets;

void Keccak::applyRound(const uint8_t round_index)
{
   uint64_t C[5];
   uint64_t D[5];

   // Theta step.
   for (uint8_t x = 0; x < 5; ++x)
   {
      C[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
   }

   for (uint8_t x = 0; x < 5; ++x)
   {
      D[x] = C[(x + 4) % 5] ^ Bits::rotateLeft64(C[(x + 1) % 5], 1);
   }
   
   for (uint8_t x = 0; x < 5; ++x)
   {
      for (uint8_t y = 0; y < 5; ++y)
      {
         state[x][y] ^= D[x];
      }
   }

   // Pi and rho steps.
   uint64_t B[5][5];
   for (uint8_t x = 0; x < 5; ++x)
   {
      for (uint8_t y = 0; y < 5; ++y)
      {
         B[y][(2 * x + 3 * y) % 5] = Bits::rotateLeft64(state[x][y], rho_offsets[x][y]);
      }
   }

   // Chi step.   
   for (uint8_t y = 0; y < 5; ++y)
   {
      for (uint8_t x = 0; x < 5; ++x)
      {
         state[x][y] = B[x][y] ^ ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]);
      }
   }

   // Iota step.
   state[0][0] ^= round_constants[round_index];
}

void Keccak::F()
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      applyRound(i);
   }
}

UInt64Vector Keccak::convertBlockToState(const BytesVector &block) const
{
   UInt64Vector int_block;
   int_block.reserve(width / lane_width);
   
   for(uint8_t i = 0; i < 200; i += 8)
   {
      int_block.push_back(LittleEndian64::toInteger(BytesVector(block.begin() + i, block.begin() + i + 8)));
   }
   
   return int_block;
}

void Keccak::applyAbsorbingPhase(const BytesVector &padded_message)
{   
   // If bitrate = 1024, then we iterate on blocks of 128 bytes + 72 bytes of 0.
   const uint64_t pad_len = padded_message.size();
   for (uint64_t i = 0; i < pad_len; i += block_size)
   {
      BytesVector block(padded_message.begin() + i, padded_message.begin() + i + block_size);
      block.insert(block.end(), capacity >> 3, 0);
      
      const UInt64Vector Pi = convertBlockToState(block);      
      for(uint8_t x = 0; x < 5; ++x)
      {
         for(uint8_t y = 0; y < 5; ++y)
         {
            state[x][y] ^= Pi[x + 5*y];
         }
      }
      
      F();
   }
}

BytesVector Keccak::applySqueezingPhase()
{
   BytesVector output;
   output.reserve(block_size);
   
   for (int16_t remaining_size = output_size; remaining_size > 0; remaining_size -= bitrate)
   {
      for(uint8_t x = 0; x < 5; ++x)
      {
         for(uint8_t y = 0; y < 5; ++y)
         {
            const BytesVector tmp = LittleEndian64::toBytesVector(state[y][x]);
            output.insert(output.end(), tmp.begin(), tmp.end());
         }
      }      
      if(remaining_size > 0)
      {
         F();
      }
   }

   return BytesVector(output.begin(), output.begin() + (output_size >> 3));
}