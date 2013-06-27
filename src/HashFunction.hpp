/*
 * Template for uint64 et uint32.
 * Duplicate code : Find a way to include little and big endian.
 */
#ifndef HASHFUNCTION_HPP
#define	HASHFUNCTION_HPP

#include <vector>
#include <string>

class HashFunction
{
protected:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<uint32_t> WordsContainer;
   typedef std::vector<uint64_t> DWordsContainer;
   
   enum class Endianness : uint8_t
   {
      little_endian,
      big_endian_32bits,
      big_endian_64bits
   };
   
   const BytesContainer appendPadding(const BytesContainer &data, const Endianness Endian) const;
   
   static const WordsContainer getLittleEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index);
   static const WordsContainer getBigEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index);
   static const BytesContainer getLittleEndianOutput(const uint16_t max_words, const WordsContainer &hash);
   static const BytesContainer getBigEndianOutput(const uint16_t max_words, const WordsContainer &hash);
   
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