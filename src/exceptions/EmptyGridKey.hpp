
#ifndef EMPTYGRIDKEY_HPP
#define EMPTYGRIDKEY_HPP

#include "EmptyKey.hpp"

namespace CryptoGL
{
    class EmptyGridKey : public EmptyKey
    {
    public:
       EmptyGridKey() {}
       explicit EmptyGridKey(const ErrorType &message) : EmptyKey(message) {}
    };
}

#endif