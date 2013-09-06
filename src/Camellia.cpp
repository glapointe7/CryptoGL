#include "Camellia.hpp"

#include "BigEndian.hpp"
#include "Bits.hpp"
#include "Tools.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint64_t Camellia::key_sigma[];
constexpr uint32_t Camellia::SP1110[256];
constexpr uint32_t Camellia::SP0222[256];
constexpr uint32_t Camellia::SP3033[256];
constexpr uint32_t Camellia::SP4404[256];

void Camellia::setKey(const BytesVector &key)
{
   if (key.size() != 16 && key.size() != 24 && key.size() != 32)
   {
      throw BadKeyLength("Your key length has to be 16, 24 or 32 bytes.", key.size());
   }
   
   if(key.size() == 16)
   {
      rounds = 18;
   }

   this->key = key;
}

void Camellia::generateSubkeys()
{
   BytesVector Kr, Kl;
   Kr.reserve(16);
   Kl.reserve(16);
   
   switch(key.size())
   {
      case 16:
         Kl = key;
         Kr.insert(Kr.end(), 16, 0);
         Ke.reserve(4);
         break;
         
      case 24:
         Kl.insert(Kl.end(), key.begin(), key.begin() + 16);
         Kr.insert(Kr.end(), key.begin() + 16, key.end());
         for(uint8_t i = 0; i < 8; ++i)
         {  // NOT(x AND t) = NOT(x) AND t = x XOR t, where t is tautology.
            Kr.push_back(Kr[i] ^ 0xFF);
         }
         Ke.reserve(6);
         break;
         
      case 32:
         Kl.insert(Kl.end(), key.begin(), key.begin() + 16);
         Kr.insert(Kr.end(), key.begin() + 16, key.end());
         Ke.reserve(6);
         break;
   }
   
   BytesVector K = getXORedBlock(Kl, Kr);
   uint64_t K1 = BigEndian64::toInteger(BytesVector(K.begin(), K.begin() + 8));
   uint64_t K2 = BigEndian64::toInteger(BytesVector(K.begin() + 8, K.end()));
   K2 ^= F(K1 ^ key_sigma[0], 0);
   K1 ^= F(K2 ^ key_sigma[1], 1);
   
   K1 ^= BigEndian64::toInteger(BytesVector(Kl.begin(), Kl.begin() + 8));
   K2 ^= BigEndian64::toInteger(BytesVector(Kl.begin() + 8, Kl.end()));
   K2 ^= F(K1 ^ key_sigma[2], 2);
   K1 ^= F(K2 ^ key_sigma[3], 3);
   
   BytesVector Ka = BigEndian64::toBytesVector(K1);
   Ka.reserve(16);
   BytesVector tmp = BigEndian64::toBytesVector(K2);
   Ka.insert(Ka.end(), tmp.begin(), tmp.end());
         
   /* Prewhitening phase. */
   Kw.reserve(4);
   Kw.push_back(BigEndian64::toInteger(BytesVector(Kl.begin(), Kl.begin() + 8)));
   Kw.push_back(BigEndian64::toInteger(BytesVector(Kl.begin() + 8, Kl.end())));
   subkeys.reserve(rounds);
   
   if(rounds == 18)
   {
      subkeys.push_back(BigEndian64::toInteger(BytesVector(Ka.begin(), Ka.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(Ka.begin() + 8, Ka.end())));

      tmp = Bits::rotateLeft128(Kl, 15);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Ka, 15);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Ka, 30);
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Kl, 45);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Ka, 45);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      tmp = Bits::rotateLeft128(Kl, 60);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Ka, 60);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Kl, 77);
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Kl, 94);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Ka, 94);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));

      tmp = Bits::rotateLeft128(Kl, 111);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      /* Postwhitening phase. */
      tmp = Bits::rotateLeft128(Ka, 111);
      Kw.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Kw.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
   }
   else
   {
      K = getXORedBlock(Ka, Kr);
      K1 = BigEndian64::toInteger(BytesVector(K.begin(), K.begin() + 8));
      K2 = BigEndian64::toInteger(BytesVector(K.begin() + 8, K.end()));
      K2 ^= F(K1 ^ key_sigma[4], 4);
      K1 ^= F(K2 ^ key_sigma[5], 5);

      BytesVector Kb = BigEndian64::toBytesVector(K1);
      Kb.reserve(16);
      tmp = BigEndian64::toBytesVector(K2);
      Kb.insert(Kb.end(), tmp.begin(), tmp.end());
      
      subkeys.push_back(BigEndian64::toInteger(BytesVector(Kb.begin(), Kb.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(Kb.begin() + 8, Kb.end())));

      tmp = Bits::rotateLeft128(Kr, 15);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Ka, 15);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kr, 30);
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kb, 30);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kl, 45);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Ka, 45);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kl, 60);
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kr, 60);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kb, 60);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kl, 77);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Ka, 77);
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Ke.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kr, 94);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Ka, 94);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      tmp = Bits::rotateLeft128(Kl, 111);
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      subkeys.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
      
      /* Postwhitening phase. */
      tmp = Bits::rotateLeft128(Kb, 111);
      Kw.push_back(BigEndian64::toInteger(BytesVector(tmp.begin(), tmp.begin() + 8)));
      Kw.push_back(BigEndian64::toInteger(BytesVector(tmp.begin() + 8, tmp.end())));
   }   
}

