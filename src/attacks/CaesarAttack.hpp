
#ifndef CAESARATTACK_HPP
#define	CAESARATTACK_HPP

#include "Attack.hpp"
#include "../String.hpp"

#include "../Vigenere.hpp"

class CaesarAttack
{
public:
   CaesarAttack();
   ~CaesarAttack();
   
   const std::string load(const std::string &filename) const;
   void setAlpha(const std::string &alpha);
   void setFilename(const std::string &filename);
   virtual void attack(std::string &cipher_text) final;
   
private:
   Caesar *C;
   std::string filename;
};

#endif