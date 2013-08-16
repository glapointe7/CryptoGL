#include "Keccak.hpp"

#include "Tools.hpp"
#include "LittleEndian.hpp"

constexpr uint64_t Keccak::round_constants[];
constexpr uint8_t Keccak::rho_offsets[][5];

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
      D[x] = C[(x + 4) % 5] ^ rotateLeft64(C[(x + 1) % 5], 1);
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
         B[y][(2 * x + 3 * y) % 5] = rotateLeft64(state[x][y], rho_offsets[x][y]);
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

const Keccak::UInt64Container Keccak::convertBlockToState(const BytesContainer &block) const
{
   UInt64Container int_block;
   int_block.reserve(width / lane_width);
   
   LittleEndian64 LE;
   for(uint8_t i = 0; i < 200; i += 8)
   {
      LE.toInteger(BytesContainer(block.begin() + i, block.begin() + i + 8));
      int_block.push_back(LE.getValue());
      LE.resetValue();
   }
   
   return int_block;
}

void Keccak::applyAbsorbingPhase(const BytesContainer &padded_message)
{   
   // If bitrate = 1024, then we iterate on blocks of 128 bytes + 72 bytes of 0.
   const uint64_t pad_len = padded_message.size();
   for (uint64_t i = 0; i < pad_len; i += block_size)
   {
      BytesContainer block(padded_message.begin() + i, padded_message.begin() + i + block_size);
      block.insert(block.end(), capacity >> 3, 0);
      
      const UInt64Container Pi = convertBlockToState(block);      
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

const Keccak::BytesContainer Keccak::applySqueezingPhase()
{
   BytesContainer output;
   output.reserve(block_size);
   
   for (int16_t remaining_size = output_size; remaining_size > 0; remaining_size -= bitrate)
   {
      LittleEndian64 LE;
      for(uint8_t x = 0; x < 5; ++x)
      {
         for(uint8_t y = 0; y < 5; ++y)
         {
            LE.toBytes(state[y][x]);
            const BytesContainer tmp = LE.getBytes();
            output.insert(output.end(), tmp.begin(), tmp.end());
         }
      }      
      if(remaining_size > 0)
      {
         F();
      }
   }

   return BytesContainer(output.begin(), output.begin() + (output_size >> 3));
}