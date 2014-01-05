
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Types.hpp"

namespace Tools
{
   /* Convert uppercase to symbol[0] and lowercase to symbol[1]. */
   void convertMajMinToSymbol(std::string &text, const std::string symbol);

   std::string baseXtoBaseY(const std::string &number, const uint8_t from_base, const uint8_t to_base);
}

#endif