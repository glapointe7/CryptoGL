/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#ifndef SYMMETRICCIPHER_HPP
#define	SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class SymmetricCipher : public Cipher<std::vector<unsigned char>>
{
public:
   typedef std::vector<unsigned char> BytesContainer;
   
   // Useful constant modes for loading / saving data in a file.
   enum class io_mode : uint8_t 
   {
      str,           // string of characters (can be a string of hex).
      byte           // vector of bytes.
   };
   
   virtual ~SymmetricCipher() {}

   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;

   virtual void setKey(const BytesContainer &) = 0;

   static void save(const std::string &filename, const BytesContainer &data);
   static const BytesContainer load(const std::string &filename);

   static const std::string hexDigest(const BytesContainer &bytes);
   static const BytesContainer getBytesFromString(const std::string &str);
   static const BytesContainer getBytesFromHexDigest(const std::string &hex_str);
   static const std::string getStringFromBytes(const BytesContainer &bytes);

protected:   
   BytesContainer key;
};

#endif	
