#include "CaesarAttack.hpp"

#include <fstream>

void CaesarAttack::setFilename(const std::string &filename)
{
   this->filename = filename;
}

void CaesarAttack::save(std::ofstream &out, const ClassicalType &data)
{
   out << data << "\n\n";
}

void CaesarAttack::attack(ClassicalType &cipher_text)
{
   const uint8_t alpha_len = C->getAlpha().length();
   C->eraseBadCharacters(cipher_text);

   try
   {
      std::ofstream out(filename.c_str());

      for (uint8_t i = 1; i < alpha_len; ++i)
      {
         C->setKey(i);
         save(out, C->decode(cipher_text));
      }
      out.close();
   }
   catch (const std::exception &e)
   {
      throw e.what();
   }
}