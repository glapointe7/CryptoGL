#include "BadKeyLength.hpp"

#include <sstream>

BadKeyLength::BadKeyLength(const std::string &text, const uint32_t len)
{
   std::stringstream ss;
   ss << text
      << " Your key length is : "
      << len
      << "byte(s)";
   
   message = ss.str();
}