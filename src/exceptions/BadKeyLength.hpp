
#ifndef BADKEYLENGTH_HPP
#define	BADKEYLENGTH_HPP

#include "BadKey.hpp"

#include <sstream>

class BadKeyLength : public BadKey
{
public:

   BadKeyLength() {}

   BadKeyLength(const std::string &text, const uint32_t len)
   {
      std::stringstream ss;
      ss << text
              << " Your key length is : "
              << len
              << "byte(s).";

      BadKey(ss.str());
   }
};

#endif