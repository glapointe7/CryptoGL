
#include "StringCipher.h"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

StringCipher::StringCipher()
{

}

void StringCipher::save(const std::string &filename, const std::string &data)
{
   try
   {
      std::ofstream out(filename.c_str());
      out << data;
      out.close();
   }
   catch (std::exception &e)
   {
       throw e.what();
   }
}

std::string StringCipher::load(const std::string &filename)
{
   std::ifstream in(filename.c_str());
   if (in)
   {
      std::string contents;
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

void StringCipher::eraseBadCharacters(std::string &text) const
{
   text.erase(std::remove_if(text.begin(), text.end(), [this](char c)
   {
      return alpha.find(c) == std::string::npos;
   }), text.end());
}

void StringCipher::setAlpha(const std::string &alpha)
{
   this->alpha = alpha;
}

std::string StringCipher::getAlpha() const
{
   return alpha;
}
