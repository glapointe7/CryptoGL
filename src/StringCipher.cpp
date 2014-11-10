#include "StringCipher.hpp"

#include <algorithm>

#include "exceptions/EmptyAlpha.hpp"
#include "exceptions/MultipleChar.hpp"

using namespace CryptoGL;

void StringCipher::checkAlpha(const ClassicalType &alpha)
{
   if (alpha.empty())
   {
      throw EmptyAlpha("Your alphabet is empty.");
   }

   if (!alpha.containsUniqueChars())
   {
      throw MultipleChar("Your alphabet have to contain unique characters.");
   }
}

void StringCipher::setAlpha(const ClassicalType &alpha)
{
   checkAlpha(alpha);
   this->alpha = alpha;
}

ClassicalType StringCipher::getAlpha() const
{
   return alpha;
}

// Append the character c (text.length() % mod) times.

ClassicalType StringCipher::appendChars(ClassicalType data, const uint32_t to_add, const char c)
{
   const uint32_t rest = data.length() % to_add;
   if (rest != 0)
   {
      data.append(to_add - rest, c);
   }

   return data;
}

// Check if text has at least a character that doesn't belong to alpha.
// Return 0 if nothing is found, otherwise it returns the first char found.

int8_t StringCipher::badAlphaFound(const ClassicalType &text) const
{
   for (const auto c : text)
   {
      if (alpha.find(c) == ClassicalType::npos)
      {
         return c;
      }
   }

   return 0;
}