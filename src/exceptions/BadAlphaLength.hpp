
#ifndef BADALPHALENGTH_HPP
#define BADALPHALENGTH_HPP

#include "BadAlpha.hpp"

#include "../String.hpp"

class BadAlphaLength : public BadAlpha
{
public:
   BadAlphaLength() {}
   BadAlphaLength(const ErrorType &message) : BadAlpha(message) {}
   BadAlphaLength(const ErrorType &message, const uint32_t len) 
   {
      const ErrorType number = String::uintToString(len);
      ErrorType str(message);
      str.reserve(message.length() + 27 + number.length());
      str.append(" Your alphabet length is : ");
      str.append(number);

      BadAlphaLength(ErrorType(str));
   }
};

#endif