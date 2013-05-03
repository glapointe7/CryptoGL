
#ifndef BADALPHALENGTH_HPP
#define	BADALPHALENGTH_HPP

#include "BadAlpha.hpp"

#include <sstream>

class BadAlphaLength : public BadAlpha
{
public:
   BadAlphaLength() {}
   BadAlphaLength(const std::string &text, const uint32_t len) 
   {
      std::stringstream ss;
      ss << text
              << " Your alpha length is : "
              << len;

      BadAlpha(ss.str());
   }
};

#endif