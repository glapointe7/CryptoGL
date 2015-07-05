
#ifndef LRC_HPP
#define LRC_HPP

#include "Checksum.hpp"

namespace CryptoGL
{
    class LRC : public Checksum <uint8_t>
    {
    public:
        LRC() { }

        uint8_t encode(const BytesVector &input) override;
    };
}

#endif