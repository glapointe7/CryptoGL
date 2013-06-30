
#ifndef DIGEST_HPP
#define DIGEST_HPP

#include <vector>
#include <string>

namespace Digest
{
   typedef std::vector<uint8_t> BytesContainer;
   
   /* Get the hexadecimal string from a vector of bytes. */
   const std::string hexDigest(const BytesContainer &bytes);
   
   const BytesContainer getBytesFromString(const std::string &str);
   
   const std::string getStringFromBytes(const BytesContainer &bytes);
   
   const BytesContainer getBytesFromHexDigest(const std::string &hex_str);
}

#endif