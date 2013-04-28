
#include "StringCipher.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

const StringCipher::ClassicalType StringCipher::ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
   std::ifstream in(filename.c_str());
   if (in)
   {
      ClassicalType contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      if (!contents.empty() && *contents.rbegin() == '\n')
      {
         contents.erase(contents.length() - 1, 1);
      }
      
      return contents;
   }
   throw errno;
}

void StringCipher::eraseBadCharacters(ClassicalType &text) const
{
   text.erase(std::remove_if(text.begin(), text.end(), [this](char c)
   {
      return alpha.find(c) == std::string::npos;
   }), text.end());
}

void StringCipher::setAlpha(const ClassicalType &alpha)
{
   this->alpha = alpha;
}

const StringCipher::ClassicalType StringCipher::getAlpha() const
{
   return alpha;
}
