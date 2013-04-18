#include "DES.hpp"

#include "Tools.h"

// La clé doit être de 64 bits (8 octets).

void DES::setKey(const BytesContainer &key)
{
   this->key = key;
}

DES::UInt64Container DES::getKeySchedule()
{  
   unsigned char j = 64;
   uint64_t key_bits = 0;
   uint64_t x;
   for(unsigned char i = 0; i < 8; ++i)
   {
      x = key[i];
      j-=8;
      key_bits |= (x << j);
   }

   // On effectue la permutation PC1 pour avoir une clé de 56 bits.
   const uint64_t K56 = getBitsFromTable(key_bits, PC1, 64, 56);

   // On sépare la clé de 56 bits en 2 clés de 28 bits.
   uint64_t K1, K2;
   K1 = K56 & 0xFFFFFFF;
   K2 = (K56 & 0xFFFFFFF0000000) >> 28;

   // On effectue une rotation à gauche de 1 ou 2 bits selon la table rot_table.
   // Ensuite, on concatène K1 et K2, puis on permute les bits selon la table PC2.
   // on effectue l'algorithme 16 fois pour générer les 16 sous-clés de 48 bits chacune.
   std::vector<uint64_t> subkeys(16, 0);
   for (unsigned char i = 0; i < 16; ++i)
   {
      // Rotation à gauche de la partie droite et de la partie gauche.
      const uint64_t KR = rotateLeft(K1, rot_table[i], 28);
      const uint64_t KL = rotateLeft(K2, rot_table[i], 28);
      K1 = KR;
      K2 = KL;

      // On concatène KL et KR.
      const uint64_t K = KR | (KL << 28);
      
      // On permute K (56 bits) selon la table PC2 pour obtenir la sous-clé (48 bits).
      subkeys[i] = getBitsFromTable(K, PC2, 56, 48);
   }

   return subkeys;
}

// À partir d'un bloc de 48 bits séparé en 8 blocs de 6 bits, on les substitue
// à l'aide des 8 tables de substitution.
// La sortie est 8 blocs de 4 bits concaténés (32 bits).
uint64_t DES::getSubstitution(const uint64_t &key_mixed) const
{
   // on divise key_mixed en 8 blocs de 6 bits chacun.
   UInt64Container sboxes(8, 0);
   const unsigned char max_6bits = 0x3F;
   unsigned char j = 48;
   for (unsigned char i = 0; i < 8; ++i)
   {
      j -= 6;
      sboxes[i] = (key_mixed >> j) & max_6bits;
   }

   // Permutations des 6 bits selon les tables S[i] pour i=0,...,7.
   // Soit un bloc de 6 bits (a,b,c,d,e,f).
   // On définit row = a.f et col = b.c.d.e où . désigne la concaténation.
   uint64_t s_block = 0;
   j = 32;
   for (unsigned char i = 0; i < 8; ++i)
   {
      j -= 4;
      uint64_t row = (sboxes[i] & 0x1) | ((sboxes[i] >> 5) << 1),
              col = (sboxes[i] >> 1) & 0xF;
      s_block |= (S[i][row][col] << j);
   }

   return s_block;
}

// Fonction F de Feistel recevant un block de données de 32 bits et une sous-clé de 48 bits.

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

const DES::BytesContainer DES::encode(const BytesContainer &clear_text)
{
   // On ajoute des 0x00 pour avoir un multiple de 8 octets.
   BytesContainer full_text(clear_text);
   unsigned char rest = clear_text.size() % 8;
   if(rest > 0)
   {
      full_text.insert(full_text.end(), 8 - rest, 0);
   }
   uint32_t clear_len = full_text.size();

   BytesContainer crypted(clear_len, 0);
   uint64_t x;
   for (uint32_t n = 0; n < clear_len; n += 8)
   {
      uint64_t data = 0;
      unsigned char i = 64;
      for (unsigned char j = 0; j < 8; ++j)
      {
         i -= 8;
         x = full_text[j+n];
         data |= (x << i);
      }
      
      // Permutation initiale des blocs de 64 bits de données.
      const uint64_t ip_data = getBitsFromTable(data, IP, 64, 64);

      // On sépare un bloc de 64 bits en 2 blocs de 32 bits.
      uint64_t L0 = (ip_data >> 32) & 0xFFFFFFFF;
      uint64_t R0 = ip_data & 0xFFFFFFFF;

      // On obtient les 16 sous-clés de 48 bits chacune.
      const UInt64Container subkeys = getKeySchedule();

      // On effectue les 16 rounds de Feistel.
      uint64_t R, L;
      for (unsigned char round = 0; round < 16; ++round)
      {
         L = R0;
         R = L0 ^ F(R0, subkeys[round]);
         L0 = L;
         R0 = R;
      }
      
      // On effectue la permutation finale de R16.L16. avec la table IP_inverse.
      const uint64_t R16L16 = (R << 32) | L;
      const uint64_t output = getBitsFromTable(R16L16, IP_inverse, 64, 64);
      
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