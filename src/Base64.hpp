
#ifndef BASE64_HPP
#define BASE64_HPP
/*
 * Source : http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
 */
#include "Cipher.hpp"

#include <vector>
#include <string>

class Base64 : public Cipher<std::string, BytesVector>
{  
public:
   std::string encode(const BytesVector &clear_data) override;
   BytesVector decode(const std::string &cipher_data) override;
   
private:
   static constexpr uint8_t pad_character = '=';
   static const std::string alpha;
};

#endif