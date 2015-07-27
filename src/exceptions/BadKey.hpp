
#ifndef BADKEY_HPP
#define BADKEY_HPP

#include "Exception.hpp"

namespace CryptoGL
{
    class BadKey : public Exception
    {
    public:
       BadKey() {}
       explicit BadKey(const ErrorType &message) : Exception(message) {}
       virtual ~BadKey() {};
    };
}

#endif