
#ifndef BADALPHALENGTH_HPP
#define BADALPHALENGTH_HPP

#include "BadAlpha.hpp"

#include "../String.hpp"

class BadAlphaLength : public BadAlpha
{
public:
   BadAlphaLength() { }
   explicit BadAlphaLength(const ErrorType &message) : BadAlpha(message) { }
   BadAlphaLength(const ErrorType &message, const uint32_t len)
      : BadAlpha(message + " Your alphabet length is : " + String::uintToString(len)) { }
};

#endif