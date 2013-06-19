
#ifndef BASE64_HPP
#define	BASE64_HPP
/*
 * Source : http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
 */
#include "Cipher.hpp"
//#include "String.hpp"
#include <vector>
#include <string>

class Base64 : public Cipher<std::string, std::vector<uint8_t> >
{
   typedef std::vector<uint8_t> BytesContainer;
   
public:
   virtual const std::string encode(const BytesContainer &clear_data) final;
   virtual const BytesContainer decode(const std::string &cipher_data) final;
   
   static const BytesContainer getBytesFromString(const std::string &str);
   static const std::string getStringFromBytes(const BytesContainer &bytes);
   
private:
   static const uint8_t pad_character = '=';
   static const std::string alpha;
};

#endif