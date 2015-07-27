
#ifndef EMPTYALPHA_HPP
#define EMPTYALPHA_HPP

#include "BadAlpha.hpp"

namespace CryptoGL
{
    class EmptyAlpha : public BadAlpha
    {
    public:
       EmptyAlpha() {}
       explicit EmptyAlpha(const ErrorType &message) : BadAlpha(message) {}
    };
}

#endif