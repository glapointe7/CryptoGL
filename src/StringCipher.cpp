
#include "StringCipher.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "String.hpp"
#include "Tools.hpp"
#include "exceptions/EmptyAlpha.hpp"
#include "exceptions/MultipleChar.hpp"

#include <iostream>

StringCipher::StringCipher()
{
   alpha = String::uppercase;
}

void StringCipher::save(const std::string &filename, const ClassicalType &data)
{
   try
   {
      std::ofstream out(filename.c_str());
      out << data;
      out.close();
   }
   catch (const std::exception &e)
   {
      throw e.what();
   }
}

const StringCipher::ClassicalType StringCipher::load(const std::string &filename)
{
   ClassicalType contents;
   std::ifstream in(filename.c_str(), std::ios::binary);
   
   in.seekg(0, std::ios::end);
   const std::ifstream::pos_type file_size = in.tellg();
   contents.reserve(file_size);
   contents.resize(file_size);
   in.seekg(0, std::ios::beg);
   in.read(&contents[0], file_size);
   in.close();

   return contents;
}

void StringCipher::eraseBadCharacters(ClassicalType &text) const
{
   text.erase(std::remove_if(text.begin(), text.end(), [this](char c) {
      return alpha.find(c) == std::string::npos;
   }), text.end());
}

void StringCipher::setAlpha(const ClassicalType &alpha)
{
   if (alpha.empty())
   {
      throw EmptyAlpha("Your alphabet is empty.");
   }

   if (!isUniqueChar(alpha))
   {
      throw MultipleChar("Your alphabet have to contain unique characters.");
   }

   this->alpha = alpha;
}

const StringCipher::ClassicalType StringCipher::getAlpha() const
{
   return alpha;
}

// Ajoute le caractère c text.length() % mod fois à la fin de text.

const StringCipher::ClassicalType
StringCipher::appendChars(const ClassicalType &text, const uint32_t mod, const char c)
{
   ClassicalType full_text(text);
   const uint32_t rest = text.length() % mod;
   if (rest != 0)
   {
      full_text.append(mod - rest, c);
   }

   return full_text;
}

// Vérifie si text contient au moins un caractère n'appartenant pas à alpha.
// Retourne 0 si aucun caractère n'est trouvé, sinon renvoie le premier caractère erroné.

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