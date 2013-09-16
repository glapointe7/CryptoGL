
#ifndef EMPTYGRIDKEY_HPP
#define EMPTYGRIDKEY_HPP

#include "EmptyKey.hpp"

class EmptyGridKey : public EmptyKey
{
public:
   EmptyGridKey() {}
   explicit EmptyGridKey(const ErrorType &message) : EmptyKey(message) {}
};

#endif