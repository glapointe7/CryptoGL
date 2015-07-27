
#ifndef MULTIPLECHAR_HPP
#define MULTIPLECHAR_HPP

#include "BadAlpha.hpp"

namespace CryptoGL
{
    class MultipleChar : public BadAlpha
    {
    public:
       MultipleChar();
       explicit MultipleChar(const ErrorType &message) : BadAlpha(message) {}
    };
}

#endif