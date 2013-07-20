#include "RC6.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

#include "Tools.hpp"
#include "LittleEndian.hpp"

void RC6::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16 && key_len != 24 && key_len != 32)
   {
      throw BadKeyLength("Your key has to be 16, 24 or 32 bytes length.", key_len);
   }

   this->key = key;
}

void RC6::generateSubkeys()
{
   const uint8_t int_size = 4;
   const uint8_t key_len = key.size();
   const uint8_t tmp_key_len = (key_len + int_size - 1) / int_size;
   UInt32Container tmp_key;
   tmp_key.reserve(tmp_key_len);

   LittleEndian32 *LE = new LittleEndian32();
   for (uint8_t i = 0; i < key_len; i += int_size)
   {
      LE->toInteger(BytesContainer(key.begin() + i, key.begin() + i + int_size));
      tmp_key.push_back(LE->getValue());
      LE->resetValue();
   }
   delete LE;

   // Initialize the expanded key table.
   const uint8_t subkeys_len = (rounds + 2) << 1;
   subkeys.resize(subkeys_len);
   subkeys[0] = P32;
   for (uint8_t i = 1; i < subkeys_len; ++i)
   {
      subkeys[i] = (subkeys[i - 1] + Q32) & 0xFFFFFFFF;
   }

   // This step mixes the secret key into the expanded key 'subkeys'.
   const uint8_t k = 3 * std::max(tmp_key_len, subkeys_len);
   uint32_t L = 0, R = 0;
   for (uint8_t l = 0, i = 0, j = 0; l < k; ++l)
   {
      L = subkeys[i] = rotateLeft((subkeys[i] + L + R) & 0xFFFFFFFF, 3, 32);
      R = tmp_key[j] = rotateLeft(tmp_key[j] + L + R, (L + R) & 31, 32);
      i = (i + 1) % subkeys_len;
      j = (j + 1) % tmp_key_len;
   }
}

uint64_t RC6::F(const uint64_t &X, const uint64_t&) const
{
   return rotateLeft((X * ((X << 1) + 1)) & 0xFFFFFFFF, 5, 32);
}

void RC6::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   uint32_t A = L >> 32;
   uint32_t B = L & 0xFFFFFFFF;
   uint32_t C = R >> 32;
   uint32_t D = R & 0xFFFFFFFF;
   
   B += subkeys[0];
   D += subkeys[1];
   for (uint8_t i = 1; i <= rounds; ++i)
   {
      const uint8_t j = i << 1;
      uint32_t t = F(B, 0);
      const uint32_t u = F(D, 0);
      A = rotateLeft(A ^ t, u & 31, 32) + subkeys[j];
      C = rotateLeft(C ^ u, t & 31, 32) + subkeys[j + 1];
      t = A; A = B; B = C; C = D; D = t;
   }
   A += subkeys[(rounds << 1) + 2];
   C += subkeys[(rounds << 1) + 3];
   
   L = (static_cast<uint64_t>(A) << 32) | B;
   R = (static_cast<uint64_t>(C) << 32) | D;
}

void RC6::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   uint32_t A = L >> 32;
   uint32_t B = L & 0xFFFFFFFF;
   uint32_t C = R >> 32;
   uint32_t D = R & 0xFFFFFFFF;
   
   C -= subkeys[(rounds << 1) + 3];
   A -= subkeys[(rounds << 1) + 2];
   for (uint8_t i = rounds; i > 0; --i)
   {
      const uint8_t j = i << 1;
      uint32_t t = A; A = D; D = C; C = B; B = t;
      const uint32_t u = F(D, 0);
      t = F(B, 0);
      C = rotateRight((C - subkeys[j + 1]) & 0xFFFFFFFF, t & 31, 32) ^ u;
      A = rotateRight((A - subkeys[j]) & 0xFFFFFFFF, u & 31, 32) ^ t;
   }
   D -= subkeys[1];
   B -= subkeys[0];
   
   L = (static_cast<uint64_t>(A) << 32) | B;
   R = (static_cast<uint64_t>(C) << 32) | D;
}

const RC6::BytesContainer RC6::getOutputBlock(const BytesContainer &block, const bool to_encode)
{
   LittleEndian32 *LE = new LittleEndian32();
   uint32_t registers[4];
   for(uint8_t i = 0; i < 16; i += 4)
   {
      LE->toInteger(BytesContainer(block.begin() + i, block.begin() + i + 4));
      registers[i >> 2] = LE->getValue();
      LE->resetValue();
   }
   delete LE;

   uint64_t L = (static_cast<uint64_t>(registers[0]) << 32) | registers[1];
   uint64_t R = (static_cast<uint64_t>(registers[2]) << 32) | registers[3];
   if (to_encode)
   {
      encodeFeistelRounds(L, R, 0);
   }
   else
   {
      decodeFeistelRounds(L, R, 0);
   }
   registers[0] = L >> 32;
   registers[1] = L & 0xFFFFFFFF;
   registers[2] = R >> 32;
   registers[3] = R & 0xFFFFFFFF;

   // Convert 32-bit integers to 8-bit integers output.
   BytesContainer output;
   output.reserve(16);
   LittleEndian32 *LE_32 = new LittleEndian32();
   for(uint8_t i = 0; i < 4; ++i)
   {
      LE_32->toBytes(registers[i]);
      const BytesContainer tmp = LE_32->getBytes();
      output.insert(output.end(), tmp.begin(), tmp.end());
   }
   delete LE_32;

   return output;
}

const RC6::BytesContainer RC6::encode(const BytesContainer &clear_text)
{
   return processEncoding(clear_text);
}

const RC6::BytesContainer RC6::decode(const BytesContainer &cipher_text)
{
   return processDecoding(cipher_text);
}