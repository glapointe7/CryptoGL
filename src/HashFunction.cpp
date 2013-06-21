
#include "HashFunction.hpp"

#include <sstream>

const HashFunction::BytesContainer 
HashFunction::appendPadding(const BytesContainer &data, const Endianness Endian) const
{
   const uint64_t bytes_len = data.size() << 3;
   BytesContainer bytes_pad(data);
   bytes_pad.reserve((bytes_len >> 3) + 64);

   // Append a bit '1' at the end.
   bytes_pad.push_back(0x80);

   // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
   const uint8_t bytes_pad_len = (120 - (bytes_pad.size() & 0x3F)) & 0x3F;
   bytes_pad.insert(bytes_pad.end(), bytes_pad_len, 0);

   // Append the 64-bit of the initial bits length of data following the endianness.    
   if(Endian == Endianness::big_endian_64bits)
   {
      for (int8_t i = 56; i >= 0; i -= 8)
      {
         bytes_pad.push_back((bytes_len >> i) & 0xFF);
      }
   }
   else
   {
      for (uint8_t i = 0; i < 64; i += 8)
      {
         bytes_pad.push_back((bytes_len >> i) & 0xFF);
      }
   }

   return bytes_pad;
}

const HashFunction::WordsContainer
HashFunction::getLittleEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index)
{
   WordsContainer words;
   words.reserve(16);

   for (uint8_t k = 0; k < 64; k += 4)
   {
      const uint32_t word = bytes[k + block_index]
              | (bytes[k + block_index + 1] << 8)
              | (bytes[k + block_index + 2] << 16)
              | (bytes[k + block_index + 3] << 24);

      words.push_back(word);
   }

   return words;
}

const HashFunction::WordsContainer
HashFunction::getBigEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index)
{
   WordsContainer words;
   words.reserve(16);

   for (uint8_t k = 0; k < 64; k += 4)
   {
      const uint32_t word = bytes[k + block_index + 3]
              | (bytes[k + block_index + 2] << 8)
              | (bytes[k + block_index + 1] << 16)
              | (bytes[k + block_index] << 24);
      
      words.push_back(word);
   }
   
   return words;
}

const HashFunction::BytesContainer 
HashFunction::getBigEndianOutput(const uint16_t max_words, const WordsContainer &hash)
{
   BytesContainer output;
   output.reserve(max_words << 2);

   for (uint8_t j = 0; j < max_words; ++j)
   {
      for(int8_t i = 24; i >= 0; i -= 8)
      {
         output.push_back((hash[j] >> i) & 0xFF);
      }
   }

   return output;
}

const HashFunction::BytesContainer 
HashFunction::getLittleEndianOutput(const uint16_t max_words, const WordsContainer &hash)
{
   BytesContainer output;
   output.reserve(max_words << 2);

   for (uint8_t j = 0; j < max_words; ++j)
   {
      for(uint8_t i = 0; i < 32; i += 8)
      {
         output.push_back((hash[j] >> i) & 0xFF);
      }
   }

   return output;
}

// Transforme les octets en nombres hexadécimaux sous forme d'une string.
const std::string HashFunction::hexDigest(const BytesContainer &bytes)
{
   std::ostringstream ss;
   ss.setf(std::ios::hex, std::ios::basefield);
   ss << std::uppercase;

   for (const auto byte : bytes)
   {
      ss.fill('0');
      ss.width(2);
      ss << static_cast<uint16_t> (byte);
   }

   return ss.str();
}

const HashFunction::BytesContainer 
HashFunction::getBytesFromString(const std::string &str)
{  
   return BytesContainer(str.begin(), str.end());
}

const std::string HashFunction::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}

const HashFunction::BytesContainer
HashFunction::getBytesFromHexDigest(const std::string &hex_str)
{
   const uint32_t hex_len = hex_str.length();
   BytesContainer bytes;
   bytes.reserve(hex_len >> 1);

   for (uint32_t i = 0; i < hex_len; i += 2)
   {
      const std::string hexa = hex_str.substr(i, 2);
      std::istringstream ss(hexa);
      ss.setf(std::ios::hex, std::ios::basefield);
      uint16_t x;
      ss >> x;
      bytes.push_back(static_cast<uint8_t> (x));
   }

   return bytes;
}
