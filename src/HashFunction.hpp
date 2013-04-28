
#ifndef HASHFUNCTION_HPP
#define	HASHFUNCTION_HPP

#include <vector>
#include <string>

class HashFunction
{
public:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<bool> BitsContainer;
   typedef std::vector<uint32_t> WordsContainer;
   
   virtual ~HashFunction() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   static const std::string hexDigest(const BytesContainer &data);
   static const BytesContainer getBytesFromString(const std::string &str);
   static const std::string getStringFromBytes(const BytesContainer &bytes);
   
protected:
   virtual const BitsContainer addPadding(const BitsContainer &) = 0;
   
   static const BitsContainer getBitsFromData(const BytesContainer &);
};

#endif


