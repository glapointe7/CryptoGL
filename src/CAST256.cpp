#include "CAST256.hpp"

#include "BigEndian.hpp"
#include "Bits.hpp"

constexpr std::array<std::array<uint32_t, 256>, 4> CAST256::S;

void CAST256::setKey(const BytesVector &key)
{
   const uint8_t keylen = key.size();
   if (keylen != 16 && keylen != 20 && keylen != 24 && keylen != 28 && keylen != 32)
   {
      throw BadKeyLength("Your key length has to be of length 16, 20, 24, 28 or 32 bytes.", key.size());
   }
   
   // Pad the key with 0 to get 256 bits length.
   BytesVector key_padded(key);
   if(key.size() < 32)
   {
      key_padded.reserve(32);
      key_padded.insert(key_padded.end(), 32 - key.size(), 0);
   }

   this->key = key_padded;
}

void CAST256::applyForwardQuadRound(UInt32Vector &beta, const uint8_t round) const
{
   const uint8_t j = round << 2;
   beta[2] ^= F1(beta[3], subkeys[j], Kr[j]);
   beta[1] ^= F2(beta[2], subkeys[j+1], Kr[j+1]);
   beta[0] ^= F3(beta[1], subkeys[j+2], Kr[j+2]);
   beta[3] ^= F1(beta[0], subkeys[j+3], Kr[j+3]);
}

void CAST256::applyReverseQuadRound(UInt32Vector &beta, const uint8_t round) const 
{
   const uint8_t j = round << 2;
   beta[3] ^= F1(beta[0], subkeys[j+3], Kr[j+3]);
   beta[0] ^= F3(beta[1], subkeys[j+2], Kr[j+2]);
   beta[1] ^= F2(beta[2], subkeys[j+1], Kr[j+1]);
   beta[2] ^= F1(beta[3], subkeys[j], Kr[j]);
}

void CAST256::applyForwardOctave(UInt32Vector &kappa, const uint8_t round) const
{
   const uint8_t j = round << 3;
   kappa[6] ^= F1(kappa[7], Tm[j], Tr[j]);
   kappa[5] ^= F2(kappa[6], Tm[j+1], Tr[j+1]);
   kappa[4] ^= F3(kappa[5], Tm[j+2], Tr[j+2]);
   kappa[3] ^= F1(kappa[4], Tm[j+3], Tr[j+3]);
   kappa[2] ^= F2(kappa[3], Tm[j+4], Tr[j+4]);
   kappa[1] ^= F3(kappa[2], Tm[j+5], Tr[j+5]);
   kappa[0] ^= F1(kappa[1], Tm[j+6], Tr[j+6]);
   kappa[7] ^= F2(kappa[0], Tm[j+7], Tr[j+7]);
}

void CAST256::generateSubkeys()
{
   subkeys.reserve(48);
   Kr.reserve(48);
   Tm.reserve(192);
   Tr.reserve(192);
   
   constexpr uint32_t Mm = 0x6ED9EBA1;
   constexpr uint8_t Mr = 17;
   uint32_t Cm = 0x5A827999;
   uint8_t Cr = 19;
   
   for(uint8_t i = 0; i < 192; ++i)
   {
      Tm.push_back(Cm);
      Cm += Mm;
      Tr.push_back(Cr);
      Cr = (Cr + Mr) & 0x1F;
   }
   
   UInt32Vector kappa;
   kappa.reserve(8);
   for(uint8_t i = 0; i < 32; i += 4)
   {
      kappa.push_back(BigEndian32::toInteger(BytesVector(key.begin() + i, key.begin() + i + 4)));
   }
   
   for(uint8_t i = 0; i < rounds; ++i)
   {
      applyForwardOctave(kappa, 2 * i);
      applyForwardOctave(kappa, 2*i + 1);
      for(uint8_t j = 0; j < 8; j += 2)
      {
         Kr.push_back(kappa[j] & 0x1F);
         subkeys.push_back(kappa[8-j-1]);
      }
   }
}

uint32_t CAST256::F1(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
   const uint32_t I = Bits::rotateLeft(Km + D, Kr);
   return ((S[0][getByteFromInteger(I, 3)] ^ S[1][getByteFromInteger(I, 2)])
           - S[2][getByteFromInteger(I, 1)]) + S[3][getByteFromInteger(I, 0)];
}

uint32_t CAST256::F2(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
   const uint32_t I = Bits::rotateLeft(Km ^ D, Kr);
   return ((S[0][getByteFromInteger(I, 3)] - S[1][getByteFromInteger(I, 2)])
           + S[2][getByteFromInteger(I, 1)]) ^ S[3][getByteFromInteger(I, 0)];
}

uint32_t CAST256::F3(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
   const uint32_t I = Bits::rotateLeft(Km - D, Kr);
   return ((S[0][getByteFromInteger(I, 3)] + S[1][getByteFromInteger(I, 2)])
           ^ S[2][getByteFromInteger(I, 1)]) - S[3][getByteFromInteger(I, 0)];
}

const UInt32Vector CAST256::encodeBlock(const UInt32Vector &input)
{
   UInt32Vector beta(input);
   for(uint8_t i = 0; i < 6; ++i)
   {
      applyForwardQuadRound(beta, i);
   }
   for(uint8_t i = 6; i < rounds; ++i)
   {
      applyReverseQuadRound(beta, i);
   }
   
   return beta;
}

const UInt32Vector CAST256::decodeBlock(const UInt32Vector &input)
{
   UInt32Vector beta(input);
   for(uint8_t i = rounds - 1; i >= 6; --i)
   {
      applyForwardQuadRound(beta, i);
   }
   for(int8_t i = 5; i >= 0; --i)
   {
      applyReverseQuadRound(beta, i);
   }
   
   return beta;
}