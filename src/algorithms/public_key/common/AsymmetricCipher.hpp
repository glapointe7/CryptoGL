
#pragma once

#include "../../common/Cipher.hpp"
#include "../../../core/types/BigInteger.hpp"

namespace CryptoGL
{
    using BigIntVector = Vector<BigInteger>;

    class AsymmetricCipher : public Cipher<BigIntVector, BytesVector>
    {   
    protected:  
       virtual BigIntVector encode(const BytesVector &) = 0;
       virtual BytesVector decode(const BigIntVector &) = 0;
       virtual void makePublicKey() = 0;
    };
}