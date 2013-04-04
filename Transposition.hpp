
#ifndef TRANSPOSITION_HPP
#define	TRANSPOSITION_HPP

#include "StringCipher.h"
#include <string>

class Transposition : public StringCipher
{
public:
   Transposition();
   virtual ~Transposition() {}
   
   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;
   
   void setKey(const std::string &key);
   
private:
   std::vector<std::string> setTable(const std::string &data) const;
   std::vector<std::string> sortColon(const std::vector<std::string> &table) const;
   
   std::string key;
};

#endif	/* TRANSPOSITION_HPP */

