#include "DES.hpp"

#include "Tools.h"

#include <algorithm>

// La clé doit être de 64 bits (8 octets).

void DES::setKey(const BytesContainer &key)
{
   this->key = key;
}

DES::UInt64Container DES::getKeySchedule()
{  
   unsigned char j = 0;
   uint64_t key_bits = 0;
   for(unsigned char i = 0; i < 8; ++i, j += 8)
   {
      key_bits |= (key[i] << j);
   }

   // On effectue la permutation PC1 pour avoir une clé de 56 bits.
   uint64_t K64 = getBitsFromTable<uint64_t, uint64_t>(key_bits, PC1);

   // On sépare la clé de 56 bits en 2 clés de 28 bits.
   uint32_t K1, K2;
   K1 = (K64 & 0xFFFFFFF);
   K2 = (K64 & 0xFFFFFFF0000000) >> 28;

   // On effectue une rotation à gauche de 1 ou 2 bits selon la table rot_table.
   // Ensuite, on concatène K1 et K2, puis on permute les bits selon la table PC2.
   // on effectue l'algorithme 16 fois pour générer les 16 sous-clés de 48 bits chacune.
   std::vector<uint64_t> subkeys(16, 0);
   for (unsigned char i = 0; i < 16; ++i)
   {
      K1 = rotateLeft(K1, rot_table[i], 28);
      K2 = rotateLeft(K2, rot_table[i], 28);

      // TO DO : RÉDUIRE LE CODE ICI.
      // On permute selon les 4 premières lignes de PC2 (24 bits).
      uint32_t pc2_1 = getBitsFromTable<uint32_t, uint64_t>(K64, PC2[0]);
      /*uint32_t pc2_1 = 0;
      for (unsigned char j = 0; j < 4; ++j)
      {
         for (auto byte : PC2[j])
         {
            if (getBitAtPosition(byte, K64) == true)
            {
               pc2_1 = setBit(pc2_1, byte - 1);
            }
         }
      }*/

      // On permute selon les 4 dernières lignes de PC2 (24 bits).
      uint32_t pc2_2 = 0;
      for (auto row : PC2[1])
      {
         for (auto byte : row)
         {
            if (getBitAtPosition(byte, K64) == true)
            {
               pc2_2 = setBit(pc2_2, byte - 29);
            }
         }
      }

      subkeys[i] = (pc2_2 << 24) | pc2_1;
   }

   return subkeys;
}

// Expension du data en un block de 48 bits à l'aide de la table d'expension E. 

/*uint64_t DES::expendBlock(const uint32_t data) const
{
   uint64_t E_block = 0;
   for (auto row : E)
   {
      for (auto byte : row)
      {
         if (getBitAtPosition(byte, data) == true)
         {
            E_block = setBit(E_block, byte - 1);
         }
      }
   }

   return E_block;
}*/

uint32_t DES::getSubstitution(const uint64_t &key_mixed) const
{
   // on divise key_mixed en 8 blocs de 6 bits chacun.
   BytesContainer sboxes(8, 0);
   const uint8_t max_6bits = 0x3F;
   uint8_t j = 0;
   for (uint8_t i = 0; i < 8; ++i, j += 6)
   {
      sboxes[i] = (key_mixed >> j) & max_6bits;
   }

   // Permutations des 6 bits selon les tables S[i] pour i=0,...,7.
   // Soit un bloc de 6 bits (a,b,c,d,e,f).
   // On définit row = a.f et col = b.c.d.e où . désigne la concaténation.
   uint32_t s_block = 0;
   j = 0;
   for (unsigned char i = 0; i < 8; ++i, j += 4)
   {
      uint8_t row = (sboxes[i] & 0x1) | ((sboxes[i] >> 5) << 1),
              col = (sboxes[i] >> 1) & 0xF;
      s_block |= (S[i][row][col] << j);
   }

   return s_block;
}

