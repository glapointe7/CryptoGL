#include "SHA.hpp"

#include "Tools.hpp"

const HashFunction::WordsContainer SHA::round_constants = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

const HashFunction::DWordsContainer SHA::first_cubic_root_primes = {
   0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
   0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
   0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
   0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
   0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
   0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
   0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
   0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
   0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
   0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
   0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
   0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
   0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
   0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
   0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
   0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
   0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
   0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
   0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
   0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
};

uint64_t SHA::maj(const uint64_t &x, const uint64_t &y, const uint64_t &z)
{
   return (x & y) ^ (x & z) ^ (y & z);
}

uint64_t SHA::ch(const uint64_t &x, const uint64_t &y, const uint64_t &z)
{
   return (x & y) ^ (~x & z);
}

uint64_t SHA::A(const uint64_t &hash, const uint64_t &x, const uint64_t &y, const uint64_t &z)
{
   return rotateRight(hash, x, 64) ^ rotateRight(hash, y, 64) ^ rotateRight(hash, z, 64);
}

uint64_t SHA::B(const uint64_t &word, const uint64_t &x, const uint64_t &y, const uint64_t &z)
{
   return rotateRight(word, x, 64) ^ rotateRight(word, y, 64) ^ (word >> z);
}

void SHA::extendWords(WordsContainer &words, const uint8_t max_size)
{
   words.reserve(max_size - 16);
   for (uint8_t j = 16; j < max_size; ++j)
   {
      const uint32_t x = rotateRight(words[j - 15], 7, 32) ^ rotateRight(words[j - 15], 18, 32) ^ (words[j - 15] >> 3);
      const uint32_t y = rotateRight(words[j - 2], 19, 32) ^ rotateRight(words[j - 2], 17, 32) ^ (words[j - 2] >> 10);
      words.push_back(words[j - 16] + x + words[j - 7] + y);
   }
}

void SHA::extendDWords(DWordsContainer &dwords, const uint8_t max_size)
{
   for (uint8_t j = 16; j < max_size; ++j)
   {
      dwords[j] = dwords[j - 16] + B(dwords[j - 15], 1, 8, 7) + dwords[j - 7] + B(dwords[j - 2], 19, 61, 6);
   }
}

const HashFunction::BytesContainer
SHA::process(const BytesContainer &data, WordsContainer &IV, const uint8_t truncate_to)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);

      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendWords(words, 64);

      WordsContainer hash(IV);
      for (uint8_t j = 0; j < 64; ++j)
      {
         const uint32_t S1 = rotateRight(hash[4], 6, 32) ^ rotateRight(hash[4], 11, 32) ^ rotateRight(hash[4], 25, 32);
         const uint32_t ch = (hash[4] & hash[5]) ^ (~hash[4] & hash[6]);
         const uint32_t tmp1 = hash[7] + S1 + ch + round_constants[j] + words[j];
         const uint32_t S0 = rotateRight(hash[0], 2, 32) ^ rotateRight(hash[0], 13, 32) ^ rotateRight(hash[0], 22, 32);
         const uint32_t maj = (hash[0] & hash[1]) ^ (hash[0] & hash[2]) ^ (hash[1] & hash[2]);
         const uint32_t tmp2 = S0 + maj;

         hash[7] = hash[6];
         hash[6] = hash[5];
         hash[5] = hash[4];
         hash[4] = hash[3] + tmp1;
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] = hash[0];
         hash[0] = tmp1 + tmp2;
      }

      for (uint8_t j = 0; j < 8; ++j)
      {
         IV[j] += hash[j];
      }
   }

   return getBigEndianOutput(truncate_to, IV);
}

const HashFunction::BytesContainer 
SHA::process512(const BytesContainer &data, DWordsContainer &IV, const uint8_t truncate_to)
{
   BytesContainer bits(appendDWordPadding(data));
   const uint64_t bits_len = bits.size();

   for (uint64_t i = 0; i < bits_len; i += 128)
   {
      DWordsContainer words = getBigEndianDWordBlocks(bits, i);
      words.resize(80);
      
      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendDWords(words, 80);
      
      DWordsContainer hash(IV);
      for (uint8_t j = 0; j < 80; ++j)
      {
         const uint64_t tmp1 = hash[7] + A(hash[4], 14, 18, 41) + ch(hash[4], hash[5], hash[6]) + first_cubic_root_primes[j] + words[j];
         const uint64_t tmp2 = A(hash[0], 28, 34, 39) + maj(hash[0], hash[1], hash[2]);

         hash[7] = hash[6];
         hash[6] = hash[5];
         hash[5] = hash[4];
         hash[4] = hash[3] + tmp1;
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] = hash[0];
         hash[0] = tmp1 + tmp2; 
      }

      for (uint8_t j = 0; j < 8; ++j)
      {
         IV[j] += hash[j];
      }
   }

   return getBigEndianDWordOutput(truncate_to, IV);
}

