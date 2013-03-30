
#include "StringCipher.h"

StringCipher::StringCipher()
{

}

void StringCipher::save(const std::string &)
{
   /*
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
    */
}
 
std::string StringCipher::load(const std::string &)
{
   /*
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
         if (keep_space == true)
         {
            data += " ";
         }
      }
      in.close();
   }
   catch (exception &e)
   {
      cout << "Erreur : " << e.what();
      return nullptr;
   }

   return data;
    */
   return "";
}

void StringCipher::setAlpha(const std::string &alpha)
{
   this->alpha = alpha;
}

std::string StringCipher::getAlpha() const
{
   return alpha;
}
