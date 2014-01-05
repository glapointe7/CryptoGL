#include "Camellia.hpp"

#include "BigEndian.hpp"
#include "Bits.hpp"
#include "Vector.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr std::array<uint64_t, 6> Camellia::key_sigma;
constexpr std::array<uint32_t, 256> Camellia::SP1110;
constexpr std::array<uint32_t, 256> Camellia::SP0222;
constexpr std::array<uint32_t, 256> Camellia::SP3033;
constexpr std::array<uint32_t, 256> Camellia::SP4404;

void Camellia::setKey(const BytesVector &key)
{
   const uint8_t key_size = key.size();
   if (key_size != 16 && key_size != 24 && key_size != 32)
   {
      throw BadKeyLength("Your key length has to be 16, 24 or 32 bytes.", key_size);
   }
   
   if(key_size == 16)
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
   
   BytesVector K = Vector::Xor(Kl, Kr);
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
   Vector::extend(Ka, BigEndian64::toBytesVector(K2));
         
   /* Prewhitening phase. */
   Kw.reserve(4);
   Vector::extend(Kw, BigEndian64::toIntegersVector(Kl));
   subkeys.reserve(rounds);
   
   if(rounds == 18)
   {
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Ka));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 15)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 15)));
      Vector::extend(Ke, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 30)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 45)));

      subkeys.push_back(BigEndian64::toIntegerRange(Bits::rotateLeft128(Ka, 45), 8));
      subkeys.push_back(BigEndian64::toIntegerRange(Bits::rotateLeft128(Kl, 60), 8, 16));

      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 60)));
      Vector::extend(Ke, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 77)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 94)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 94)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 111)));
      Vector::extend(Kw, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 111)));
   }
   else
   {
      K = Vector::Xor(Ka, Kr);
      K1 = BigEndian64::toInteger(BytesVector(K.begin(), K.begin() + 8));
      K2 = BigEndian64::toInteger(BytesVector(K.begin() + 8, K.end()));
      K2 ^= F(K1 ^ key_sigma[4], 4);
      K1 ^= F(K2 ^ key_sigma[5], 5);

      BytesVector Kb = BigEndian64::toBytesVector(K1);
      Kb.reserve(16);
      Vector::extend(Kb, BigEndian64::toBytesVector(K2));
      
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Kb));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kr, 15)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 15)));
      Vector::extend(Ke, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kr, 30)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kb, 30)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 45)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 45)));
      Vector::extend(Ke, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 60)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kr, 60)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kb, 60)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 77)));
      Vector::extend(Ke, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 77)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kr, 94)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Ka, 94)));
      Vector::extend(subkeys, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kl, 111)));
      Vector::extend(Kw, BigEndian64::toIntegersVector(Bits::rotateLeft128(Kb, 111)));
   }   
}

// Used technic of P.29 from the specs.
uint64_t Camellia::F(const uint64_t half_block, const uint8_t) const
{
   uint32_t D = SP1110[half_block & 0xFF] ^ SP0222[(half_block >> 24) & 0xFF] 
           ^ SP3033[(half_block >> 16) & 0xFF] ^ SP4404[(half_block >> 8) & 0xFF];
   
   uint32_t U = SP1110[(half_block >> 56) & 0xFF] ^ SP0222[(half_block >> 48) & 0xFF] 
           ^ SP3033[(half_block >> 40) & 0xFF] ^ SP4404[(half_block >> 32) & 0xFF];
   
   D ^= U;
   U = D ^ Bits::rotateRight(U, 8);
   
   return (static_cast<uint64_t>(D) << 32) | U;
}

uint64_t Camellia::FL(const uint64_t &half_block, const uint64_t &subkey)
{
   uint32_t Xl = half_block >> 32;
   uint32_t Xr = half_block & 0xFFFFFFFF;
   const uint32_t Kl = subkey >> 32;
   const uint32_t Kr = subkey & 0xFFFFFFFF;
   
   Xr ^= Bits::rotateLeft(Xl & Kl, 1);
   Xl ^= Xr | Kr;
   
   return (static_cast<uint64_t>(Xl) << 32) | Xr;
}

uint64_t Camellia::FLInverse(const uint64_t &half_block, const uint64_t &subkey)
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
   
   const uint8_t big_rounds = rounds / 8;
   for(uint8_t i = 0; i < big_rounds; ++i)
   {
      const uint8_t Ke_index = 2 * i;
      L = FL(L, Ke[Ke_index]);
      R = FLInverse(R, Ke[Ke_index + 1]);
      
      const uint8_t index = 6 * i;
      for(uint8_t j = 6; j < 12; j += 2)
      {
         R ^= F(L ^ subkeys[index + j], 0);
         L ^= F(R ^ subkeys[index + j + 1], 0);
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
   
   const uint8_t big_rounds = rounds / 8;
   for(int8_t j = big_rounds - 1; j >= 0; --j)
   {
      const uint8_t Ke_index = 2 * j;
      L = FL(L, Ke[Ke_index + 1]);
      R = FLInverse(R, Ke[Ke_index]);
      
      const uint8_t index = 6 * j;
      for(int8_t i = 5; i >= 0; i -= 2)
      {
         R ^= F(L ^ subkeys[index + i], 0);
         L ^= F(R ^ subkeys[index + i - 1], 0);
      }
   }
   R ^= Kw[0];
   L ^= Kw[1];
}

UInt64Vector Camellia::encodeBlock(const UInt64Vector &input)
{
   uint64_t L0 = input[0] ^ Kw[0];
   uint64_t R0 = input[1] ^ Kw[1];
   encodeFeistelRounds(L0, R0, 0);
   
   return {R0, L0};
}

UInt64Vector Camellia::decodeBlock(const UInt64Vector &input)
{
   uint64_t L0 = input[0] ^ Kw[2];
   uint64_t R0 = input[1] ^ Kw[3];
   decodeFeistelRounds(L0, R0, 0);
   
   return {R0, L0};
}