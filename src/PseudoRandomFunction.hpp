
#pragma once

#include "Types.hpp"

namespace CryptoGL
{
    template <class SeedType, class GenerateType>
    class PseudoRandomFunction
    {
    protected:
        PseudoRandomFunction() { }
        explicit PseudoRandomFunction(const SeedType seed) : seed(seed) { }
        virtual ~PseudoRandomFunction() { }

        virtual GenerateType generate() = 0;

        SeedType seed;
    };
}