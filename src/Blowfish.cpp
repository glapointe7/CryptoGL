#include "Blowfish.hpp"

#include "exceptions/BadKeyLength.hpp"

void Blowfish::setKey(const BytesContainer &key)
{
   if (key.size() < 6 || key.size() > 56)
   {
      throw BadKeyLength("Your key length have to be between 6 bytes and 56 bytes.", key.size());
   }

   this->key = key;
}

const Blowfish::UInt64Container Blowfish::getKeySchedule()
{
   UInt64Container subkeys(18, 0);
   //const uint32_t key_len = key.size();

   // on subdivise la clé en blocs de 32 bits.


   return subkeys;
}

uint64_t Blowfish::F(const uint64_t &data, const uint64_t &subkey) const
{
   BytesContainer V;
   V.reserve(4);
   const uint64_t x = data ^ subkey;
   V.push_back((x >> 24) & 0xFF);
   V.push_back((x >> 16) & 0xFF);
   V.push_back((x >> 8) & 0xFF);
   V.push_back(x & 0xFF);
   
   return (((sbox0[V[0]] + sbox1[V[1]]) & 0xFFFFFFFF) ^ (sbox2[V[2]])) + (sbox3[V[3]] & 0xFFFFFFFF);
}

const Blowfish::BytesContainer 
Blowfish::getOutputBlock(const BytesContainer &data, const int8_t lower_round)
{
   int8_t is_increasing = 1;
   if (lower_round < 0)
   {
      is_increasing = -1;
   }
   
   uint64_t value = 0;
   for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
   {
      const uint64_t x = data[j];
      value |= (x << i);
   }
   
   uint64_t L0 = (value >> 32) & 0xFFFFFFFF;
   uint64_t R0 = value & 0xFFFFFFFF;

   // On obtient les 16 sous-clés de 48 bits chacune.
   const UInt64Container subkeys = getKeySchedule();

   // Process the 16 Feistel rounds.
   uint64_t R, L;
   for (int8_t round = lower_round; round <= lower_round + 15; ++round)
   {
      L = R0;
      R = L0 ^ F(R0, subkeys[round * is_increasing]);
      L0 = L;
      R0 = R;
   }

   // On effectue la permutation finale de R16.L16. avec la table IP_inverse.
   const uint64_t R16L16 = (R << 32) | L;
   
   // Transform the encoded / decoded block to 8 blocks of 8 bits.
   BytesContainer output_block(8, 0);
   for (int8_t j = 7, i = 0; j >= 0; --j, i += 8)
   {
      output_block[j] = (R16L16 >> i) & 0xFF;
   }
   
   return output_block;
}

const Blowfish::BytesContainer Blowfish::encode(const BytesContainer &clear_text)
{
   BytesContainer crypted;
   const uint32_t clear_len = clear_text.size();
   crypted.reserve(clear_len);

   for (uint32_t block = 0; block < clear_len; block += 8)
   {
      uint64_t value = 0;
      for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
      {
         const uint64_t x = clear_text[j + block];
         value |= (x << i);
      }

      // Split the 64-bits block in 2 blocks of 32 bits.
      uint64_t L0 = (value >> 32) & 0xFFFFFFFF;
      uint64_t R0 = value & 0xFFFFFFFF;

      const UInt64Container subkeys = getKeySchedule();
      
      // Process the 16 Feistel rounds.
      uint64_t R, L;
      for (int8_t round = 0; round <= 15; ++round)
      {
         L = R0;
         R = L0 ^ F(R0, subkeys[round]);
         L0 = L;
         R0 = R;
      }
      
      L = R0 ^ subkeys[16];
      R = L0 ^ subkeys[17];
      //const uint64_t R16L16 = (R << 32) | L;
   }

   return crypted;
}

const Blowfish::BytesContainer Blowfish::decode(const BytesContainer &cipher_text)
{
   BytesContainer decrypted;

   return decrypted;
}