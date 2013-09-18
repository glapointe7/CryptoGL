
#ifndef BADKEYLENGTH_HPP
#define BADKEYLENGTH_HPP

#include "BadKey.hpp"

#include "../String.hpp"

class BadKeyLength : public BadKey
{
public:

   BadKeyLength() {}
   explicit BadKeyLength(const ErrorType &message) : BadKey(message) {}
   BadKeyLength(const ErrorType &message, const uint32_t len)
      : BadKey(message + "Your key length is : " + String::uintToString(len)) { }
};

using BadIVLength = BadKeyLength;

#endif