/*
 * Interface for all cipher types : Symmetric and asymmetric ciphers.
 */
#pragma once

#include "../../core/types/Types.hpp"

namespace CryptoGL
{
    template <class OutputType, class InputType = OutputType>
    class Cipher
    {
    protected:
        virtual ~Cipher() = default;
        virtual OutputType encode(const InputType &) = 0;
        virtual InputType decode(const OutputType &) = 0;
    };
}