
#ifndef HASHFUNCTION_HPP
#define	HASHFUNCTION_HPP

#include <vector>
#include <string>

class HashFunction
{
protected:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<uint32_t> WordsContainer;
   
   virtual const BytesContainer appendPadding(const BytesContainer &) const = 0;
   virtual const WordsContainer getWordBlocks(const BytesContainer &, const uint64_t &) const = 0;
   virtual const BytesContainer getOutput() const = 0;
   
public:   
   virtual ~HashFunction() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   
   /* Get the hexadecimal string from a vector of bytes. */
   static const std::string hexDigest(const BytesContainer &data);
   static const BytesContainer getBytesFromString(const std::string &str);
   static const std::string getStringFromBytes(const BytesContainer &bytes);
   static const BytesContainer getBytesFromHexDigest(const std::string &hex_str);
};

#endif