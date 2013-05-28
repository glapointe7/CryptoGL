#include "Feistel.hpp"

void Feistel::processFeistelRounds(uint64_t &L, uint64_t &R, const UInt64Container &subkeys, 
        const uint8_t lower_round, const uint8_t rounds, const int8_t is_increasing)
{
   const int8_t upper_round = lower_round + (rounds * is_increasing);
   for (int8_t round = lower_round; round != upper_round; round += is_increasing)
   {
      const uint64_t Li = R;
      const uint64_t Ri = L ^ F(R, subkeys[round]);
      L = Li;
      R = Ri;
   }
}