// Used technic of P.29 from the specs.
const uint64_t Camellia::F(const uint64_t half_block, const uint8_t) const
{
   uint32_t D = SP1110[half_block & 0xFF] ^ SP0222[(half_block >> 24) & 0xFF] 
           ^ SP3033[(half_block >> 16) & 0xFF] ^ SP4404[(half_block >> 8) & 0xFF];
   
   uint32_t U = SP1110[(half_block >> 56) & 0xFF] ^ SP0222[(half_block >> 48) & 0xFF] 
           ^ SP3033[(half_block >> 40) & 0xFF] ^ SP4404[(half_block >> 32) & 0xFF];
   
   D ^= U;
   U = D ^ Bits::rotateRight(U, 8);
   
   return (static_cast<uint64_t>(D) << 32) | U;
}

uint64_t Camellia::FL(const uint64_t &half_block, const uint64_t &subkey) const
{
   uint32_t Xl = half_block >> 32;
   uint32_t Xr = half_block & 0xFFFFFFFF;
   const uint32_t Kl = subkey >> 32;
   const uint32_t Kr = subkey & 0xFFFFFFFF;
   
   Xr ^= Bits::rotateLeft(Xl & Kl, 1);
   Xl ^= Xr | Kr;
   
   return (static_cast<uint64_t>(Xl) << 32) | Xr;
}

uint64_t Camellia::FLInverse(const uint64_t &half_block, const uint64_t &subkey) const
{
   uint32_t Xl = half_block >> 32;
   uint32_t Xr = half_block & 0xFFFFFFFF;
   const uint32_t Kl = subkey >> 32;
   const uint32_t Kr = subkey & 0xFFFFFFFF;
   
   Xl ^= Xr | Kr;
   Xr ^= Bits::rotateLeft(Xl & Kl, 1);
   
   return (static_cast<uint64_t>(Xl) << 32) | Xr;
}

void Camellia::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   for(uint8_t j = 0; j < 6; j += 2)
   {
      R ^= F(L ^ subkeys[j], 0);
      L ^= F(R ^ subkeys[j + 1], 0);
   }
   
   const uint8_t big_rounds = rounds >> 3;
   for(uint8_t i = 0; i < big_rounds; ++i)
   {
      const uint8_t index = 6 * i;
      const uint8_t Ke_index = index / 3;
      L = FL(L, Ke[Ke_index]);
      R = FLInverse(R, Ke[Ke_index + 1]);
      
      for(uint8_t j = 0; j < 6; j += 2)
      {
         R ^= F(L ^ subkeys[index + j + 6], 0);
         L ^= F(R ^ subkeys[index + j + 7], 0);
      }
   }
   
   R ^= Kw[2];
   L ^= Kw[3];
}

void Camellia::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   const uint8_t lower_bound = subkeys.size() - 6;
   for(uint8_t i = subkeys.size() - 1; i >= lower_bound; i -= 2)
   {
      R ^= F(L ^ subkeys[i], 0);
      L ^= F(R ^ subkeys[i - 1], 0);
   }
   
   const uint8_t big_rounds = rounds >> 3;
   for(int8_t j = big_rounds - 1; j >= 0; --j)
   {
      const uint8_t index = 6 * j;
      const uint8_t Ke_index = index / 3;
      L = FL(L, Ke[Ke_index + 1]);
      R = FLInverse(R, Ke[Ke_index]);
      
      for(int8_t i = 5; i >= 0; i -= 2)
      {
         R ^= F(L ^ subkeys[index + i], 0);
         L ^= F(R ^ subkeys[index + i - 1], 0);
      }
   }
   R ^= Kw[0];
   L ^= Kw[1];
}

const UInt64Vector Camellia::encodeBlock(const UInt64Vector &input)
{
   uint64_t L0 = input[0] ^ Kw[0];
   uint64_t R0 = input[1] ^ Kw[1];
   encodeFeistelRounds(L0, R0, 0);
   
   return {R0, L0};
}

const UInt64Vector Camellia::decodeBlock(const UInt64Vector &input)
{
   uint64_t L0 = input[0] ^ Kw[2];
   uint64_t R0 = input[1] ^ Kw[3];
   decodeFeistelRounds(L0, R0, 0);
   
   return {R0, L0};
}