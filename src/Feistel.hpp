/*
 * Feistel network scheme.
 */
#ifndef FEISTEL_HPP
#define FEISTEL_HPP

#include "BlockCipher.hpp"

namespace CryptoGL
{
    template <class FeistelType, class DataType, uint8_t InputBlockSize, class IndianType, class SubkeyType = FeistelType>
    class Feistel : public BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>
    {
    protected:      
       /* Constructor with an IV : CBC, CFB and OFB modes are accepted. */
       Feistel(const OperationModes mode, const uint8_t round, const BytesVector &IV) 
          : BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>(mode, round, IV) {}

       /* Constructor with no IV needed : ECB and CTR modes are accepted. */
       Feistel(const OperationModes mode, const uint8_t round) 
          : Feistel(mode, round, {}) {}

       virtual ~Feistel() {}

       virtual FeistelType F(const FeistelType half_block, const uint8_t) const = 0;
       virtual void encodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
       virtual void decodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
    };
}

#endif