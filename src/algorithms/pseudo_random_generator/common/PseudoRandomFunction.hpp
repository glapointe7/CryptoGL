
#pragma once

#include "../../../core/types/Types.hpp"

namespace CryptoGL
{
    template <typename SeedType, typename GenerateType>
    class PseudoRandomFunction
    {
    protected:
        PseudoRandomFunction() = default;
        explicit PseudoRandomFunction(const SeedType seed) : seed(seed) { }
        virtual ~PseudoRandomFunction() = default;

        virtual GenerateType generate() = 0;

        SeedType seed;
    };
}