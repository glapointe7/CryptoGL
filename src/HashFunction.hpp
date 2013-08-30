/*
 * HMAC source : http://tools.ietf.org/html/rfc2104
 */
#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

#include <vector>
#include <string>

#include "Types.hpp"

#include "BigEndian.hpp"
#include "LittleEndian.hpp"

template <class UInt, class Endian>
class HashFunction
{
protected:
   typedef std::vector<UInt> UIntContainer;
   
   explicit HashFunction(const uint8_t in_block_length) : in_block_length(in_block_length) {}
   virtual ~HashFunction() {}
   
   /* Input block length in bytes for a hash function. */
   const uint8_t in_block_length;
   
   virtual const BytesVector encode(const BytesVector &) = 0;
     
   virtual const BytesVector appendPadding(const BytesVector &data) const
   {
      const uint8_t rest = sizeof(UInt) << 1;
      BytesVector bytes_pad(data);
      bytes_pad.reserve(data.size() + (in_block_length << 1));

      // Append a bit '1' at the end.
      bytes_pad.push_back(0x80);

      // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
      const uint8_t bytes_pad_len = ((in_block_length << 1) - rest - (bytes_pad.size() & (in_block_length - 1))) & (in_block_length - 1);
      bytes_pad.insert(bytes_pad.end(), bytes_pad_len + rest - 8, 0);

      return bytes_pad;
   }
   
   /* Append the initial length of the message after padding. */
   template<class Endian_type>
   static void appendLength(BytesVector &bytes, const uint64_t &length)
   {
      const BytesVector bytes_pad = Endian_type::toBytesVector(length);
      bytes.insert(bytes.end(), bytes_pad.begin(), bytes_pad.end());
   }
   
   const UIntContainer getInputBlocks(const BytesVector &bytes, const uint64_t &block_index) const
   {      
      const uint8_t UInt_size = sizeof(UInt);

      UIntContainer words;
      words.reserve(in_block_length / UInt_size);
      for (uint8_t k = 0; k < in_block_length; k += UInt_size)
      {
         words.push_back(Endian::toInteger(BytesVector(bytes.begin() + k + block_index, bytes.begin() + k + block_index + UInt_size)));
      }
      
      return words;
   }
   
   static const BytesVector getOutput(const uint8_t max_words, const UIntContainer &hash)
   {
      const uint8_t UInt_size = sizeof(UInt);
      BytesVector output;
      output.reserve(max_words << 2);

      uint16_t max = max_words;
      if(UInt_size == 8)
      {
         max >>= 1;
      }
      
      for (uint8_t j = 0; j < max; ++j)
      {
         const BytesVector bytes = Endian::toBytesVector(hash[j]);
         output.insert(output.end(), bytes.begin(), bytes.end());
      }
      
      if (max_words % 2 && UInt_size == 8)
      {
         const BytesVector bytes = Endian::toBytesVector(hash[max]);
         output.insert(output.end(), bytes.begin(), bytes.begin() + 4);
      }

      return output;
   }
   
public:
   const BytesVector hmacEncode(const BytesVector &hmac_key, const BytesVector &message)
   {
      BytesVector key(hmac_key);
      if(hmac_key.size() > in_block_length)
      {
         key = encode(key);
      }
      
      BytesVector out_key_pad(in_block_length, 0x5C);
      BytesVector in_key_pad(in_block_length, 0x36);
      
      const uint8_t key_len = key.size();
      for(uint8_t i = 0; i < key_len; ++i)
      {
         out_key_pad[i] ^= key[i];
         in_key_pad[i] ^= key[i];
      }
      in_key_pad.insert(in_key_pad.end(), message.begin(), message.end());
      
      const BytesVector in_key_pad_encoded = encode(in_key_pad);
      out_key_pad.insert(out_key_pad.end(), in_key_pad_encoded.begin(), in_key_pad_encoded.end());
      
      return encode(out_key_pad);
   }
};

#endif