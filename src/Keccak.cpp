#include "Keccak.hpp"

#include "Tools.hpp"
#include "LittleEndian.hpp"

const uint64_t Keccak::round_constants[24] = {
   0x0000000000000001,
   0x0000000000008082,
   0x800000000000808A,
   0x8000000080008000,
   0x000000000000808B,
   0x0000000080000001,
   0x8000000080008081,
   0x8000000000008009,
   0x000000000000008A,
   0x0000000000000088,
   0x0000000080008009,
   0x000000008000000A,
   0x000000008000808B,
   0x800000000000008B,
   0x8000000000008089,
   0x8000000000008003,
   0x8000000000008002,
   0x8000000000000080,
   0x000000000000800A,
   0x800000008000000A,
   0x8000000080008081,
   0x8000000000008080,
   0x0000000080000001,
   0x8000000080008008
};

const uint8_t Keccak::rotation_offsets[5][5] = {
   {0, 36, 3, 41, 18},
   {1, 44, 10, 45, 2},
   {62, 6, 43, 15, 61},
   {28, 55, 25, 21, 56},
   {27, 20, 39, 8, 14}
};

void Keccak::applyRound(const uint8_t round_index)
{
   uint64_t B[5][5];
   uint64_t C[5];
   uint64_t D[5];

   // Theta step.
   for (uint8_t x = 0; x < 5; ++x)
   {
      C[x] = state[getIndex(x, 0)] ^ state[getIndex(x, 1)] ^ state[getIndex(x, 2)]
              ^ state[getIndex(x, 3)] ^ state[getIndex(x, 4)];
   }

   for (uint8_t x = 0; x < 5; ++x)
   {
      D[x] = C[(5 + (x - 1)) % 5] ^ rotateLeft64(C[(x + 1) % 5], 1);
   }
   
   for (uint8_t x = 0; x < 5; ++x)
   {
      for (uint8_t y = 0; y < 5; ++y)
      {
         state[getIndex(x, y)] ^= D[x];
      }
   }

   // Pi and rho steps.
   for (uint8_t x = 0; x < 5; ++x)
   {
      for (uint8_t y = 0; y < 5; ++y)
      {
         B[y][(2 * x + 3 * y) % 5] = rotateLeft64(state[getIndex(x, y)], rotation_offsets[x][y]);
      }
   }

   // Chi step.   
   for (uint8_t x = 0; x < 5; ++x)
   {
      for (uint8_t y = 0; y < 5; ++y)
      {
         state[getIndex(x, y)] = B[x][y] ^ ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]);
      }
   }

   // Iota step.
   state[0] ^= round_constants[round_index];
}

void Keccak::F()
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      applyRound(i);
   }
}

void Keccak::applyAbsorbingPhase(const BytesContainer &padded_message)
{
   const uint64_t pad_len = padded_message.size();
   const uint8_t lane_bytes = lane_width >> 3;
   const uint16_t block_size = bitrate / lane_bytes;
   
   // If bitrate = 1024, then we iterate on blocks of 128 bytes.
   LittleEndian64 *LE = new LittleEndian64();
   for (uint64_t i = 0; i < pad_len; i += block_size)
   {
      // Block of 128 bytes from the padded input message (if r = 1024).
      const BytesContainer block(padded_message.begin() + i, padded_message.begin() + i + block_size);

      /* x + 5y < bitrate / lane_width. If bitrate = 1024 and lane_width = 64, x + 5y < 16. */
      // Transform 64-bit sub-blocks in a 64-bit integer in little-endian. XOR the sub-block onto the state. 
      for (uint16_t j = 0; j < block_size; j += lane_bytes)
      {
         LE->toInteger(BytesContainer(block.begin() + j, block.begin() + j + lane_bytes));
         state[j / lane_bytes] ^= LE->getValue();
         LE->resetValue();
      }
      F();
   }
   delete LE;
}

const Keccak::BytesContainer Keccak::applySqueezingPhase()
{
   const uint16_t block_size = bitrate / (lane_width >> 3);
   BytesContainer output;
   output.reserve(block_size);

   const BytesContainer tmp = getOutput(block_size >> 2, state);
   output.insert(output.end(), tmp.begin(), tmp.end());
   
   int16_t remaining_size = output_size - bitrate;
   while (remaining_size > 0)
   {
      F();
      const BytesContainer tmp = getOutput(block_size >> 2, state);
      output.insert(output.end(), tmp.begin(), tmp.end());

      remaining_size -= bitrate;
   }

   return BytesContainer(output.begin(), output.begin() + (output_size >> 3));
}