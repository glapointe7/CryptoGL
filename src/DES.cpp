#include "DES.hpp"

#include "Tools.hpp"

#include "exceptions/BadKeyLength.hpp"

void DES::setKey(const BytesContainer &key)
{
   if (key.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key.size());
   }

   this->key = key;
}

const DES::UInt64Container DES::getKeySchedule()
{
   uint64_t key_bits = 0;
   for (uint8_t i = 0, j = 56; i < 8; ++i, j -= 8)
   {
      key_bits |= (static_cast<uint64_t> (key[i]) << j);
   }

   // We permute with the PC1 table to get a 56-bits key.
   const uint64_t K56 = getBitsFromTable(key_bits, PC1, 64, 56);

   // Split the 56-bits key in 2 28-bits sub-keys.
   uint64_t K1, K2;
   K1 = K56 & 0xFFFFFFF;
   K2 = (K56 & 0xFFFFFFF0000000) >> 28;

   // On effectue une rotation à gauche de 1 ou 2 bits selon la table rot_table.
   // Ensuite, on concatène K1 et K2, puis on permute les bits selon la table PC2.
   // on effectue l'algorithme 16 fois pour générer les 16 sous-clés de 48 bits chacune.
   UInt64Container sub_keys(16, 0);
   for (uint8_t i = 0; i < 16; ++i)
   {
      // Rotation à gauche de la partie droite et de la partie gauche.
      const uint64_t KR = rotateLeft(K1, rot_table[i], 28);
      const uint64_t KL = rotateLeft(K2, rot_table[i], 28);
      K1 = KR;
      K2 = KL;

      const uint64_t K = KR | (KL << 28);

      // On permute K (56 bits) selon la table PC2 pour obtenir la sous-clé (48 bits).
      sub_keys[i] = getBitsFromTable(K, PC2, 56, 48);
   }

   return sub_keys;
}

// À partir d'un bloc de 48 bits séparé en 8 blocs de 6 bits, on les substitue
// à l'aide des 8 tables de substitution.
// La sortie est 8 blocs de 4 bits concaténés (32 bits).

uint64_t DES::getSubstitution(const uint64_t &key_mixed) const
{
   // on divise key_mixed en 8 blocs de 6 bits chacun.
   UInt64Container sboxes(8, 0);
   for (uint8_t i = 0, j = 42; i < 8; ++i, j -= 6)
   {
      sboxes[i] = (key_mixed >> j) & 0x3F;
   }

   // Permutations des 6 bits selon les tables S[i] pour i=0,...,7.
   // Soit un bloc de 6 bits (a,b,c,d,e,f).
   // On définit row = a.f et col = b.c.d.e où . désigne la concaténation.
   uint64_t s_block = 0;
   for (uint8_t i = 0, j = 28; i < 8; ++i, j -= 4)
   {
      const uint64_t row = (sboxes[i] & 0x1) | ((sboxes[i] >> 5) << 1);
      const uint64_t col = (sboxes[i] >> 1) & 0xF;
      s_block |= (S[i][row][col] << j);
   }

   return s_block;
}

// Feistel function F 
// Param : 32-bits data and a 48-bits sub-key.

uint64_t DES::F(const uint64_t &data, const uint64_t &subkey) const
{
   // Expension du data de 32 bits à 48 bits avec la table E.
   const uint64_t E_block = getBitsFromTable(data, E, 32, 48);

   // On effectue un XOR avec le bloc E_block et la sous-clé.
   const uint64_t key_mixed = E_block ^ subkey;

   // On effectue les substitutions avec les 8 tables de S. Sortie de 32 bits.
   const uint64_t s_block = getSubstitution(key_mixed);

   // Permutation finale avant la sortie des 32 bits avec la table P.
   return getBitsFromTable(s_block, P, 32, 32);
}

const DES::BytesContainer
DES::getOutputBlock(const BytesContainer &data, const uint32_t block, const int8_t lower_round)
{
   int8_t is_increasing = 1;
   if (lower_round < 0)
   {
      is_increasing = -1;
   }
   
   uint64_t value = 0;
   for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
   {
      const uint64_t x = data[j + block];
      value |= (x << i);
   }

   // Permutation initiale des blocs de 64 bits de données.
   const uint64_t ip_data = getBitsFromTable(value, IP, 64, 64);

   // On sépare un bloc de 64 bits en 2 blocs de 32 bits.
   uint64_t L0 = (ip_data >> 32) & 0xFFFFFFFF;
   uint64_t R0 = ip_data & 0xFFFFFFFF;

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
   const uint64_t output = getBitsFromTable(R16L16, IP_inverse, 64, 64);

   // Transform the encoded / decoded block to 8 blocks of 8 bits.
   BytesContainer output_block(8, 0);
   for (int8_t j = 7, i = 0; j >= 0; --j, i += 8)
   {
      output_block[j] = (output >> i) & 0xFF;
   }
   
   return output_block;
}

const DES::BytesContainer
DES::process(const BytesContainer &data, const int8_t lower_round)
{
   const uint32_t clear_len = data.size();
   BytesContainer toReturn;
   toReturn.reserve(clear_len);

   for (uint32_t n = 0; n < clear_len; n += 8)
   {
      const BytesContainer output = getOutputBlock(data, n, lower_round);
      // if (banane
      // block = strategy->kekchose(poutput)
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }

   return toReturn;
}

const DES::BytesContainer DES::encode(const BytesContainer &clear_text)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }

   // Pad with 0x00 to get a multiple of 64 bits.
   BytesContainer full_text(clear_text);
   const uint8_t rest = clear_text.size() % 8;
   if (rest != 0)
   {
      full_text.insert(full_text.end(), 8 - rest, 0);
   }

   return process(full_text, 0);
}

const DES::BytesContainer DES::decode(const BytesContainer &cipher_text)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }

   return process(cipher_text, -15);
}
