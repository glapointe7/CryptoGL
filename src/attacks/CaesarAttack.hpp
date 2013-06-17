
#ifndef CAESARATTACK_HPP
#define	CAESARATTACK_HPP

#include "ClassicalAttack.hpp"

#include "../Vigenere.hpp"

class CaesarAttack : public ClassicalAttack<Caesar>
{
public:
   CaesarAttack() { C = new Caesar(0); }
   ~CaesarAttack() { delete C; }
   
   void setFilename(const std::string &filename);
   virtual void attack(ClassicalType &cipher_text) final;
   
private:
   static void save(std::ofstream &out, const ClassicalType &data);
   
   Caesar *C;
   std::string filename;
};

#endif