
#ifndef ATTACK_HPP
#define	ATTACK_HPP

#include <string>

class Attack
{
public:
   virtual ~Attack() {}
   virtual void attack(std::string &cipher_text) = 0;
   //virtual void save(const std::string &filename, const std::string &data) = 0;
};

#endif