const HashFunction::BytesContainer 
SHA::appendDWordPadding(const BytesContainer &data) const
{
   const uint64_t bytes_len = data.size() << 3;
   BytesContainer bytes_pad(data);
   bytes_pad.reserve((bytes_len >> 3) + 256);

   // Append a bit '1' at the end.
   bytes_pad.push_back(0x80);

   // Pad with '0' bits at the end of bits_pad until the length is 896 (mod 1024).
   // Also append high order 64-bit from the 128-bit initial length.
   const uint8_t bytes_pad_len = (240 - (bytes_pad.size() & 0x7F)) & 0x7F;
   bytes_pad.insert(bytes_pad.end(), bytes_pad_len + 8, 0);

   // Append the 128-bit of the initial bits length of data in BIG ENDIAN. 
   for (int8_t i = 56; i >= 0; i -= 8)
   {
      bytes_pad.push_back((bytes_len >> i) & 0xFF);
   }
   
   return bytes_pad;
}

const HashFunction::DWordsContainer 
SHA::getBigEndianDWordBlocks(const BytesContainer &bytes, const uint64_t &block_index)
{
   DWordsContainer words;
   words.reserve(16);

   for (uint8_t k = 0; k < 128; k += 8)
   {      
      uint64_t dword = 0;
      for(uint8_t i = 0; i < 8; ++i)
      {
         dword |= static_cast<uint64_t>(bytes[k + block_index + i]) << ((7-i) << 3);
      }
      
      words.push_back(dword);
   }
   
   return words;
}

const HashFunction::BytesContainer 
SHA::getBigEndianDWordOutput(const uint16_t max_words, const DWordsContainer &hash)
{
   BytesContainer output;
   output.reserve(max_words << 2);

   const uint16_t max_64bits = max_words >> 1;
   for (uint8_t j = 0; j < max_64bits; ++j)
   {
      for(int8_t i = 56; i >= 0; i -= 8)
      {
         output.push_back((hash[j] >> i) & 0xFF);
      }
   }
   
   if(max_words % 2)
   {
      for(uint8_t i = 56; i >= 32; i -= 8)
      {
         output.push_back((hash[max_64bits] >> i) & 0xFF);
      }
   }

   return output;
}

void SHA1::extendWords(WordsContainer &words, const uint8_t max_size)
{
   words.reserve(max_size - 16);
   for (uint8_t j = 16; j < max_size; ++j)
   {
      words.push_back(rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32));
   }
}

const HashFunction::BytesContainer SHA1::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);

      // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
      extendWords(words, 80);

      WordsContainer hash(IV);
      uint32_t f, k;
      for (uint8_t j = 0; j < 80; ++j)
      {
         if (j < 20)
         {
            f = ch(hash[1], hash[2], hash[3]);
            k = 0x5A827999;
         }
         else if (j < 40)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0x6ED9EBA1;
         }
         else if (j < 60)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = 0x8F1BBCDC;
         }
         else
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0xCA62C1D6;
         }

         const uint32_t tmp = rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
         hash[4] = hash[3];
         hash[3] = hash[2];
         hash[2] = rotateLeft(hash[1], 30, 32);
         hash[1] = hash[0];
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 5; ++j)
      {
         IV[j] += hash[j];
      }
   }

   return getBigEndianOutput(5, IV);
}

const HashFunction::BytesContainer SHA224::encode(const BytesContainer &data)
{
   return process(data, IV, 7);
}

const HashFunction::BytesContainer SHA256::encode(const BytesContainer &data)
{
   return process(data, IV, 8);
}

const HashFunction::BytesContainer SHA384::encode(const BytesContainer &data)
{
   return process512(data, IV, 12);
}

const HashFunction::BytesContainer SHA512::encode(const BytesContainer &data)
{
   return process512(data, IV, 16);
}

const HashFunction::BytesContainer SHA512_224::encode(const BytesContainer &data)
{
   buildIV({0x32, 0x32, 0x34});
   
   return process512(data, IV, 7);
}

const HashFunction::BytesContainer SHA512_256::encode(const BytesContainer &data)
{
   buildIV({0x32, 0x35, 0x36});
   
   return process512(data, IV, 8);
}

void SHA512_t::buildIV(const BytesContainer &t)
{
   IV.reserve(8);
   SHA512 *S = new SHA512();
   const DWordsContainer IV_512 = S->getIV();
   
   DWordsContainer tmp_IV;
   tmp_IV.reserve(8);
   for(uint8_t i = 0; i < 8; ++i)
   {
      tmp_IV.push_back(IV_512[i] ^ 0xa5a5a5a5a5a5a5a5); 
   }
   
   S->setIV(tmp_IV);
   
   BytesContainer data = {0x53, 0x48, 0x41, 0x2D, 0x35, 0x31, 0x32, 0x2F};
   data.reserve(11);
   data.insert(data.end(), t.begin(), t.end());
   const BytesContainer answer = S->encode(data);
   delete S;
   
   for(uint8_t j = 0; j < 64; j += 8)
   {
      uint64_t dword = 0;
      for(uint8_t i = 0; i < 8; ++i)
      {
         dword |= static_cast<uint64_t>(answer[i + j]) << ((7-i) << 3);
      }

      IV.push_back(dword);
   }
}