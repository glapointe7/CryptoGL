/* 
 * File:   Data.cpp
 * Author: gabriel
 * 
 * Created on March 23, 2013, 2:48 PM
 */

#include "Data.h"
#include <fstream>
#include <iostream>

Data::Data() {
}

Data::Data(const Data& orig) {
}

Data::~Data() {
}

// Charge le contenu du fichier texte filename en conservant les espaces si
// keep_space est true.
string Data::load(const string filename, bool keep_space)
{
   string data = "";
    
   // Ouverture du fichier en lecture.
   try
   {
      ifstream in(filename.c_str());

      while(!in.eof())
      {
         string tmp;
         in >> tmp;
         data += tmp;
         if(keep_space == true)
         {
            data += " ";
         }
      }
      in.close();
   }
   catch(exception &e)
   {
      cout << "Erreur : " << e.what();
      return nullptr;
   }

   return data;
}

// Sauvegarde le texte data dans filename.
void Data::save(const string filename, const string data)
{
   try
   {
      ofstream out(filename.c_str());
      out << data;
      out.close();
   }
   catch(exception &e)
   {
      cout << "Erreur : " << e.what();
   }
}