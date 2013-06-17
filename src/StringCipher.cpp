
#include "StringCipher.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include "String.hpp"
#include "Tools.hpp"

#include "exceptions/EmptyAlpha.hpp"
#include "exceptions/MultipleChar.hpp"

StringCipher::StringCipher()
{
   alpha = String::uppercase;
}

void StringCipher::encodeFile(const std::string &clear_text_file, const std::string &cipher_text_file)
{
   // Opening files for reading and saving.
   std::ifstream in(clear_text_file.c_str(), std::ios::binary);
   std::ofstream out(cipher_text_file.c_str(), std::ios::binary);

   // Get the file size to read.
   in.seekg(0, std::ios::end);
   const uint64_t file_size = in.tellg();
   in.seekg(0, std::ios::beg);


   ClassicalType contents;
   const uint16_t size = std::numeric_limits<uint16_t>::max() - 1;
   if (file_size > size)
   {
      const uint64_t toRepeat = file_size / size;
      const uint64_t rest = file_size % size;
      for (uint64_t i = 0; i < toRepeat; ++i)
      {
         contents.reserve(size);
         contents.resize(size);
         in.read(&contents[0], size);
         out << this->encode(contents);
         contents.clear();
      }
      contents.reserve(rest);
      contents.resize(rest);
      in.read(&contents[0], rest);
      contents.erase(rest - 1);
      out << this->encode(contents);
   }
   else
   {
      contents.reserve(file_size);
      contents.resize(file_size);
      in.read(&contents[0], file_size);
      out << this->encode(contents);
   }

   in.close();
   out.close();
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