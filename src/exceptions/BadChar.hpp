
#ifndef BADCHAR_HPP
#define BADCHAR_HPP

#include "BadAlpha.hpp"

namespace CryptoGL
{
    class BadChar : public BadAlpha
    {
    public:

       BadChar() {}

       BadChar(const ErrorType &message, const char c)
        : BadAlpha(message + " The bad character is : '" + c + "'.") {}
    };
}

#endif