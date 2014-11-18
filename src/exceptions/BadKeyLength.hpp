
#ifndef BADKEYLENGTH_HPP
#define BADKEYLENGTH_HPP

#include "BadKey.hpp"

#include "../String.hpp"
#include "../Integer.hpp"

namespace CryptoGL
{
    class BadKeyLength : public BadKey
    {
    public:

       BadKeyLength() {}
       explicit BadKeyLength(const ErrorType &message) : BadKey(message) {}
       BadKeyLength(const ErrorType &message, const uint32_t len)
          : BadKey(message + "Your key length is : " + uint32::toString(len)) { }
    };

    using BadIVLength = BadKeyLength;
}

#endif