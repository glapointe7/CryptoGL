
#ifndef BADCHAR_HPP
#define BADCHAR_HPP

#include "BadAlpha.hpp"

class BadChar : public BadAlpha
{
public:

   BadChar() {}

   BadChar(const ErrorType &message, const char c)
   {
      ErrorType str(message);
      str.reserve(message.length() + 28);
      str.append(" The bad character is : '");
      str.push_back(c);
      str.append("'.");
      
      BadAlpha(ErrorType(str));
   }
};

#endif