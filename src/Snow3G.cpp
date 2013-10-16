#include "Snow3G.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint8_t Snow3G::SR[];
constexpr uint8_t Snow3G::SQ[];

void Snow3G::setKey(const BytesVector &key)
{
   if (key.size() != 16)
   {
      throw BadKeyLength("Your key has to be 128 bits length.", key.size());
   }
   this->key = key;
   keySetup();
}

void Snow3G::setIV(const BytesVector &IV)
{
   if (IV.size() != 16)
   {
      throw BadIVLength("Your IV has to be 128 bits length.", IV.size());
   }
   this->IV = IV;
}

constexpr uint8_t Snow3G::mulx(const uint8_t V, const uint8_t c)
{
   return V & 0x80 ? (V << 1) ^ c : V << 1;
}

uint8_t Snow3G::mulxPow(const uint8_t V, const uint8_t i, const uint8_t c)
{
   if(i == 0)
      return V;
   
   return mulx(mulxPow(V, i-1, c), c);
}

uint32_t Snow3G::mulAlpha(const uint8_t c)
{
   return BigEndian32::toInteger({mulxPow(c, 23, 0xA9), mulxPow(c, 245, 0xA9), 
           mulxPow(c, 48, 0xA9), mulxPow(c, 239, 0xA9)});
}

uint32_t Snow3G::divAlpha(const uint8_t c)
{
   return BigEndian32::toInteger({mulxPow(c, 16, 0xA9), mulxPow(c, 39, 0xA9), 
           mulxPow(c, 6, 0xA9), mulxPow(c, 64, 0xA9)});
}

void Snow3G::shiftStages(const uint32_t v)
{
   stages[0] = stages[1]; stages[1] = stages[2]; stages[2] = stages[3]; stages[3] = stages[4];
   stages[4] = stages[5]; stages[5] = stages[6]; stages[6] = stages[7]; stages[7] = stages[8];
   stages[8] = stages[9]; stages[9] = stages[10]; stages[10] = stages[11]; stages[11] = stages[12];
   stages[12] = stages[13]; stages[13] = stages[14]; stages[14] = stages[15]; stages[15] = v;
}

uint32_t Snow3G::getv() const
{
   return ((stages[0] << 8) & 0xFFFFFF00) ^ mulAlpha(stages[0] >> 24) ^ stages[2]
                     ^ ((stages[11] >> 8) & 0x00FFFFFF) ^ divAlpha(stages[11] & 0xFF);
}

void Snow3G::initializationMode(const uint32_t F)
{
   shiftStages(getv() ^ F);
}

void Snow3G::keystreamMode()
{
   shiftStages(getv());
}

uint32_t Snow3G::S(const uint32_t w, const uint8_t *sbox, const uint8_t c)
{
   const uint8_t SBox[4] {sbox[w >> 24], sbox[(w >> 16) & 0xFF], sbox[(w >> 8) & 0xFF], sbox[w & 0xFF]};
   BytesVector r;
   r.reserve(4);
   r.push_back(mulx(SBox[0], c) ^ SBox[1] ^ SBox[2] ^ mulx(SBox[3], c) ^ SBox[3]);
   r.push_back(mulx(SBox[0], c) ^ SBox[0] ^ mulx(SBox[1], c) ^ SBox[2] ^ SBox[3]); 
   r.push_back(SBox[0] ^ mulx(SBox[1], c) ^ SBox[1] ^ mulx(SBox[2], c) ^ SBox[3]);
   r.push_back(SBox[0] ^ SBox[1] ^ mulx(SBox[2], c) ^ SBox[2] ^ mulx(SBox[3], c));
   
   return BigEndian32::toInteger(r);
}

uint32_t Snow3G::S1(const uint32_t w) const
{
   return S(w, SR, 0x1B);
}

uint32_t Snow3G::S2(const uint32_t w) const
{
   return S(w, SQ, 0x69);
}

uint32_t Snow3G::finiteStateMachine()
{
   const uint32_t F = (stages[15] + R1) ^ R2;
   const uint32_t r = R2 + (R3 ^ stages[5]);
   R3 = S2(R2);
   R2 = S1(R1);
   R1 = r;
   
   return F;
}

void Snow3G::keySetup()
{
   UInt32Vector K, Iv;
   K.reserve(4);
   Iv.reserve(4);
   
   for(uint8_t i = 0; i < 16; i += 4)
   {
      K.push_back(BigEndian32::toInteger(BytesVector(key.begin() + i, key.begin() + i + 4)));
      Iv.push_back(BigEndian32::toInteger(BytesVector(IV.begin() + i, IV.begin() + i + 4)));
   }
   
   stages.resize(16);
   stages[15] = K[3] ^ Iv[0];
   stages[14] = K[2];
   stages[13] = K[1];
   stages[12] = K[0] ^ Iv[1];
   stages[11] = K[3] ^ 0xFFFFFFFF;
   stages[10] = K[2] ^ 0xFFFFFFFF ^ Iv[2];
   stages[9] = K[1] ^ 0xFFFFFFFF ^ Iv[3];
   stages[8] = K[0] ^ 0xFFFFFFFF;
   stages[7] = K[3];
   stages[6] = K[2];
   stages[5] = K[1];
   stages[4] = K[0];
   stages[3] = K[3] ^ 0xFFFFFFFF;
   stages[2] = K[2] ^ 0xFFFFFFFF;
   stages[1] = K[1] ^ 0xFFFFFFFF;
   stages[0] = K[0] ^ 0xFFFFFFFF;
   
   R1 = R2 = R3 = 0;
   
   // The FSM is clocked producing the 32-bit words F.
   // Then, the LFSR is clocked in Initialization Mode consuming F.
   for(uint8_t i = 0; i < 32; ++i)
   {
      const uint32_t F = finiteStateMachine();
      initializationMode(F);
   }
}

UInt32Vector Snow3G::generateKeystream()
{
   finiteStateMachine();
   keystreamMode();
   
   UInt32Vector keystream;
   keystream.reserve(output_size);
   for(uint32_t t = 0; t < output_size; ++t)
   {
      const uint32_t F = finiteStateMachine();
      keystream.push_back(F ^ stages[0]);
      keystreamMode();
   }
   
   return keystream;
}