/*uint32_t DES::getOutput(const uint32_t s_block) const
{
   uint32_t output = 0;
   for (auto row : P)
   {
      for (auto byte : row)
      {
         if (getBitAtPosition(byte, s_block) == true)
         {
            output = setBit(output, byte - 1);
         }
      }
   }

   return output;
}*/

// Fonction F de Feistel recevant un block de données de 32 bits et une sous-clé de 48 bits.

uint32_t DES::F(const uint32_t data, const uint64_t &subkey) const
{
   // Expension du data de 32 bits à 48 bits avec la table E.
   const uint64_t E_block = getBitsFromTable<uint64_t, uint32_t>(data, E);
   //const uint64_t E_block = expendBlock(data);

   // On effectue un XOR avec le bloc E_block et la sous-clé.
   const uint64_t key_mixed = E_block ^ subkey;

   // On effectue les substitutions avec les 8 tables de S.
   const uint32_t s_block = getSubstitution(key_mixed);

   // Permutation finale avant la sortie des 32 bits avec la table P.
   return getBitsFromTable<uint32_t, uint32_t>(s_block, P);
   //return getOutput(s_block);
}

// Permutation initiale des 64 bits de données selon la table IP.

/*uint64_t DES::getInitialPermutation(const uint64_t &data) const
{
   uint64_t ip_data = 0;
   for (auto row : IP)
   {
      for (auto byte : row)
      {
         if (getBitAtPosition(byte, data) == true)
         {
            ip_data = setBit(ip_data, byte - 1);
         }
      }
   }

   return ip_data;
}

uint64_t DES::getFinalPermutation(const uint64_t &data) const
{
   uint64_t ip_data = 0;
   for (auto row : IP_inverse)
   {
      for (auto byte : row)
      {
         if (getBitAtPosition(byte, data) == true)
         {
            ip_data = setBit(ip_data, byte - 1);
         }
      }
   }

   return ip_data;
}*/

const DES::BytesContainer DES::encode(const BytesContainer &clear_text)
{
   // On ajoute des 0x00 pour avoir un multiple de 8 octets.
   BytesContainer full_text(clear_text);
   full_text.insert(full_text.end(), 8 - (clear_text.size() % 8), 0);
   uint32_t clear_len = full_text.size();

   BytesContainer crypted(clear_len, 0);
   for (uint32_t n = 0; n < clear_len; n += 8)
   {
      // Permutation initiale des blocs de 64 bits de données.
      uint64_t data = 0;
      unsigned char i = 0;
      for (char j = 7; j >= 0; --j, i += 8)
      {
         data |= (full_text[j+n] << i);
      }
      uint64_t ip_data = getBitsFromTable<uint64_t, uint64_t>(data, IP);
      //uint64_t ip_data = getInitialPermutation(data);

      // On sépare un bloc de 64 bits en 2 blocs de 32 bits.
      uint32_t L = (ip_data >> 32) & 0xFFFFFFFF;
      uint32_t R = ip_data & 0xFFFFFFFF;

      // On obtient les 16 sous-clés de 48 bits chacune.
      UInt64Container subkeys = getKeySchedule();

      // On effectue les 16 rounds de Feistel.
      for (unsigned char round = 0; round < 16; ++round)
      {
         R = L ^ F(R, subkeys[round]);
         std::swap(L, R);
      }
      
      // On effectue la permutation finale de R16.L16. avec la table IP_inverse.
      const uint64_t R16L16 = (R << sizeof(R)) | L;
      const uint64_t output = getBitsFromTable<uint64_t, uint64_t>(R16L16, IP_inverse);
      
      // On transforme le bloc encodé en 8 blocs de 8 bits.
      i = 0;
      for(char j = 7; j >= 0; --j, i += 8)
      {
         crypted[j+n] = (output >> i) & 0xFF;
      }
   }

   return crypted;
}

const DES::BytesContainer DES::decode(const BytesContainer &cipher_text)
{
   BytesContainer decrypted;

   return decrypted;
}