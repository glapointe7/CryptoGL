
#ifndef HASHFUNCTIONS_HPP
#define	HASHFUNCTIONS_HPP

#include <vector>
#include <string>

class HashFunctions
{
public:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<bool> BitsContainer;
   typedef std::vector<uint32_t> WordsContainer;
   
   virtual ~HashFunctions() {}
   
   virtual BytesContainer encode(const BytesContainer &) = 0;
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   static std::string hexDigest(const BytesContainer &data);
   static BytesContainer getBytesFromString(const std::string &str);
   static std::string getStringFromBytes(const BytesContainer &bytes);
   
protected:
   virtual BitsContainer addPadding(const BitsContainer &) = 0;
   
   static BitsContainer getBitsFromData(const BytesContainer &);
};

#endif