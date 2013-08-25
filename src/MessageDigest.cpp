#include "MessageDigest.hpp"

#include "Bits.hpp"

constexpr uint8_t MD2::digits_of_pi[256];
constexpr uint8_t MD4::left_rotation_table[48];
constexpr uint8_t MD4::word_indexes[48];
constexpr uint8_t MD5::left_rotation_table[64];
constexpr uint32_t MD5::sine_magic_numbers[64];

const MessageDigest::BytesContainer MD2::appendPadding(const BytesContainer &data) const
{
   const uint64_t bytes_len = data.size();
   BytesContainer bytes_pad(data);

   // Pad with the value 'bytes_to_pad' at the end of bits_pad until the length is multiple of 16.
   uint8_t bytes_to_pad = 16 - (bytes_pad.size() & 0xF);
   bytes_pad.reserve(bytes_len + 16 + bytes_to_pad);
   bytes_pad.insert(bytes_pad.end(), bytes_to_pad, bytes_to_pad);

   return bytes_pad;
}

void MD2::process(const BytesContainer &data, BytesContainer &hash, const uint64_t &index)
{
   for (uint8_t j = 0; j < 16; ++j)
   {
      hash[16 + j] = data[index + j];
      hash[32 + j] = hash[j] ^ data[index + j];
   }

   // Process 18 rounds.
   uint8_t t = 0;
   for (uint8_t j = 0; j < 18; ++j)
   {
      for (uint8_t k = 0; k < 48; ++k)
      {
         t = hash[k] ^= digits_of_pi[t];
      }
      t = (t + j) & 0xFF;
   }
}

const MessageDigest::BytesContainer MD2::encode(const BytesContainer &data)
{
   const BytesContainer bytes(appendPadding(data));
   BytesContainer hash(48, 0);

   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      process(bytes, hash, i);

      // Update checksum.
      uint8_t t = checksum[15];
      for (uint8_t j = 0; j < 16; ++j)
      {
         t = checksum[j] ^= digits_of_pi[bytes[i + j] ^ t];
      }
   }

   // Last update of checksum.
   process(checksum, hash, 0);

   return BytesContainer(hash.begin(), hash.begin() + 16);
}

const MessageDigest::BytesContainer MD4::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);
   
   const uint64_t bytes_len = bytes.size();
   /* Initial values. */
   UInt32Container state = {
      0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476
   };

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash(state);
      uint32_t f, k = 0;
      for (uint8_t j = 0; j < 48; ++j)
      {
         if (j < 16)
         {
            f = F(hash[1], hash[2], hash[3]);
         }
         else if (j < 32)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = 0x5A827999;
         }
         else
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0x6ED9EBA1;
         }

         const uint32_t tmp = hash[3];
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] = Bits::rotateLeft(hash[0] + f + words[word_indexes[j]] + k, left_rotation_table[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getOutput(4, state);
}

const MessageDigest::BytesContainer MD5::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);
   
   const uint64_t bytes_len = bytes.size();
   /* Initial values. */
   UInt32Container state = {
      0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476
   };

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 64; ++j)
      {
         if (j < 16)
         {
            f = F(hash[1], hash[2], hash[3]);
            k = j;
         }
         else if (j < 32)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = ((5 * j) + 1) & 0xF;
         }
         else if (j < 48)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = ((3 * j) + 5) & 0xF;
         }
         else
         {
            f = I(hash[1], hash[2], hash[3]);
            k = (7 * j) & 0xF;
         }

         const uint32_t tmp = hash[3];
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] += Bits::rotateLeft(hash[0] + f + words[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getOutput(4, state);
}