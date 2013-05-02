
#ifndef EMPTYGRIDKEY_HPP
#define	EMPTYGRIDKEY_HPP

#include "EmptyKey.hpp"

class EmptyGridKey : public EmptyKey
{
public:
   EmptyGridKey() {}
   EmptyGridKey(const std::string &text) { message = text; }
};

#endif