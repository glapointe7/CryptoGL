#include "Feistel.hpp"

void Feistel::processRoundFunction(uint64_t &L, uint64_t &R, const uint8_t round) const
{
   const uint64_t Li = R;
   const uint64_t Ri = L ^ F(R, subkeys[round]);
   L = Li;
   R = Ri;
}

void Feistel::encodeRounds(uint64_t &L, uint64_t &R) const
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      processRoundFunction(L, R, i);
   }
}

void Feistel::decodeRounds(uint64_t &L, uint64_t &R) const
{
   for (int8_t i = rounds - 1; i >= 0; --i)
   {
      processRoundFunction(L, R, i);
   }
}