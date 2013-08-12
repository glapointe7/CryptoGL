#include "StringCipher.hpp"

#include <string>
#include <algorithm>

#include "Tools.hpp"

#include "exceptions/EmptyAlpha.hpp"
#include "exceptions/MultipleChar.hpp"

void StringCipher::eraseBadCharacters(ClassicalType &text) const
{
   text.erase(std::remove_if(text.begin(), text.end(), [this](char c) {
      return alpha.find(c) == std::string::npos;
   }), text.end());
}

void StringCipher::checkAlpha(const ClassicalType &alpha) const
{
   if (alpha.empty())
   {
      throw EmptyAlpha("Your alphabet is empty.");
   }

   if (!isUniqueChar(alpha))
   {
      throw MultipleChar("Your alphabet have to contain unique characters.");
   }
}

void StringCipher::setAlpha(const ClassicalType &alpha)
{
   checkAlpha(alpha);
   this->alpha = alpha;
}

const StringCipher::ClassicalType StringCipher::getAlpha() const
{
   return alpha;
}

// Append the character c (text.length() % mod) times.

const StringCipher::ClassicalType
StringCipher::appendChars(const ClassicalType &data, const uint32_t to_add, const char c)
{
   ClassicalType full_text(data);
   const uint32_t rest = data.length() % to_add;
   if (rest != 0)
   {
      full_text.append(to_add - rest, c);
   }

   return full_text;
}

// Check if text has at least a character that doesn't belong to alpha.
// Return 0 if nothing is found, otherwise it returns the first char found.

const char StringCipher::badAlphaFound(const ClassicalType &text) const
{
   for (const auto c : text)
   {
      if (alpha.find(c) == std::string::npos)
      {
         return c;
      }
   }

   return 0;
}