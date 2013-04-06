
#include "StringCipher.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

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
      std::cout << "Erreur : " << e.what();
   }
}

std::string StringCipher::load(const std::string &filename)
{
   // Ouverture du fichier en lecture.
   /*std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
   if (in)
   {
      std::ostringstream contents;
      contents << in.rdbuf();
      in.close();
      return (contents.str());
   }
   throw(errno);*/

   std::string data = "";
   try
   {
      std::ifstream in(filename.c_str());

      while (!in.eof())
      {
         std::string tmp;
         in >> tmp;
         data += tmp;
      }

      in.close();
   }
   catch (std::exception &e)
   {
      std::cout << "Erreur : " << e.what();
      return "";
   }

   return data;
}

void StringCipher::setAlpha(const std::string &alpha)
{
   this->alpha = alpha;
}

std::string StringCipher::getAlpha() const
{
   return alpha;
}
