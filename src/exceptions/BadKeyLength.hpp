
#ifndef BADKEYLENGTH_HPP
#define BADKEYLENGTH_HPP

#include "BadKey.hpp"

#include "../String.hpp"

class BadKeyLength : public BadKey
{
public:

   BadKeyLength() {}

   BadKeyLength(const ErrorType &message, const uint32_t len)
   {
      const ErrorType number = String::uintToString(len);
      ErrorType str(message);
      str.reserve(message.length() + 27 + number.length());
      str.append("Your key length is : ");
      str.append(number);
      str.append(" byte(s).");

      BadKey(ErrorType(str));
   }
};

using BadIVLength = BadKeyLength;

#endif