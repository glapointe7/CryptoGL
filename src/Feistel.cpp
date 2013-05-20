#include "Feistel.hpp"

void Feistel::processFeistelRounds(uint64_t &L, uint64_t &R, const UInt64Container &subkeys, 
        const int8_t lower_round, const int8_t round_max)
{
   int8_t is_increasing = 1;
   if (lower_round < 0)
   {
      is_increasing = -1;
   }
   
   const int8_t upper_round = lower_round + round_max;
   for (int8_t round = lower_round; round <= upper_round; ++round)
   {
      const int8_t positive_round = round * is_increasing;
      const uint64_t Li = R;
      const uint64_t Ri = L ^ F(R, subkeys[positive_round]);
      L = Li;
      R = Ri;
   }
}