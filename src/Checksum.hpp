#pragma once

#include "Types.hpp"

namespace CryptoGL
{
    template <class DataType>
    class Checksum
    {
    public:
        virtual ~Checksum() { }

        virtual DataType encode(const BytesVector &) = 0;
    };
}