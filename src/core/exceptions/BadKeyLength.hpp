
#ifndef BADKEYLENGTH_HPP
#define BADKEYLENGTH_HPP

#include "BadKey.hpp"
#include "../types/String.hpp"
#include "../types/Integer.hpp"

namespace CryptoGL
{
    class BadKeyLength : public BadKey
    {
    public:

       BadKeyLength() {}
       explicit BadKeyLength(const ErrorType &message) : BadKey(message) {}
       BadKeyLength(const ErrorType &message, const uint32_t len)
          : BadKey(message + "Your key length is : " + String(std::to_string(len))) { }
    };

    using BadIVLength = BadKeyLength;
}

#endif