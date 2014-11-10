/*
 * Source : http://cr.yp.to/snuffle/spec.pdf
 */
#ifndef SALSA20_HPP
#define SALSA20_HPP

#include "SynchronousStreamCipher.hpp"

#include "Endian.hpp"

#include <array>

namespace CryptoGL
{
    class Salsa20 : public SynchronousStreamCipher<UInt32Vector, BigEndian32, 64>
    {
    public:
       Salsa20(const BytesVector &key, const BytesVector &IV) { setIV(IV); setKey(key); }

       /* Generate 64 bytes of keystream. */
       UInt32Vector generateKeystream() override;

       void setIV(const BytesVector &IV);
       void setKey(const BytesVector &key) override;

    private:
       void keySetup() override;
       void IVSetup();

       static UInt32Vector quarterRound(const UInt32Vector &Y);
       static UInt32Vector rowRound(const UInt32Vector &Y);
       static UInt32Vector columnRound(const UInt32Vector &Y);
       static UInt32Vector doubleRound(const UInt32Vector &Y);

       BytesVector IV;
       BytesVector subkeys;
       uint64_t counter = 0;

       static constexpr std::array<std::array<uint8_t, 4>, 4> sigma = {{
          {{101, 120, 112, 97}}, {{110, 100, 32, 51}}, {{50, 45, 98, 121}}, {{116, 101, 32, 107}}
       }};

       static constexpr std::array<std::array<uint8_t, 4>, 4> tau = {{
          {{101, 120, 112, 97}}, {{110, 100, 32, 49}}, {{54, 45, 98, 121}}, {{116, 101, 32, 107}}
       }};
    };
}

#endif