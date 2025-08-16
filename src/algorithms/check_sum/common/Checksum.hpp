#pragma once

#include "../../../core/types/Types.hpp"

namespace CryptoGL
{
    template <typename DataType>
    class Checksum
    {
    public:
        virtual ~Checksum() = default;

        virtual DataType encode(const BytesVector &) = 0;
    };
}