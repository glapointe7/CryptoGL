
#include "StringCipher.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

StringCipher::StringCipher()
{

}

void StringCipher::save(const std::string &filename, const string &data)
{
    try
   {
      ofstream out(filename.c_str());
      out << data;
      out.close();
   }
   catch (exception &e)
   {
      cout << "Erreur : " << e.what();
   }
}
 
std::string StringCipher::load(const std::string &filename)
{
    string data = "";

   // Ouverture du fichier en lecture.
   try
   {
      ifstream in(filename.c_str());

      while (!in.eof())
      {
         string tmp;
         in >> tmp;
         data += tmp;
      }
      
      in.close();
   }
   catch (exception &e)
   {
      cout << "Erreur : " << e.what();
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
