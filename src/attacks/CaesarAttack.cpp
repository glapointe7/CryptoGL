#include "CaesarAttack.hpp"

#include <fstream>

CaesarAttack::CaesarAttack()
{
   C = new Caesar();
}

CaesarAttack::~CaesarAttack()
{
   delete C;
}

void CaesarAttack::setFilename(const std::string &filename)
{
   this->filename = filename;
}

const std::string CaesarAttack::load(const std::string &filename) const
{
   return C->load(filename);
}

void CaesarAttack::setAlpha(const std::string &alpha)
{
   C->setAlpha(alpha);
}

void CaesarAttack::attack(std::string &cipher_text)
{
   uint32_t alpha_len = C->getAlpha().length();
   C->eraseBadCharacters(cipher_text);

   try
   {
      std::ofstream out(filename.c_str());

      for (uint8_t i = 1; i < alpha_len; ++i)
      {
         C->setKey(i);
         out << C->decode(cipher_text) << "\n\n";
      }
      out.close();
   }
   catch (const std::exception &e)
   {
      throw e.what();
   }
}