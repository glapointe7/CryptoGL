#include "DES.hpp"

#include "Tools.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

// Left rotation table for the 16 rounds.
const uint8_t DES::rot_table[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// First table : initial permutation.
const uint8_t DES::IP[64] = {
   58, 50, 42, 34, 26, 18, 10, 2,
   60, 52, 44, 36, 28, 20, 12, 4,
   62, 54, 46, 38, 30, 22, 14, 6,
   64, 56, 48, 40, 32, 24, 16, 8,
   57, 49, 41, 33, 25, 17, 9, 1,
   59, 51, 43, 35, 27, 19, 11, 3,
   61, 53, 45, 37, 29, 21, 13, 5,
   63, 55, 47, 39, 31, 23, 15, 7
};

// 2nd table : inverse initial permutation.
const uint8_t DES::IP_inverse[64] = {
   40, 8, 48, 16, 56, 24, 64, 32,
   39, 7, 47, 15, 55, 23, 63, 31,
   38, 6, 46, 14, 54, 22, 62, 30,
   37, 5, 45, 13, 53, 21, 61, 29,
   36, 4, 44, 12, 52, 20, 60, 28,
   35, 3, 43, 11, 51, 19, 59, 27,
   34, 2, 42, 10, 50, 18, 58, 26,
   33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion table.
const uint8_t DES::E[48] = {
   32, 1, 2, 3, 4, 5,
   4, 5, 6, 7, 8, 9,
   8, 9, 10, 11, 12, 13,
   12, 13, 14, 15, 16, 17,
   16, 17, 18, 19, 20, 21,
   20, 21, 22, 23, 24, 25,
   24, 25, 26, 27, 28, 29,
   28, 29, 30, 31, 32, 1
};

// Permutation table.
const uint8_t DES::P[32] = {
   16, 7, 20, 21,
   29, 12, 28, 17,
   1, 15, 23, 26,
   5, 18, 31, 10,
   2, 8, 24, 14,
   32, 27, 3, 9,
   19, 13, 30, 6,
   22, 11, 4, 25
};

// Substitution tables.
const uint8_t DES::S[8][4][16] = {
   {
      {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
      {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
      {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
      {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
   },
   {
      {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
      {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
      {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
      {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
   },
   {
      {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
      {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
      {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
      {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
   },
   {
      {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
      {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
      {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
      {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
   },
   {
      {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
      {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
      {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
      {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
   },
   {
      {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
      {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
      {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
      {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
   },
   {
      {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
      {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
      {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
      {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
   },
   {
      {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
      {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
      {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
      {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
   }
};

// Permutation choice table 1.
const uint8_t DES::PC1[56] = {
   57, 49, 41, 33, 25, 17, 9,
   1, 58, 50, 42, 34, 26, 18,
   10, 2, 59, 51, 43, 35, 27,
   19, 11, 3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
   7, 62, 54, 46, 38, 30, 22,
   14, 6, 61, 53, 45, 37, 29,
   21, 13, 5, 28, 20, 12, 4
};

// Permutation choice table 2.
const uint8_t DES::PC2[48] = {
   14, 17, 11, 24, 1, 5,
   3, 28, 15, 6, 21, 10,
   23, 19, 12, 4, 26, 8,
   16, 7, 27, 20, 13, 2,
   41, 52, 31, 37, 47, 55,
   30, 40, 51, 45, 33, 48,
   44, 49, 39, 56, 34, 53,
   46, 42, 50, 36, 29, 32
};

void DES::setKey(const BytesContainer &key)
{
   if (key.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key.size());
   }

   this->key = key;
}

void DES::generateSubkeys()
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
   subkeys.reserve(16);
   for (uint8_t i = 0; i < 16; ++i)
   {
      // Rotation à gauche de la partie droite et de la partie gauche.
      const uint64_t KR = rotateLeft(K1, rot_table[i], 28);
      const uint64_t KL = rotateLeft(K2, rot_table[i], 28);
      K1 = KR;
      K2 = KL;

      const uint64_t K = KR | (KL << 28);

      // On permute K (56 bits) selon la table PC2 pour obtenir la sous-clé (48 bits).
      subkeys.push_back(getBitsFromTable(K, PC2, 56, 48));
   }
}

// À partir d'un bloc de 48 bits séparé en 8 blocs de 6 bits, on les substitue
// à l'aide des 8 tables de substitution.
// La sortie est 8 blocs de 4 bits concaténés (32 bits).

uint64_t DES::getSubstitution(const uint64_t &key_mixed) const
{
   // on divise key_mixed en 8 blocs de 6 bits chacun.
   uint64_t sboxes[8];
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

void DES::encodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t round) const
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      const uint64_t Li = R;
      const uint64_t Ri = L ^ F(R, subkeys[i]);
      L = Li;
      R = Ri;
   }
}

void DES::decodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t) const
{
   for (int8_t i = rounds - 1; i >= 0; --i)
   {
      const uint64_t Li = R;
      const uint64_t Ri = L ^ F(R, subkeys[i]);
      L = Li;
      R = Ri;
   }
}

const DES::BytesContainer 
DES::getOutputBlock(const BytesContainer &data, const bool to_encode)
{
   uint64_t value = 0;
   for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
   {
      const uint64_t x = data[j];
      value |= (x << i);
   }

   // Initial permutation of the 64-bits data blocks.
   const uint64_t ip_data = getBitsFromTable(value, IP, 64, 64);

   // Split the 64-bits block in 2 blocks of 32 bits L and R for the 16 Feistel rounds.
   uint64_t L = (ip_data >> 32) & 0xFFFFFFFF;
   uint64_t R = ip_data & 0xFFFFFFFF;
   if (to_encode)
   {
      encodeFeistelRounds(L, R, 0);
   }
   else
   {
      decodeFeistelRounds(L, R, 0);
   }
   // On effectue la permutation finale de R16.L16. avec la table IP_inverse.
   const uint64_t RL = (R << 32) | L;
   const uint64_t output = getBitsFromTable(RL, IP_inverse, 64, 64);

   // Transform the encoded / decoded block to 8 blocks of 8 bits.
   BytesContainer output_block(8, 0);
   for (int8_t j = 7, i = 0; j >= 0; --j, i += 8)
   {
      output_block[j] = (output >> i) & 0xFF;
   }

   return output_block;
}

const DES::BytesContainer DES::encode(const BytesContainer &clear_text)
{
   // Pad with 0x00 to get a multiple of 64 bits.
   return process(addPadding(clear_text, 8, 0), 8, true);
}

const DES::BytesContainer DES::decode(const BytesContainer &cipher_text)
{
   return process(cipher_text, 8, false);
}